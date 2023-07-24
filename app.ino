// #include "applications.h"
// #include "flipper_format.h"
// #include "furi.h"
// #include "furi_hal.h"
// #include "storage.h"
// #include "dir_walk.h"
// #include "SD.h"
// #define MAX_NAME_LENGTH 255
// #define HARDCODED_ANIMATION_NAME "L1_Tv_128x47"
// #define ANIMATION_META_FILE "meta.txt"
// #define ANIMATION_DIR "/dolphin"
// #define ANIMATION_MANIFEST_FILE ANIMATION_DIR "/manifest.txt"


void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  /*
  // Initialize FURI layer
  furi_init();
  // // Flipper FURI HAL
  furi_hal_init();
  FuriPubSub* pubsub = furi_pubsub_alloc();
  
  // app_storage_setup();
  // app_input_setup();
  // app_gui_setup();
  // app_desktop_setup();

  Serial.println("Boot mode Normal, starting services");

  // for(size_t i = 0; i < FLIPPER_SERVICES_COUNT; i++) {
  //     Serial.printf("Starting service %s", FLIPPER_SERVICES[i].name);

  //     FuriThread* thread = furi_thread_alloc_ex(
  //         FLIPPER_SERVICES[i].name,
  //         FLIPPER_SERVICES[i].stack_size,
  //         FLIPPER_SERVICES[i].app,
  //         NULL);
  //     furi_thread_mark_as_service(thread);
  //     furi_thread_set_appid(thread, FLIPPER_SERVICES[i].appid);

  //     furi_thread_start(thread);
  // }
  Serial.println("Startup complete");
  
  //  Run Kernel
  furi_run();
  */


  // Serial.println("try furi_record_open");
  // Storage* api = furi_record_open(RECORD_STORAGE);
  // Serial.println("furi_record_open");
  // pinMode(5, OUTPUT);
  // delay(10);
  // if (!SD.begin(5)){
  //     Serial.println(F("Failed to mount SD Card"));
  // } else {
  //     Serial.println(F("Mounted SD Card"));
  // }
   /*
  //  ### [ESCRIBE ARCHIVO DEMO]
 
  const char* name = "L3_Furippa3_128x64";

  Serial.println("try storage_file_alloc");
  Storage* storage = (Storage*)furi_record_open(RECORD_STORAGE);
  bool isCreated = storage_simply_mkdir(storage, "/dolphin");
  Serial.println("isCreated:");
  Serial.println(isCreated);
  FZFile* ff = storage_file_alloc(storage);
  bool result = storage_file_open(ff, ANIMATION_MANIFEST_FILE, FILE_WRITE);
  const char* manifest = "Filetype: Flipper Animation Manifest\nVersion: 1\n\nName: L3_Furippa3_128x64\nMin butthurt: 0\nMax butthurt: 6\nMin level: 3\nMax level: 3\nWeight: 3\n";
  storage_file_write(ff, manifest, strlen(manifest));
  Serial.println("storage_file_open [W]");
  Serial.println(result);

  storage_file_close(ff);
  //  ### [/ESCRIBE ARCHIVO DEMO]

  //  ### [LEE ARCHIVO DEMO]
  ff = storage_file_alloc(storage);
  result = storage_file_open(ff, ANIMATION_MANIFEST_FILE, FILE_READ);
  uint8_t buffer[128];
  uint16_t bytesRead;
   while (!storage_file_eof(ff)) {
    bytesRead =  storage_file_read(ff, buffer, sizeof(buffer));
    if (bytesRead > 0) {
      for (uint16_t i = 0; i < bytesRead; i++) {
        Serial.write(buffer[i]);
      }
    }
  }
  storage_dir_close(ff);
  storage_file_free(ff);
  //  ### [/LEE ARCHIVO DEMO]


  //  ### [LISTA DIRECTORIOS]
  FZFile* dir = storage_file_alloc(storage);
  if(storage_dir_open(dir, "/" )) {
    Serial.println("storage_dir_open");
      FileInfo fileinfo;
      char name[MAX_NAME_LENGTH];
      bool read_done = false;

      while(storage_dir_read(dir, &fileinfo, name, MAX_NAME_LENGTH)) {
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
     Serial.println(storage_file_get_error(ff));
  }

  storage_dir_close(dir);
  storage_file_free(dir);
  //  ### [/LISTA DIRECTORIOS]
  furi_record_close(RECORD_STORAGE);

    */
    
}
void loop() {

  // app_storage_loop();
  // app_input_loop();
  // app_gui_loop();
  // app_desktop_loop();
}

