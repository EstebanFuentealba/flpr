#pragma once
#include "stdbool.h"
#include <driver/gpio.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Number of gpio on one port
 */
#define GPIO_NUMBER (16U)

/**
 * Interrupt callback prototype
 */
typedef void (*GpioExtiCallback)(void* ctx);

/**
 * Gpio interrupt type
 */
typedef struct {
    GpioExtiCallback callback;
    void* context;
    volatile bool ready;
} GpioInterrupt;

/**
 * Gpio modes
 */
typedef enum {
    GpioModeInput,
    GpioModeOutputPushPull,
    GpioModeOutputOpenDrain,
    GpioModeAltFunctionPushPull,
    GpioModeAltFunctionOpenDrain,
    GpioModeAnalog,
    GpioModeInterruptRise,
    GpioModeInterruptFall,
    GpioModeInterruptRiseFall,
    GpioModeEventRise,
    GpioModeEventFall,
    GpioModeEventRiseFall,
} GpioMode;

/**
 * Gpio pull modes
 */
typedef enum {
    GpioPullNo,
    GpioPullUp,
    GpioPullDown,
} GpioPull;

/**
 * Gpio speed modes
 */
typedef enum {
    GpioSpeedLow,
    GpioSpeedMedium,
    GpioSpeedHigh,
    GpioSpeedVeryHigh,
} GpioSpeed;

/**
 * Gpio alternate functions
 */
typedef enum {
    GpioAltFnUnused = 0, /*!< just dummy value */
} GpioAltFn;

/**
 * Gpio structure
 */
typedef struct {
    // uint16_t port;
    gpio_num_t pin;
} GpioPin;

/**
 * GPIO initialization function, simple version
 * @param gpio  GpioPin
 * @param mode  GpioMode
 */
void furi_hal_gpio_init_simple(const GpioPin* gpio, const GpioMode mode);

/**
 * GPIO initialization function, normal version
 * @param gpio  GpioPin
 * @param mode  GpioMode
 * @param pull  GpioPull
 * @param speed GpioSpeed
 */
void furi_hal_gpio_init(
    const GpioPin* gpio,
    const GpioMode mode,
    const GpioPull pull,
    const GpioSpeed speed);

/**
 * GPIO initialization function, extended version
 * @param gpio  GpioPin
 * @param mode  GpioMode
 * @param pull  GpioPull
 * @param speed GpioSpeed
 * @param alt_fn GpioAltFn
 */
void furi_hal_gpio_init_ex(
    const GpioPin* gpio,
    const GpioMode mode,
    const GpioPull pull,
    const GpioSpeed speed,
    const GpioAltFn alt_fn);

/**
 * Add and enable interrupt
 * @param gpio GpioPin
 * @param cb   GpioExtiCallback
 * @param ctx  context for callback
 */
void furi_hal_gpio_add_int_callback(const GpioPin* gpio, GpioExtiCallback cb, void* ctx);

/**
 * Enable interrupt
 * @param gpio GpioPin
 */
void furi_hal_gpio_enable_int_callback(const GpioPin* gpio);

/**
 * Disable interrupt
 * @param gpio GpioPin
 */
void furi_hal_gpio_disable_int_callback(const GpioPin* gpio);

/**
 * Remove interrupt
 * @param gpio GpioPin
 */
void furi_hal_gpio_remove_int_callback(const GpioPin* gpio);

/**
 * GPIO write pin
 * @param gpio  GpioPin
 * @param state true / false
 */
static inline void furi_hal_gpio_write(const GpioPin* gpio, const bool state) {
    digitalWrite(gpio->pin, state);
}

/**
 * GPIO read pin
 * @param gpio  GpioPin
 * @return true / false
 */
static inline bool furi_hal_gpio_read(const GpioPin* gpio) {
    return digitalRead(gpio->pin);
}

#ifdef __cplusplus
}
#endif