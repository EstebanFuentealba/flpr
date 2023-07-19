#include "furi.h"
#include "furi_hal_gpio.h"
// #include <furi_hal_version.h>
#include "furi_hal_resources.h"
#include <Arduino.h>

#define GPIO_PIN_MAP(pin, prefix) \
    (((pin) == GPIO_NUM_0) ? prefix##0 :  \
    ((pin) == GPIO_NUM_1) ? prefix##1 :  \
    ((pin) == GPIO_NUM_2) ? prefix##2 :  \
    ((pin) == GPIO_NUM_3) ? prefix##3 :  \
    ((pin) == GPIO_NUM_4) ? prefix##4 :  \
    ((pin) == GPIO_NUM_5) ? prefix##5 :  \
    ((pin) == GPIO_NUM_6) ? prefix##6 :  \
    ((pin) == GPIO_NUM_7) ? prefix##7 :  \
    ((pin) == GPIO_NUM_8) ? prefix##8 :  \
    ((pin) == GPIO_NUM_9) ? prefix##9 :  \
    ((pin) == GPIO_NUM_10) ? prefix##10 :  \
    ((pin) == GPIO_NUM_11) ? prefix##11 :  \
    ((pin) == GPIO_NUM_12) ? prefix##12 :  \
    ((pin) == GPIO_NUM_13) ? prefix##13 :  \
    ((pin) == GPIO_NUM_14) ? prefix##14 :  \
    prefix##15)

// #define GET_SYSCFG_EXTI_LINE(pin) GPIO_PIN_MAP(pin, GPIO_NUM_)
// #define GET_EXTI_LINE(pin) GPIO_PIN_MAP(pin, GPIO_NUM_EXTI_LINE_)

static volatile GpioInterrupt gpio_interrupt[GPIO_NUMBER];

static uint8_t furi_hal_gpio_get_pin_num(const GpioPin* gpio) {
    uint8_t pin_num = 0;
    for(pin_num = 0; pin_num < GPIO_NUMBER; pin_num++) {
        if(gpio->pin & (1 << pin_num)) break;
    }
    return pin_num;
}

void furi_hal_gpio_init_simple(const GpioPin* gpio, const GpioMode mode) {
    furi_hal_gpio_init(gpio, mode, GpioPullNo, GpioSpeedLow);
}

void furi_hal_gpio_init(
    const GpioPin* gpio,
    const GpioMode mode,
    const GpioPull pull,
    const GpioSpeed speed) {
    // we cannot set alternate mode in this function
    // furi_check(mode != GpioModeAltFunctionPushPull);
    // furi_check(mode != GpioModeAltFunctionOpenDrain);

    furi_hal_gpio_init_ex(gpio, mode, pull, speed, GpioAltFnUnused);
}

void furi_hal_gpio_init_ex(
    const GpioPin* gpio,
    const GpioMode mode,
    const GpioPull pull,
    const GpioSpeed speed,
    const GpioAltFn alt_fn) {
    // uint32_t sys_exti_line = GET_SYSCFG_EXTI_LINE(gpio->pin);
    // uint32_t exti_line = GET_EXTI_LINE(gpio->pin);

    // portMUX_TYPE portMux = portMUX_INITIALIZER_UNLOCKED;
    // Configure GPIO with interrupts disabled
    // portENTER_CRITICAL(&portMux);

    // Set GPIO speed
    switch(speed) {
    case GpioSpeedLow:
        gpio_set_drive_capability( gpio->pin, GPIO_DRIVE_CAP_0);
        break;
    case GpioSpeedMedium:
        gpio_set_drive_capability( gpio->pin, GPIO_DRIVE_CAP_1);
        break;
    case GpioSpeedHigh:
    case GpioSpeedVeryHigh:
        gpio_set_drive_capability( gpio->pin, GPIO_DRIVE_CAP_3);
        break;
    default:
        break;
    }

    // Set GPIO pull-up/pull-down
    switch(pull) {
    case GpioPullNo:
        gpio_set_pull_mode(gpio->pin, GPIO_FLOATING);
        break;
    case GpioPullUp:
        gpio_set_pull_mode(gpio->pin, GPIO_PULLUP_ONLY);
        break;
    case GpioPullDown:
        gpio_set_pull_mode(gpio->pin, GPIO_PULLDOWN_ONLY);
        break;
    default:
        break;
    }

    // Set GPIO mode
    switch(mode) {
    case GpioModeInput:
        gpio_set_direction( gpio->pin, GPIO_MODE_INPUT);
        break;
    case GpioModeOutputPushPull:
    case GpioModeOutputOpenDrain:
        gpio_set_direction( gpio->pin, GPIO_MODE_OUTPUT);
        break;
    case GpioModeInterruptFall:
        gpio_set_direction( gpio->pin, GPIO_MODE_INPUT);
        gpio_set_intr_type( gpio->pin, GPIO_INTR_NEGEDGE);
        break;
    case GpioModeInterruptRise:
        gpio_set_direction( gpio->pin, GPIO_MODE_INPUT);
        gpio_set_intr_type( gpio->pin, GPIO_INTR_POSEDGE);
        break;
    case GpioModeInterruptRiseFall:
        gpio_set_direction( gpio->pin, GPIO_MODE_INPUT);
        gpio_set_intr_type( gpio->pin, GPIO_INTR_ANYEDGE);
        break;
    default:
        break;
    }

    // Disable interrupt for the GPIO pin
    gpio_intr_disable(gpio->pin);

    //  portEXIT_CRITICAL(&portMux);


    // Initialize interrupt related fields
    // gpio_interrupt[gpio->pin].handler = NULL;
    // gpio_interrupt[gpio->pin].context = NULL;

    // Map EXTI line to GPIO pin
    // gpio_matrix_in(gpio->pin, exti_line, false);

    // // Map GPIO to EXTI controller
    // gpio_matrix_in(gpio->port, sys_exti_line, true);
}

// void furi_hal_gpio_deinit(const GpioPin* gpio) {
//     gpio_reset_pin(gpio->pin);
// }

// void furi_hal_gpio_write(const GpioPin* gpio, const bool level) {
//     gpio_set_level(gpio->pin, level);
// }

// bool furi_hal_gpio_read(const GpioPin* gpio) {
//     return gpio_get_level(gpio->pin);
// }

// void furi_hal_gpio_toggle(const GpioPin* gpio) {
//     gpio_set_level(gpio->pin, !gpio_get_level(gpio->pin));
// }

// void furi_hal_gpio_register_interrupt(
//     const GpioPin* gpio,
//     const GpioInterruptEdge edge,
//     const GpioInterruptHandler handler,
//     void* context) {
//     gpio_interrupt[gpio->pin].handler = handler;
//     gpio_interrupt[gpio->pin].context = context;

//     gpio_set_intr_type( gpio->pin, edge == GpioInterruptEdgeFalling ? GPIO_INTR_NEGEDGE : GPIO_INTR_POSEDGE);
//     gpio_intr_enable( gpio->pin);
// }

// void furi_hal_gpio_unregister_interrupt(const GpioPin* gpio) {
//     gpio_interrupt[gpio->pin].handler = NULL;
//     gpio_interrupt[gpio->pin].context = NULL;

//     gpio_intr_disable( gpio->pin);
// }

// void furi_hal_gpio_interrupt_handler(const uint32_t gpio_pin) {
//     if(gpio_interrupt[gpio_pin].handler) {
//         gpio_interrupt[gpio_pin].handler(gpio_interrupt[gpio_pin].context);
//     }
// }

void furi_hal_gpio_add_int_callback(const GpioPin* gpio, GpioExtiCallback cb, void* ctx) {
    // furi_assert(gpio);
    // furi_assert(cb);

    // portMUX_TYPE portMux = portMUX_INITIALIZER_UNLOCKED;
    // Configure GPIO with interrupts disabled
    // portENTER_CRITICAL(&portMux);
    
    uint8_t pin_num = furi_hal_gpio_get_pin_num(gpio);

    // furi_check(gpio_interrupt[pin_num].callback == NULL);
    gpio_interrupt[pin_num].callback = cb;
    gpio_interrupt[pin_num].context = ctx;
    gpio_interrupt[pin_num].ready = true;

    // portEXIT_CRITICAL(&portMux);
}