#include "furi_hal_resources.h"
// #include <furi_hal_bus.h>
#include "furi.h"

#include <Arduino.h>


#define TAG "FuriHalResources"


// #define BUTTON_UP_PIN  25
// #define BUTTON_DOWN_PIN  32
// #define BUTTON_LEFT_PIN  16
// #define BUTTON_RIGHT_PIN  26
// #define BUTTON_OK_PIN  27
// #define BUTTON_BACK_PIN  33



// const int pin_swdio = 13;
// const int pin_swclk = 14;

// const int pin_vibro = 8;
// const int pin_ibutton = 14;

// const int pin_display_cs = 11;
// const int pin_display_rst_n = 0;
// const int pin_display_di = 1;
// const int pin_sdcard_cs = 12;
// const int pin_sdcard_cd = 10;

// const int pin_button_up = 27;
const GpioPin gpio_button_down = {.pin = 32};
const GpioPin gpio_button_up = {.pin = 25};
const GpioPin gpio_button_left = {.pin = 16};
const GpioPin gpio_button_right = {.pin = 26};
const GpioPin gpio_button_ok = {.pin = 27};
const GpioPin gpio_button_back = {.pin = 33};
// const int pin_button_down = 27;
// const int pin_button_right = 12;
// const int pin_button_left = 11;
// const int pin_button_ok = 3;
// const int pin_button_back = 13;

// const int pin_spi_d_miso = 2;
// const int pin_spi_d_mosi = 15;
// const int pin_spi_d_sck = 1;

// const int pin_ext_pc0 = 0;
// const int pin_ext_pc1 = 1;
// const int pin_ext_pc3 = 3;
// const int pin_ext_pb2 = 2;
// const int pin_ext_pb3 = 3;
// const int pin_ext_pa4 = 4;
// const int pin_ext_pa6 = 6;
// const int pin_ext_pa7 = 7;

// const int pin_ext_pc5 = 5;
// const int pin_ext_pc4 = 4;
// const int pin_ext_pa5 = 5;
// const int pin_ext_pb9 = 9;
// const int pin_ext_pa0 = 0;
// const int pin_ext_pa1 = 1;
// const int pin_ext_pa15 = 15;
// const int pin_ext_pe4 = 4;
// const int pin_ext_pa2 = 2;
// const int pin_ext_pb4 = 4;
// const int pin_ext_pb5 = 5;
// const int pin_ext_pd0 = 0;
// const int pin_ext_pb13 = 13;

// const int pin_usart_tx = 6;
// const int pin_usart_rx = 7;

// const int pin_i2c_power_sda = 10;
// const int pin_i2c_power_scl = 9;

// const int pin_speaker = 8;

// const int pin_periph_power = 3;

// const int pin_usb_dm = 11;
// const int pin_usb_dp = 12;

// const int pin_debug_button = 4;


typedef size_t GPIO_TypeDef;

//  TODO: Implement
const InputPin input_pins[] = {
    {.gpio = &gpio_button_up, .key = InputKeyUp, .inverted = true, .name = "Up"},
    {.gpio = &gpio_button_down, .key = InputKeyDown, .inverted = true, .name = "Down"},
    {.gpio = &gpio_button_right, .key = InputKeyRight, .inverted = true, .name = "Right"},
    {.gpio = &gpio_button_left, .key = InputKeyLeft, .inverted = true, .name = "Left"},
    {.gpio = &gpio_button_ok, .key = InputKeyOk, .inverted = false, .name = "OK"},
    {.gpio = &gpio_button_back, .key = InputKeyBack, .inverted = true, .name = "Back"},
};

const size_t gpio_pins_count = COUNT_OF(input_pins);
const size_t input_pins_count = sizeof(input_pins) / sizeof(InputPin);


static void furi_hal_resources_init_input_pins() {
    for(size_t i = 0; i < input_pins_count; i++) {
        pinMode(input_pins[i].gpio->pin, INPUT_PULLUP);
    }
}

void furi_hal_resources_init_early() {
    //  TODO:
}

void furi_hal_resources_deinit_early() {
    //  TODO:
}

void furi_hal_resources_init() {
    furi_hal_resources_init_input_pins();
    // SD Card stepdown control

    // Display pins

    // Alternative pull configuration for shutdown

    // Hard reset USB

    // External header pins


    Serial.println("[furi_hal_resources] furi_hal_resources_init");
}

/**
 * Get a corresponding external connector pin number for a gpio
 * @param gpio GpioPin
 * @return pin number or -1 if gpio is not on the external connector
 */
int32_t furi_hal_resources_get_ext_pin_number(const uint8_t gpio) {
    //  TODO: implements external pins
    return -1;
}
