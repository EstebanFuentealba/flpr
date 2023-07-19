#include "furi.h"
#include "furi_hal.h"
#include "storage.h"
#include "dir_walk.h"
#include "SD.h"

#define MAX_NAME_LENGTH 255

void setup() {
  Serial.begin(115200);
  Serial.println("setup");


  // Initialize FURI layer
  furi_init();
  // // Flipper FURI HAL
  furi_hal_init();
  FuriPubSub* pubsub = furi_pubsub_alloc();
  app_storage_setup();
  app_input_setup();
  app_gui_setup();

  //  Run Kernel
  furi_run();

}
void loop() {
  app_storage_loop();
  app_input_loop();
  app_gui_loop();
}

