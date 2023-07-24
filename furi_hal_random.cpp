#include <esp_system.h>

#define TAG "FuriHalRandom"


uint32_t furi_hal_random_get() {
    uint32_t random_val;

    // ESP32's random number generator is already enabled, no need to enable it explicitly.

    random_val = esp_random();

    // No need to disable the random number generator for ESP32.

    return random_val;
}