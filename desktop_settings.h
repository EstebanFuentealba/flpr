#pragma once

#include "desktop_settings_filename.h"

#include "furi_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "saved_struct.h"
#include "storage.h"

#define DESKTOP_SETTINGS_VER (8)

#define DESKTOP_SETTINGS_PATH INT_PATH(DESKTOP_SETTINGS_FILE_NAME)
#define DESKTOP_SETTINGS_MAGIC (0x17)
#define PIN_MAX_LENGTH 12

#define DESKTOP_SETTINGS_RUN_PIN_SETUP_ARG "run_pin_setup"

#define DESKTOP_SETTINGS_SAVE(x) \
    saved_struct_save(           \
        DESKTOP_SETTINGS_PATH,   \
        (x),                     \
        sizeof(DesktopSettings), \
        DESKTOP_SETTINGS_MAGIC,  \
        DESKTOP_SETTINGS_VER)

#define DESKTOP_SETTINGS_LOAD(x) \
    saved_struct_load(           \
        DESKTOP_SETTINGS_PATH,   \
        (x),                     \
        sizeof(DesktopSettings), \
        DESKTOP_SETTINGS_MAGIC,  \
        DESKTOP_SETTINGS_VER)

#define MAX_PIN_SIZE 10
#define MIN_PIN_SIZE 4
#define MAX_APP_LENGTH 128

typedef struct {
    InputKey data[MAX_PIN_SIZE];
    uint8_t length;
} PinCode;

typedef struct {
    char name_or_path[MAX_APP_LENGTH];
} FavoriteApp;

typedef struct {
    FavoriteApp favorite_primary;
    FavoriteApp favorite_secondary;
    PinCode pin_code;
    uint32_t auto_lock_delay_ms;
    uint8_t dummy_mode;
} DesktopSettings;