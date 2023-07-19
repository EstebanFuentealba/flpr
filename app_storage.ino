#include "SD.h"
#include "furi_hal_resources.h"

void app_storage_setup() {
    pinMode(SD_CS_Pin, OUTPUT);
    delay(10);
    if (!SD.begin(SD_CS_Pin)){
        Serial.println(F("Failed to mount SD Card"));
    } else {
        Serial.println(F("Mounted SD Card"));
    }
}

void app_storage_loop() {
    
}