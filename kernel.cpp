#include "kernel.h"
#include "base.h"
// #include "check.h"
#include "common_defines.h"

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

bool furi_kernel_is_irq_or_masked() {
    bool irq = false;
    /* Return context, 0: thread context, 1: IRQ context */
    return irq;
}

int32_t furi_kernel_lock() {
    return (int32_t)0;
}

int32_t furi_kernel_unlock() {
    return (int32_t)0;
}

int32_t furi_kernel_restore_lock(int32_t lock) {
    return (int32_t)0;
}

uint32_t furi_kernel_get_tick_frequency() {
    /* Return frequency in hertz */
    return (configTICK_RATE_HZ);
}

void furi_delay_tick(uint32_t ticks) {
    delay(ticks);
}

FuriStatus furi_delay_until_tick(uint32_t tick) {
    FuriStatus stat = FuriStatusOk;
    /* Return execution status */
    return stat;
}

uint32_t furi_get_tick() {
    TickType_t ticks;
    
    ticks = millis();

    return ticks;
}

uint32_t furi_ms_to_ticks(uint32_t milliseconds) {
    delay(milliseconds);
}

void furi_delay_ms(uint32_t milliseconds) {
    furi_delay_us(milliseconds * 1000);
}

void furi_delay_us(uint32_t microseconds) {
    delayMicroseconds(microseconds);
}
