#include "mutex.h"
#include "common_defines.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>



FuriMutex* furi_mutex_alloc(FuriMutexType type) {
    
    // vTaskDelay(pdMS_TO_TICKS(500)); 
    Serial.println("[mutex] furi_mutex_alloc");
    // SemaphoreHandle_t hMutex = NULL;
    // Serial.println((uint32_t)hMutex);
    // hMutex = xSemaphoreCreateMutex();
    // return ((FuriMutex*)hMutex);
    SemaphoreHandle_t hMutex = NULL;


    if(type == FuriMutexTypeNormal) {
        hMutex = xSemaphoreCreateMutex();
    } else if(type == FuriMutexTypeRecursive) {
        hMutex = xSemaphoreCreateRecursiveMutex();
    } else {
        Serial.println("[mutex] Programming error");
    }

    if(type == FuriMutexTypeRecursive) {
        /* Set LSB as 'recursive mutex flag' */
        hMutex = (SemaphoreHandle_t)((uint32_t)hMutex | 1U);
    }
    return ((FuriMutex*)hMutex);
}

void furi_mutex_free(FuriMutex* instance) {
        if (instance) {
            // vSemaphoreDelete((SemaphoreHandle_t)instance);
            vSemaphoreDelete((SemaphoreHandle_t)((uint32_t)instance & ~1U));

            delete instance;
        }
}

FuriStatus furi_mutex_acquire(FuriMutex* instance, uint32_t timeout) {

    FuriStatus stat = FuriStatusOk;
    Serial.println("[mutex] furi_mutex_acquire");
    Serial.println((uint32_t)instance);

    // SemaphoreHandle_t hMutex = (SemaphoreHandle_t)((uint32_t)instance & ~1U);
    // /* Extract recursive mutex flag */
    // uint32_t rmtx = (uint32_t)instance & 1U;
    // if(hMutex == NULL) {
    //     Serial.println("[mutex] furi_mutex_acquire FuriStatusErrorParameter");
    //     return FuriStatusErrorParameter;
    // }
    // TickType_t tickTimeout = (timeout == 0U) ? portMAX_DELAY : pdMS_TO_TICKS(timeout);
    // BaseType_t result = xSemaphoreTake((SemaphoreHandle_t)instance, tickTimeout);
    // return (result == pdTRUE) ? FuriStatusOk : ((timeout != 0U) ? FuriStatusErrorTimeout : FuriStatusErrorResource);

    SemaphoreHandle_t hMutex;
    
    uint32_t rmtx;

    hMutex = (SemaphoreHandle_t)((uint32_t)instance & ~1U);

    /* Extract recursive mutex flag */
    rmtx = (uint32_t)instance & 1U;

    stat = FuriStatusOk;

    /*if(FURI_IS_IRQ_MODE()) {
        stat = FuriStatusErrorISR;
    } else */
    if(hMutex == NULL) {
        stat = FuriStatusErrorParameter;
    } else {
        if(rmtx != 0U) {
            if(xSemaphoreTakeRecursive(hMutex, timeout) != pdPASS) {
                if(timeout != 0U) {
                    stat = FuriStatusErrorTimeout;
                } else {
                    stat = FuriStatusErrorResource;
                }
            }
        } else {
            if(xSemaphoreTake(hMutex, timeout) != pdPASS) {
                if(timeout != 0U) {
                    stat = FuriStatusErrorTimeout;
                } else {
                    stat = FuriStatusErrorResource;
                }
            }
        }
    }
    /* Return execution status */
    return (stat);
}

FuriStatus furi_mutex_release(FuriMutex* instance) {
    FuriStatus stat = FuriStatusOk;
    Serial.println("[mutex] furi_mutex_release");
    // Serial.println((uint32_t)instance);
    // BaseType_t result = xSemaphoreGive(instance);    
    // return (result == pdTRUE) ? FuriStatusOk : FuriStatusErrorResource;

    SemaphoreHandle_t hMutex;
    // FuriStatus stat;
    uint32_t rmtx;

    hMutex = (SemaphoreHandle_t)((uint32_t)instance & ~1U);

    /* Extract recursive mutex flag */
    rmtx = (uint32_t)instance & 1U;

    stat = FuriStatusOk;

    /*if(FURI_IS_IRQ_MODE()) {
        stat = FuriStatusErrorISR;
    } else */
    if(hMutex == NULL) {
        stat = FuriStatusErrorParameter;
    } else {
        if(rmtx != 0U) {
            if(xSemaphoreGiveRecursive(hMutex) != pdPASS) {
                stat = FuriStatusErrorResource;
            }
        } else {
            if(xSemaphoreGive(hMutex) != pdPASS) {
                stat = FuriStatusErrorResource;
            }
        }
    }
    Serial.println("[mutex] furi_mutex_release xSemaphoreGive");
    /* Return execution status */
    return (stat);


}

FuriThreadId furi_mutex_get_owner(FuriMutex* instance) {
    // TaskHandle_t owner = xSemaphoreGetMutexHolder((SemaphoreHandle_t)instance);
    // return (FuriThreadId)owner;
    FuriThreadId owner = 0;
    SemaphoreHandle_t hMutex;

    hMutex = (SemaphoreHandle_t)((uint32_t)instance & ~1U);

    if((hMutex == NULL)) {
        owner = 0;
    } else {
        owner = (FuriThreadId)xSemaphoreGetMutexHolder(hMutex);
    }
    /* Return owner thread ID */
    return (owner);
}