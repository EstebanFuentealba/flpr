#include "timer.h"
#include <Arduino.h>

struct FuriTimerImpl {
    FuriTimerCallback callback;
    FuriTimerType type;
    void* context;
    unsigned long interval;
    unsigned long previousMillis;
    bool running;
};

FuriTimer* furi_timer_alloc(FuriTimerCallback func, FuriTimerType type, void* context) {
    FuriTimerImpl* timer = new FuriTimerImpl;
    timer->callback = func;
    timer->type = type;
    timer->context = context;
    timer->interval = 0;
    timer->previousMillis = 0;
    timer->running = false;
    return reinterpret_cast<FuriTimer*>(timer);
}

void furi_timer_free(FuriTimer* instance) {
    delete reinterpret_cast<FuriTimerImpl*>(instance);
}

static void furi_timer_handle_callback(void* arg) {
    FuriTimerImpl* timer = reinterpret_cast<FuriTimerImpl*>(arg);
    if (timer->callback) {
        timer->callback(timer->context);
        if (timer->type == FuriTimerTypeOnce) {
            timer->running = false;
        }
    }
}

FuriStatus furi_timer_start(FuriTimer* instance, uint32_t ticks) {
    FuriTimerImpl* timer = reinterpret_cast<FuriTimerImpl*>(instance);
    timer->interval = ticks;
    timer->previousMillis = millis();
    timer->running = true;
    if (timer->interval > 0) {
        esp_timer_create_args_t timerArgs;
        timerArgs.callback = furi_timer_handle_callback;
        timerArgs.arg = timer;
        timerArgs.dispatch_method = ESP_TIMER_TASK;
        timerArgs.name = "furi_timer";
        esp_timer_handle_t timerHandle;
        esp_timer_create(&timerArgs, &timerHandle);
        esp_timer_start_periodic(timerHandle, timer->interval * 1000);
        timer->context = reinterpret_cast<void*>(timerHandle);
    }
    return 1;
}

FuriStatus furi_timer_stop(FuriTimer* instance) {
    FuriTimerImpl* timer = reinterpret_cast<FuriTimerImpl*>(instance);
    timer->running = false;
    if (timer->interval > 0) {
        esp_timer_handle_t timerHandle = reinterpret_cast<esp_timer_handle_t>(timer->context);
        esp_timer_stop(timerHandle);
        esp_timer_delete(timerHandle);
        timer->context = nullptr;
    }
    return 1;
}

uint32_t furi_timer_is_running(FuriTimer* instance) {
    FuriTimerImpl* timer = reinterpret_cast<FuriTimerImpl*>(instance);
    return timer->running ? 1 : 0;
}

void furi_delay_tick(uint32_t ticks) {
    vTaskDelay(ticks / portTICK_PERIOD_MS);
}

uint32_t furi_kernel_get_tick_frequency() {
    return configTICK_RATE_HZ;
}
