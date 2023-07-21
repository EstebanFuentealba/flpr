#include "flipper_format.h"
#include "furi.h"
#include "furi_hal.h"
#include "storage.h"
#include "dir_walk.h"
#include "SD.h"

#define MAX_NAME_LENGTH 255
#define HARDCODED_ANIMATION_NAME "L1_Tv_128x47"
#define ANIMATION_META_FILE "meta.txt"
#define ANIMATION_DIR "/dolphin"
#define ANIMATION_MANIFEST_FILE ANIMATION_DIR "/manifest.txt"


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

  //  ### [ESCRIBE ARCHIVO DEMO]

  const char* name = "L3_Furippa3_128x64";

  Serial.println("try storage_file_alloc");
  Storage* storage = furi_record_open(RECORD_STORAGE);
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


  /*
  FlipperFormat* file = flipper_format_file_alloc(storage);
  flipper_format_set_strict_mode(file, true);
  Serial.println("flipper_format_set_strict_mode");
  FuriString* read_string;
  read_string = furi_string_alloc();


  uint32_t u32value;
  flipper_format_file_open_existing(file, ANIMATION_MANIFEST_FILE);
  
  Serial.println("flipper_format_file_open_existing");

  if(flipper_format_read_header(file, read_string, &u32value)){
    if(!furi_string_cmp(read_string, furi_string_alloc_set_str("Flipper Animation Manifest"))) {
      flipper_format_set_strict_mode(file, false);
      while(flipper_format_read_string(file, "Name", read_string) &&
            furi_string_cmp(read_string, furi_string_alloc_set_str(name)))
          ;
    }
  } else {
    Serial.println("NO flipper_format_read_header");
  }

  

  
  
  
    Serial.print("name: ");
    Serial.println(furi_string_get_cstr(read_string));

   if(flipper_format_read_uint32(file, "Min butthurt", &u32value, 1)) {
    Serial.print("Min butthurt: ");
    Serial.println(u32value);
   }
    if(flipper_format_read_uint32(file, "Max butthurt", &u32value, 1)) {
    Serial.print("Max butthurt: ");
    Serial.println(u32value);
   }
     if(flipper_format_read_uint32(file, "Min level", &u32value, 1)) {
    Serial.print("Min level: ");
    Serial.println(u32value);
   }
   if(flipper_format_read_uint32(file, "Max level", &u32value, 1)) {
    Serial.print("Max level: ");
    Serial.println(u32value);
   }
   if(flipper_format_read_uint32(file, "Weight", &u32value, 1)) {
    Serial.print("Weight: ");
    Serial.println(u32value);
   }


  Serial.println("flipper_format_file_close");
  flipper_format_file_close(file);
  */


  // flipper_format_set_strict_mode(file, true);

  // manifest_info->name = malloc(furi_string_size(read_string) + 1);
  // strcpy((char*)manifest_info->name, furi_string_get_cstr(read_string));

  // if(!flipper_format_read_uint32(file, "Min butthurt", &u32value, 1)) break;
  // manifest_info->min_butthurt = u32value;
  // if(!flipper_format_read_uint32(file, "Max butthurt", &u32value, 1)) break;
  // manifest_info->max_butthurt = u32value;
  // if(!flipper_format_read_uint32(file, "Min level", &u32value, 1)) break;
  // manifest_info->min_level = u32value;
  // if(!flipper_format_read_uint32(file, "Max level", &u32value, 1)) break;
  // manifest_info->max_level = u32value;
  // if(!flipper_format_read_uint32(file, "Weight", &u32value, 1)) break;
  // manifest_info->weight = u32value;


    
  // storage_dir_close(file);
  // storage_file_free(file);
  furi_record_close(RECORD_STORAGE);

    
    
}
void loop() {
  app_storage_loop();
  app_input_loop();
  app_gui_loop();
}

