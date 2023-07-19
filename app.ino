#include "furi.h"
#include "furi_hal.h"

void setup() {
  Serial.begin(115200);
  Serial.println("setup");


  // Initialize FURI layer
  furi_init();
  // // Flipper FURI HAL
  furi_hal_init();
  FuriPubSub* pubsub = furi_pubsub_alloc();
  app_input_setup();
  app_gui_setup();

  //  Run Kernel
  furi_run();

  Serial.println("RUN");
  

}
void loop() {
  app_input_loop();
  app_gui_loop();
}

