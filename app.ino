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

  Serial.println("RUN");

  Serial.println("VSPI");
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);
  Serial.print("HSPI: ");
  Serial.println(HSPI);
  Serial.print("VSPI: ");
  Serial.println(VSPI);



  Serial.println("try furi_record_open");
  Storage* api = furi_record_open(RECORD_STORAGE);
  Serial.println("furi_record_open");
  // pinMode(5, OUTPUT);
  // delay(10);
  // if (!SD.begin(5)){
  //     Serial.println(F("Failed to mount SD Card"));
  // } else {
  //     Serial.println(F("Mounted SD Card"));
  // }
  Serial.println("try storage_file_alloc");
  FZFile* file = storage_file_alloc(api);
Serial.println("storage_file_alloc");
  if(storage_dir_open(file, "/" )) {
    Serial.println("storage_dir_open");
      FileInfo fileinfo;
      char name[MAX_NAME_LENGTH];
      bool read_done = false;

      while(storage_dir_read(file, &fileinfo, name, MAX_NAME_LENGTH)) {
          read_done = true;
          if(file_info_is_dir(&fileinfo)) {
              Serial.print("\t[D] ");
              Serial.println(name);
          } else {
              Serial.print("\t[F] ");
              Serial.print(name);
              Serial.print(" ");
              Serial.println((uint32_t)(fileinfo.size));
          }
      }

      if(!read_done) {
          Serial.printf("\tEmpty\r\n");
      }
  } else {
     Serial.println("storage_file_get_error");
     Serial.println(storage_file_get_error(file));
  }

  storage_dir_close(file);
  storage_file_free(file);
  furi_record_close(RECORD_STORAGE);

  

}
void loop() {
  app_storage_loop();
  app_input_loop();
  app_gui_loop();
}

