#pragma once
#include "furi.h"
#include <Arduino.h>
#include <stdint.h>
#include <cstddef>

/* Input Related Constants */
#define INPUT_DEBOUNCE_TICKS 4

/* Input Keys */
typedef enum {
    InputKeyUp,
    InputKeyDown,
    InputKeyRight,
    InputKeyLeft,
    InputKeyOk,
    InputKeyBack,
    InputKeyMAX, /**< Special value */
} InputKey;

/* Light */
typedef enum {
    LightRed = (1 << 0),
    LightGreen = (1 << 1),
    LightBlue = (1 << 2),
    LightBacklight = (1 << 3),
} Light;

typedef struct {
    const GpioPin* gpio;
    const InputKey key;
    const bool inverted;
    const char* name;
} InputPin;

typedef struct {
    const uint8_t pin;
    const char* name;
    const bool debug;
} GpioPinRecord;

extern const InputPin input_pins[];
extern const size_t input_pins_count;

extern const GpioPinRecord gpio_pins[];
extern const size_t gpio_pins_count;

extern const uint8_t gpio_swdio;
extern const uint8_t gpio_swclk;

extern const uint8_t gpio_vibro;
extern const uint8_t gpio_ibutton;

extern const uint8_t gpio_cc1101_g0;
extern const uint8_t gpio_rf_sw_0;

extern const uint8_t gpio_subghz_cs;
extern const uint8_t gpio_display_cs;
extern const uint8_t gpio_display_rst_n;
extern const uint8_t gpio_display_di;
extern const uint8_t gpio_sdcard_cs;
extern const uint8_t gpio_sdcard_cd;
extern const uint8_t gpio_nfc_cs;

extern const GpioPin gpio_button_up;
extern const GpioPin gpio_button_down;
extern const GpioPin gpio_button_right;
extern const GpioPin gpio_button_left;
extern const GpioPin gpio_button_ok;
extern const GpioPin gpio_button_back;

extern const uint8_t gpio_spi_d_miso;
extern const uint8_t gpio_spi_d_mosi;
extern const uint8_t gpio_spi_d_sck;
extern const uint8_t gpio_spi_r_miso;
extern const uint8_t gpio_spi_r_mosi;
extern const uint8_t gpio_spi_r_sck;

extern const uint8_t gpio_ext_pc0;
extern const uint8_t gpio_ext_pc1;
extern const uint8_t gpio_ext_pc3;
extern const uint8_t gpio_ext_pb2;
extern const uint8_t gpio_ext_pb3;
extern const uint8_t gpio_ext_pa4;
extern const uint8_t gpio_ext_pa6;
extern const uint8_t gpio_ext_pa7;

extern const uint8_t gpio_nfc_irq_rfid_pull;
extern const uint8_t gpio_rfid_carrier_out;
extern const uint8_t gpio_rfid_data_in;
extern const uint8_t gpio_rfid_carrier;

extern const uint8_t gpio_infrared_rx;
extern const uint8_t gpio_infrared_tx;

extern const uint8_t gpio_usart_tx;
extern const uint8_t gpio_usart_rx;
extern const uint8_t gpio_i2c_power_sda;
extern const uint8_t gpio_i2c_power_scl;

extern const uint8_t gpio_speaker;

extern const uint8_t gpio_periph_power;

extern const uint8_t gpio_usb_dm;
extern const uint8_t gpio_usb_dp;


#define BUTTON_BACK_GPIO_Port GPIOC
#define BUTTON_BACK_Pin 33
#define BUTTON_DOWN_GPIO_Port GPIOC
#define BUTTON_DOWN_Pin 32
#define BUTTON_LEFT_GPIO_Port GPIOB
#define BUTTON_LEFT_Pin 16
#define BUTTON_OK_GPIO_Port GPIOH
#define BUTTON_OK_Pin 27
#define BUTTON_RIGHT_GPIO_Port GPIOB
#define BUTTON_RIGHT_Pin 26
#define BUTTON_UP_GPIO_Port GPIOB
#define BUTTON_UP_Pin 25

#define CC1101_CS_GPIO_Port GPIOD
#define CC1101_CS_Pin 15
// #define CC1101_G0_GPIO_Port GPIOA
// #define CC1101_G0_Pin 1

// #define DISPLAY_CS_GPIO_Port GPIOC
// #define DISPLAY_CS_Pin 5
// #define DISPLAY_DI_GPIO_Port GPIOB
// #define DISPLAY_DI_Pin 19
// #define DISPLAY_RST_GPIO_Port GPIOB
// #define DISPLAY_RST_Pin 0       //  TODO: Implementar RESET

#define IR_RX_GPIO_Port GPIOA
#define IR_RX_Pin 17    //  IRRECEIVER
#define IR_TX_GPIO_Port GPIOB
#define IR_TX_Pin 4     //  IRTRANSMITTER

// #define NFC_CS_GPIO_Port GPIOE
// #define NFC_CS_Pin 4

// #define PA4_GPIO_Port GPIOA
// #define PA4_Pin 4
// #define PA6_GPIO_Port GPIOA
// #define PA6_Pin 6
// #define PA7_GPIO_Port GPIOA
// #define PA7_Pin 7
// #define PB2_GPIO_Port GPIOB
// #define PB2_Pin 2
// #define PB3_GPIO_Port GPIOB
// #define PB3_Pin 3
// #define PC0_GPIO_Port GPIOC
// #define PC0_Pin 0
// #define PC1_GPIO_Port GPIOC
// #define PC1_Pin 1
// #define PC3_GPIO_Port GPIOC
// #define PC3_Pin 3

// #define QUARTZ_32MHZ_IN_GPIO_Port GPIOC
// #define QUARTZ_32MHZ_IN_Pin 14
// #define QUARTZ_32MHZ_OUT_GPIO_Port GPIOC
// #define QUARTZ_32MHZ_OUT_Pin 15

// #define RFID_OUT_GPIO_Port GPIOB
// #define RFID_OUT_Pin 13
// #define RFID_PULL_GPIO_Port GPIOA
// #define RFID_PULL_Pin 2
// #define RFID_RF_IN_GPIO_Port GPIOC
// #define RFID_RF_IN_Pin 5
// #define RFID_CARRIER_GPIO_Port GPIOA
// #define RFID_CARRIER_Pin 15

// #define RF_SW_0_GPIO_Port GPIOC
// #define RF_SW_0_Pin 4

// #define SD_CD_GPIO_Port GPIOC
// #define SD_CD_Pin 10
#define SD_CS_GPIO_Port GPIOC
#define SD_CS_Pin 5

// #define SPEAKER_GPIO_Port GPIOB
// #define SPEAKER_Pin 8

// #define VIBRO_GPIO_Port GPIOA
// #define VIBRO_Pin 8

// #define iBTN_GPIO_Port GPIOB
// #define iBTN_Pin 14

// #define USART1_TX_Pin 6
// #define USART1_TX_Port GPIOB
// #define USART1_RX_Pin 7
// #define USART1_RX_Port GPIOB

// #define SPI_D_MISO_GPIO_Port GPIOC
// #define SPI_D_MISO_Pin 2
// #define SPI_D_MOSI_GPIO_Port GPIOB
// #define SPI_D_MOSI_Pin 15
// #define SPI_D_SCK_GPIO_Port GPIOD
// #define SPI_D_SCK_Pin 1

// #define SPI_R_MISO_GPIO_Port GPIOB
// #define SPI_R_MISO_Pin 4
// #define SPI_R_MOSI_GPIO_Port GPIOB
// #define SPI_R_MOSI_Pin 5
// #define SPI_R_SCK_GPIO_Port GPIOA
// #define SPI_R_SCK_Pin 5

// #define NFC_IRQ_Pin RFID_PULL_Pin
// #define NFC_IRQ_GPIO_Port RFID_PULL_GPIO_Port

void furi_hal_resources_init_early();

void furi_hal_resources_deinit_early();

void furi_hal_resources_init();

/**
 * Get a corresponding external connector pin number for a gpio
 * @param gpio GpioPin
 * @return pin number or -1 if gpio is not on the external connector
 */
int32_t furi_hal_resources_get_ext_pin_number(const uint8_t gpio);
