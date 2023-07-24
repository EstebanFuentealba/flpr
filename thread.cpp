#include "thread.h"
#include "kernel.h"
#include "memmgr.h"
#include "common_defines.h"
#include "esp_system.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/event_groups.h>
#include <freertos/timers.h>


#include "furi_string.h"

#define TAG "FuriThread"


#define THREAD_NOTIFY_INDEX 0 // Index 0 is used for stream buffers (En FreeRTOS, los índices de notificación comienzan desde cero, por lo que deberías cambiar el valor de THREAD_NOTIFY_INDEX a 0 en lugar de 1.)

/* Limits */
#define MAX_BITS_TASK_NOTIFY 31U
#define MAX_BITS_EVENT_GROUPS 24U

#define THREAD_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_TASK_NOTIFY) - 1U))
#define EVENT_FLAGS_INVALID_BITS (~((1UL << MAX_BITS_EVENT_GROUPS) - 1U))


constexpr size_t MY_TASK_STACK_SIZE = 1024;
static StaticTask_t myTaskBuffer;
static StackType_t myTaskStack[ MY_TASK_STACK_SIZE ];


typedef void* FuriThreadId;

struct FuriThread {
    FuriThreadState state;
    int32_t ret;

    FuriThreadCallback callback;
    void* context;

    FuriThreadStateCallback state_callback;
    void* state_context;

    char* name;
    char* appid;

    FuriThreadPriority priority;

    TaskHandle_t task_handle;
    size_t heap_size;

    // FuriThreadStdout output;

    // Keep all non-alignable byte types in one place,
    // this ensures that the size of this structure is minimal
    bool is_service;
    bool heap_trace_enabled;

    size_t stack_size;
};

static size_t __furi_thread_stdout_write(FuriThread* thread, const char* data, size_t size);
static int32_t __furi_thread_stdout_flush(FuriThread* thread);

/** Catch threads that are trying to exit wrong way */
__attribute__((__noreturn__)) void furi_thread_catch() { //-V1082
    // If you're here it means you're probably doing something wrong
    // with critical sections or with scheduler state
    asm volatile("nop"); // extra magic
    Serial.println("You are doing it wrong"); //-V779
    __builtin_unreachable();
}


FuriThread* furi_thread_alloc() {
    FuriThread* thread = (FuriThread*)malloc(sizeof(FuriThread));
    thread->is_service = false;

    return thread;
}

FuriThread* furi_thread_alloc_ex(const char* name, uint32_t stack_size, FuriThreadCallback callback, void* context) {
    FuriThread* thread = (FuriThread*)furi_thread_alloc();
    furi_thread_set_name(thread, name);
    furi_thread_set_stack_size(thread, stack_size);
    furi_thread_set_callback(thread, callback);
    furi_thread_set_context(thread, context);
    return thread;
}

void furi_thread_free(FuriThread* thread) {
    if(thread->name) free(thread->name);
    if(thread->appid) free(thread->appid);
    // furi_string_free(thread->output.buffer);

    free(thread);
}

void furi_thread_set_name(FuriThread* thread, const char* name) {
    // if(thread->name) free(thread->name);
    thread->name = name ? strdup(name) : NULL;
}

void furi_thread_set_appid(FuriThread* thread, const char* appid) {
    //  TODO: Fix free
    // if(thread->appid) free(thread->appid);
    thread->appid = appid ? strdup(appid) : NULL;
}

void furi_thread_mark_as_service(FuriThread* thread) {
    thread->is_service = true;
}

void furi_thread_set_stack_size(FuriThread* thread, size_t stack_size) {
    // if (thread == nullptr) {
    //     return;
    // }

    thread->stack_size = stack_size;
}

void furi_thread_set_callback(FuriThread* thread, FuriThreadCallback callback) {
    // if (thread == nullptr) {
    //     return;
    // }

    thread->callback = callback;
}

void furi_thread_set_context(FuriThread* thread, void* context) {
    // if (thread == nullptr) {
    //     return;
    // }

    thread->context = context;
}

void furi_thread_set_priority(FuriThread* thread, FuriThreadPriority priority) {
    if (thread == nullptr) {
        return;
    }

    thread->priority = priority;
}

static int32_t __furi_thread_stdout_flush(FuriThread* thread) {
    // FuriString* buffer = thread->output.buffer;
    // size_t size = furi_string_size(buffer);
    // if(size > 0) {
    //     __furi_thread_stdout_write(thread, furi_string_get_cstr(buffer), size);
    //     furi_string_reset(buffer);
    // }
    return 0;
}


static void furi_thread_set_state(FuriThread* thread, FuriThreadState state) {
    // furi_assert(thread);
    Serial.println("[thread] try set state");
    thread->state = state;
    Serial.println("[thread] set state");
    //  TODO: check state_callback
    // if(thread->state_callback != nullptr) {
    //     Serial.println("[thread] try set state_callback");
    //     thread->state_callback(state, thread->state_context);
    // }
}
static void furi_thread_body(void* context) {
    // furi_assert(context);
    FuriThread* thread = (FuriThread*)context;

    // store thread instance to thread local storage
    // furi_assert(pvTaskGetThreadLocalStoragePointer(NULL, 0) == NULL);
    // vTaskSetThreadLocalStoragePointer(NULL, 0, thread);

    // furi_assert(thread->state == FuriThreadStateStarting);
    furi_thread_set_state(thread, FuriThreadStateRunning);

    // TaskHandle_t task_handle = xTaskGetCurrentTaskHandle();
    // if(thread->heap_trace_enabled == true) {
    //     memmgr_heap_enable_thread_trace((FuriThreadId)task_handle);
    // }

    thread->ret = thread->callback(thread->context);

    // if(thread->heap_trace_enabled == true) {
    //     furi_delay_ms(33);
    //     thread->heap_size = memmgr_heap_get_thread_memory((FuriThreadId)task_handle);
    //     furi_log_print_format( //-V576
    //         thread->heap_size ? FuriLogLevelError : FuriLogLevelInfo,
    //         TAG,
    //         "%s allocation balance: %u",
    //         thread->name ? thread->name : "Thread",
    //         thread->heap_size);
    //     memmgr_heap_disable_thread_trace((FuriThreadId)task_handle);
    // }

    // furi_assert(thread->state == FuriThreadStateRunning);

    if(thread->is_service) {
        Serial.printf(
            "%s service thread TCB memory will not be reclaimed\n",
            thread->name ? thread->name : "<unknown service>");
    }

    // flush stdout
    // __furi_thread_stdout_flush(thread);

    furi_thread_set_state(thread, FuriThreadStateStopped);

    vTaskDelete(NULL);
    // furi_thread_catch();
}

void furi_thread_start(FuriThread* thread) {
    // if (thread == nullptr || thread->task_handle != nullptr) {
    //     Serial.println("furi_thread_start nullptr");
    //     return;
    // }
    
        Serial.println("[thread] try furi_thread_start");

        furi_thread_set_state(thread, FuriThreadStateStarting);
        Serial.println("[thread] furi_thread_start");
        uint32_t stack = thread->stack_size / sizeof(StackType_t);
        UBaseType_t priority = thread->priority ? thread->priority : FuriThreadPriorityNormal;

        
        if(thread->is_service) {
            Serial.println("[thread] try xTaskCreate");
        
            // BaseType_t result = xTaskCreatePinnedToCore((TaskFunction_t)thread->callback, thread->name, 2048, NULL, configMAX_PRIORITIES - 1, NULL, 1);
         thread->task_handle =  (TaskHandle_t)xTaskCreatePinnedToCore((TaskFunction_t)thread->callback, thread->name, 2048, NULL, thread->priority, NULL, 0);
            // thread->task_handle = (TaskHandle_t)xTaskCreatePinnedToCore((TaskFunction_t)thread->callback, thread->name, thread->stack_size / sizeof(StackType_t), NULL, thread->priority, &thread->context, 1);
            // BaseType_t result = (BaseType_t)xTaskCreateStatic(furi_thread_body,
            //     thread->name,
            //     stack,
            //     thread,
            //     priority,
            //     myTaskStack,
            //     &myTaskBuffer);
            // BaseType_t result = xTaskCreate(
            //             furi_thread_body, thread->name, stack, thread, priority, &thread->task_handle);            /* Task handle. */
            Serial.println("[thread] xTaskCreate");
        //     StackType_t xStack[sizeof(StackType_t) * stack];  // Tamaño de la pila de la tarea en palabras (configurable)
        //     StaticTask_t xTaskBuffer;
        //     thread->task_handle = xTaskCreateStatic(
        //         furi_thread_body,
        //         thread->name,
        //         stack,
        //         thread,
        //         priority,
        //         memmgr_alloc_from_pool(sizeof(StackType_t) * stack),
        //         memmgr_alloc_from_pool(sizeof(StaticTask_t))
        //         // xStack,
        //         // &xTaskBuffer
        //         );
        } else {
            // BaseType_t result = xTaskCreate(
            //     furi_thread_body, thread->name, stack, thread, priority, &thread->task_handle);

            //  Serial.println(result);

            // if (result != pdPASS) {
            //     furi_thread_set_state(thread, FuriThreadStateStopped);
            //     // thread->state = FuriThreadStateStopped;
            // }
        }


        
       
}

bool furi_thread_join(FuriThread* thread) {

    // !!! IMPORTANT NOTICE !!!
    //
    // If your thread exited, but your app stuck here: some other thread uses
    // all cpu time, which delays kernel from releasing task handle
    while(thread->task_handle) {
        furi_delay_ms(10);
    }
    return true;
}

FuriThreadId furi_thread_get_id(FuriThread* thread) {
    return thread->task_handle;
}

FuriThreadPriority furi_thread_get_current_priority() {
    #ifdef RUN_THREAD
        return static_cast<FuriThreadPriority>(uxTaskPriorityGet(nullptr));
    #endif
    return FuriThreadPriorityNone;
}
void furi_thread_set_state_callback(FuriThread* thread, FuriThreadStateCallback callback) {
    // furi_assert(thread);
    // furi_assert(thread->state == FuriThreadStateStopped);
    thread->state_callback = callback;
}

void furi_thread_yield() {
    taskYIELD();
}

FuriThreadId furi_thread_get_current_id() {
   
        return static_cast<FuriThreadId>(xTaskGetCurrentTaskHandle());
    
}

FuriThread* furi_thread_get_current() {
    FuriThreadId id = furi_thread_get_current_id();
    return static_cast<FuriThread*>(pvTaskGetThreadLocalStoragePointer(id, 0));

}

const char* furi_thread_get_name(FuriThreadId thread_id) {
    
    TaskHandle_t handle = static_cast<TaskHandle_t>(thread_id);
    return pcTaskGetTaskName(handle);
    
}

const char* furi_thread_get_appid(FuriThreadId thread_id) {
    // Not implemented for ESP32
    return " ";
}

uint32_t furi_thread_get_stack_space(FuriThreadId thread_id) {

    TaskHandle_t handle = static_cast<TaskHandle_t>(thread_id);
    return uxTaskGetStackHighWaterMark(handle) * sizeof(StackType_t);

}

FuriThreadStdoutWriteCallback furi_thread_get_stdout_callback() {
    // Not implemented for ESP32
    return nullptr;
}

void furi_thread_set_stdout_callback(FuriThreadStdoutWriteCallback callback) {
    // Not implemented for ESP32
}

size_t furi_thread_stdout_write(const char* data, size_t size) {
    // Not implemented for ESP32
    return 0;
}

int32_t furi_thread_stdout_flush() {
    // Not implemented for ESP32
    return 0;
}

void furi_thread_suspend(FuriThreadId thread_id) {

    vTaskSuspend(static_cast<TaskHandle_t>(thread_id));

}

void furi_thread_resume(FuriThreadId thread_id) {

    vTaskResume(static_cast<TaskHandle_t>(thread_id));

}

bool furi_thread_is_suspended(FuriThreadId thread_id) {

    TaskHandle_t handle = static_cast<TaskHandle_t>(thread_id);
    return eTaskGetState(handle) == eSuspended;

}
uint32_t furi_thread_flags_clear(uint32_t flags) {

        TaskHandle_t hTask;
        uint32_t rflags, cflags;

        /*if (FURI_IS_IRQ_MODE()) {
            rflags = static_cast<uint32_t>(FuriStatusErrorISR);
        } else*/ if ((flags & THREAD_FLAGS_INVALID_BITS) != 0U) {
            rflags = static_cast<uint32_t>(FuriStatusErrorParameter);
        } else {
            hTask = xTaskGetCurrentTaskHandle();

            // Limpiar las banderas sin consultar el estado anterior
            xTaskNotify(hTask, flags, eSetValueWithoutOverwrite);

            // Esperar a que se establezcan las banderas y obtener el estado actualizado
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(0));
            cflags = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(0));

            rflags = cflags;
        }
    
        /* Return flags before clearing */
        return rflags;

}




uint32_t furi_thread_flags_wait(uint32_t flags, uint32_t options, uint32_t timeout) {

    uint32_t rflags, nval;
    uint32_t clear;
    TickType_t t0, td, tout;
    BaseType_t rval;
    Serial.println("[thread] try furi_thread_flags_wait");
    // if (xPortInIsrContext()) {
    //     rflags = static_cast<uint32_t>(FuriStatusErrorISR);
    // } else 
        if ((flags & THREAD_FLAGS_INVALID_BITS) != 0U) {
        rflags = static_cast<uint32_t>(FuriStatusErrorParameter);
    } else {
        if ((options & FuriFlagNoClear) == FuriFlagNoClear) {
            clear = 0U;
        } else {
            clear = flags;
        }

        rflags = 0U;
        tout = timeout;

        t0 = xTaskGetTickCount();
        do {
            rval = xTaskNotifyWaitIndexed(THREAD_NOTIFY_INDEX, 0, clear, &nval, tout);

            if (rval == pdTRUE) {
                rflags &= flags;
                rflags |= nval;

                if ((options & FuriFlagWaitAll) == FuriFlagWaitAll) {
                    if ((flags & rflags) == flags) {
                        break;
                    } else {
                        if (timeout == 0U) {
                            rflags = static_cast<uint32_t>(FuriStatusErrorResource);
                            break;
                        }
                    }
                } else {
                    if ((flags & rflags) != 0) {
                        break;
                    } else {
                        if (timeout == 0U) {
                            rflags = static_cast<uint32_t>(FuriStatusErrorResource);
                            break;
                        }
                    }
                }

                /* Update timeout */
                td = xTaskGetTickCount() - t0;

                if (td > tout) {
                    tout = 0;
                } else {
                    tout -= td;
                }
            } else {
                if (timeout == 0) {
                    rflags = static_cast<uint32_t>(FuriStatusErrorResource);
                } else {
                    rflags = static_cast<uint32_t>(FuriStatusErrorTimeout);
                }
            }
        } while (rval != pdFALSE);
    }

    /* Return flags before clearing */
    return rflags;
}



uint32_t furi_thread_flags_set(FuriThreadId thread_id, uint32_t flags) {

    TaskHandle_t hTask = (TaskHandle_t)thread_id;
    uint32_t rflags;
    BaseType_t yield;

    if((hTask == NULL) || ((flags & THREAD_FLAGS_INVALID_BITS) != 0U)) {
        rflags = (uint32_t)FuriStatusErrorParameter;
    } else {
        rflags = (uint32_t)FuriStatusError;

        /*if(FURI_IS_IRQ_MODE()) {
            yield = pdFALSE;

            (void)xTaskNotifyIndexedFromISR(hTask, THREAD_NOTIFY_INDEX, flags, eSetBits, &yield);
            (void)xTaskNotifyAndQueryIndexedFromISR(
                hTask, THREAD_NOTIFY_INDEX, 0, eNoAction, &rflags, NULL);

            portYIELD_FROM_ISR(yield);
        } else {*/
            (void)xTaskNotifyIndexed(hTask, THREAD_NOTIFY_INDEX, flags, eSetBits);
            (void)xTaskNotifyAndQueryIndexed(
                hTask, THREAD_NOTIFY_INDEX, 0, eNoAction, &rflags);
        //}
    }
    /* Return flags after setting */
    return (rflags);
    // TaskHandle_t hTask = (TaskHandle_t)thread_id;
    // uint32_t rflags;
    // BaseType_t yield;

    // if ((hTask == NULL) || ((flags & THREAD_FLAGS_INVALID_BITS) != 0U)) {
    //     rflags = (uint32_t)FuriStatusErrorParameter;
    // } else {
    //     rflags = (uint32_t)FuriStatusError;

    //     if (FURI_IS_IRQ_MODE()) {
    //         yield = pdFALSE;

    //         xTaskNotifyIndexedFromISR(hTask, THREAD_NOTIFY_INDEX, flags, eSetValueWithOverwrite, &yield);
    //         xTaskNotifyAndQueryIndexedFromISR(
    //             hTask, THREAD_NOTIFY_INDEX, 0, eNoAction, &rflags, NULL);

    //         portYIELD_FROM_ISR(yield);
    //     } else {
    //         xTaskNotifyIndexed(hTask, THREAD_NOTIFY_INDEX, flags, eSetValueWithOverwrite);
    //         xTaskNotifyAndQueryIndexed(hTask, THREAD_NOTIFY_INDEX, 0, eNoAction, &rflags);
    //     }
    // }
    // /* Return flags after setting */
    // return rflags;
}
