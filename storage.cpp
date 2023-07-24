#include "storage.h"
#include "storage_i.h"
#include "storage_message.h"
// #include "storage_processing.h"
#include "furi.h"
#include "storage_glue.h"
#include "storage_ext.h"
#include "icon_i.h"
#include "assets_icons.h"

#define STORAGE_TICK 1000


// 'SDcardMounted_11x8', 11x8px
// const unsigned char SDcardMounted_11x8_bitmap_SDcardMounted_11x8 [] PROGMEM = {
// 	0xff, 0xe0, 0xff, 0x20, 0xff, 0xe0, 0xff, 0x20, 0xff, 0xe0, 0xff, 0x20, 0xff, 0xe0, 0xe6, 0x00
// };
// const uint8_t* const frame_SDcardMounted_11x8_bitmap_SDcardMounted_11x8[] = { SDcardMounted_11x8_bitmap_SDcardMounted_11x8 };

// Icon I_SDcardMounted_11x8 = {
//     .width = 11,
//     .height = 8,
//     .frame_count = 1,
//     .frame_rate = 1,
//     .frames = frame_SDcardMounted_11x8_bitmap_SDcardMounted_11x8
// };

// // 'SDcardFail_11x8', 11x8px
// const unsigned char SDcardFail_11x8_bitmap_SDcardFail_11x8 [] PROGMEM = {
// 	0xff, 0xe0, 0xed, 0xe0, 0xff, 0xe0, 0xe1, 0xe0, 0xde, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xe6, 0x00
// };
// const uint8_t* const frame_SDcardFail_11x8_bitmap_SDcardFail_11x8[] = { SDcardFail_11x8_bitmap_SDcardFail_11x8 };

// Icon I_SDcardFail_11x8 = {
//     .width = 11,
//     .height = 8,
//     .frame_count = 1,
//     .frame_rate = 1,
//     .frames = frame_SDcardFail_11x8_bitmap_SDcardFail_11x8
// };

#define ICON_SD_MOUNTED &I_SDcardMounted_11x8
#define ICON_SD_ERROR &I_SDcardFail_11x8

#define TAG RECORD_STORAGE

static void storage_app_sd_icon_draw_callback(Canvas* canvas, void* context) {
    // furi_assert(canvas);
    // furi_assert(context);
    Storage* app = ( Storage*)context;

    // here we don't care about thread race when reading / writing status
    switch(app->storage.status) {
    case StorageStatusNotReady:
        break;
    case StorageStatusOK:
        canvas_draw_icon(canvas, 0, 0, ICON_SD_MOUNTED);
        break;
    default:
        canvas_draw_icon(canvas, 0, 0, ICON_SD_ERROR);
        break;
    }
}

Storage* storage_app_alloc() {
    Storage* app = (Storage*)malloc(sizeof(Storage));
    app->message_queue = furi_message_queue_alloc(8, sizeof(StorageMessage));
    app->pubsub = furi_pubsub_alloc();

    for(uint8_t i = 0; i < STORAGE_COUNT; i++) {
        storage_data_init(&app->storage);
        storage_data_timestamp(&app->storage);
    }

    storage_ext_init(&app->storage);

    // sd icon gui
    app->sd_gui.enabled = false;
    app->sd_gui.view_port = view_port_alloc();
    view_port_set_width(app->sd_gui.view_port, icon_get_width(ICON_SD_MOUNTED));
    view_port_draw_callback_set(app->sd_gui.view_port, storage_app_sd_icon_draw_callback, app);
    view_port_enabled_set(app->sd_gui.view_port, false);

    Gui* gui = (Gui*)furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, app->sd_gui.view_port, GuiLayerStatusBarLeft);
    furi_record_close(RECORD_GUI);

    return app;
}

void storage_tick(Storage* app) {
    StorageApi api = app->storage.api;
    if(api.tick != NULL) {
        api.tick(&app->storage);
    }

    // storage not enabled but was enabled (sd card unmount)
    if(app->storage.status == StorageStatusNotReady && app->sd_gui.enabled == true) {
        app->sd_gui.enabled = false;
        view_port_enabled_set(app->sd_gui.view_port, false);

        // FURI_LOG_I(TAG, "SD card unmount");
        StorageEvent event = {.type = StorageEventTypeCardUnmount};
        furi_pubsub_publish(app->pubsub, &event);
    }

    // storage enabled (or in error state) but was not enabled (sd card mount)
    if((app->storage.status == StorageStatusOK ||
        app->storage.status == StorageStatusNotMounted ||
        app->storage.status == StorageStatusNoFS ||
        app->storage.status == StorageStatusNotAccessible ||
        app->storage.status == StorageStatusErrorInternal) &&
       app->sd_gui.enabled == false) {
        app->sd_gui.enabled = true;
        view_port_enabled_set(app->sd_gui.view_port, true);

        if(app->storage.status == StorageStatusOK) {
            // FURI_LOG_I(TAG, "SD card mount");
            StorageEvent event = {.type = StorageEventTypeCardMount};
            furi_pubsub_publish(app->pubsub, &event);
        } else {
            // FURI_LOG_I(TAG, "SD card mount error");
            StorageEvent event = {.type = StorageEventTypeCardMountError};
            furi_pubsub_publish(app->pubsub, &event);
        }
    }
}

// int32_t storage_srv(void* p) {
//     UNUSED(p);
//     Storage* app = storage_app_alloc();
//     furi_record_create(RECORD_STORAGE, app);

//     StorageMessage message;
//     while(1) {
//         if(furi_message_queue_get(app->message_queue, &message, STORAGE_TICK) == FuriStatusOk) {
//             storage_process_message(app, &message);
//         } else {
//             storage_tick(app);
//         }
//     }

//     return 0;
// }


/*
#include "storage.h"
#include <FS.h>
#include <SD.h>

FZFile* storage_file_alloc(Storage* storage) {
  FZFile* file = new File();
  return file;
}

void storage_file_free(FZFile* file) {
  if (file->isFile()) {
    file->close();
  }
  delete file;
}

bool storage_file_open(
    FZFile* file,
    const char* path,
    FS_AccessMode access_mode,
    FS_OpenMode open_mode) {
  if (!file) {
    return false;
  }

  fs::File fsFile;
  switch (access_mode) {
    case FS_READ:
      fsFile = SPIFFS.open(path, "r");
      break;
    case FS_WRITE:
      fsFile = SPIFFS.open(path, "w");
      break;
    case FS_APPEND:
      fsFile = SPIFFS.open(path, "a");
      break;
    default:
      return false;
  }

  if (!fsFile) {
    return false;
  }

  file->fsFile = fsFile;
  return true;
}

bool storage_file_close(FZFile* file) {
  if (!file) {
    return false;
  }

  if (!file->fsFile) {
    return false;
  }

  file->fsFile.close();
  return true;
}

bool storage_file_is_open(FZFile* file) {
  if (!file) {
    return false;
  }

  return file->fsFile && file->fsFile.isOpen();
}

bool storage_file_is_dir(FZFile* file) {
  if (!file) {
    return false;
  }

  return file->fsFile && file->fsFile.isDirectory();
}

uint16_t storage_file_read(FZFile* file, void* buff, uint16_t bytes_to_read) {
  if (!file || !file->fsFile) {
    return 0;
  }

  return file->fsFile.readBytes(static_cast<uint8_t*>(buff), bytes_to_read);
}

uint16_t storage_file_write(FZFile* file, const void* buff, uint16_t bytes_to_write) {
  if (!file || !file->fsFile) {
    return 0;
  }

  return file->fsFile.write(static_cast<const uint8_t*>(buff), bytes_to_write);
}

bool storage_file_seek(FZFile* file, uint32_t offset, bool from_start) {
  if (!file || !file->fsFile) {
    return false;
  }

  if (from_start) {
    return file->fsFile.seek(offset);
  } else {
    return file->fsFile.seek(offset, SeekCur);
  }
}

uint64_t storage_file_tell(FZFile* file) {
  if (!file || !file->fsFile) {
    return 0;
  }

  return file->fsFile.position();
}

bool storage_file_truncate(FZFile* file) {
  if (!file || !file->fsFile) {
    return false;
  }

  return file->fsFile.truncate();
}

uint64_t storage_file_size(FZFile* file) {
  if (!file || !file->fsFile) {
    return 0;
  }

  return file->fsFile.size();
}

bool storage_file_sync(FZFile* file) {
  if (!file || !file->fsFile) {
    return false;
  }

  return file->fsFile.flush();
}

bool storage_file_eof(FZFile* file) {
  if (!file || !file->fsFile) {
    return false;
  }

  return file->fsFile.available() == 0;
}

bool storage_file_exists(Storage* storage, const char* path) {
  return SPIFFS.exists(path);
}

bool storage_file_copy_to_file(FZFile* source, FZFile* destination, uint32_t size) {
  if (!source || !source->fsFile || !destination || !destination->fsFile) {
    return false;
  }

  uint8_t buffer[256];
  uint32_t remaining = size;

  while (remaining > 0) {
    uint32_t bytes_to_copy = min(remaining, sizeof(buffer));
    uint32_t bytes_read = source->fsFile.read(buffer, bytes_to_copy);
    if (bytes_read == 0) {
      return false;
    }
    uint32_t bytes_written = destination->fsFile.write(buffer, bytes_read);
    if (bytes_written != bytes_read) {
      return false;
    }
    remaining -= bytes_read;
  }

  return true;
}

bool storage_dir_open(FZFile* file, const char* path) {
  if (!file) {
    return false;
  }

  fs::Dir dir = SPIFFS.openDir(path);
  file->fsDir = dir;
  return true;
}

bool storage_dir_close(FZFile* file) {
  if (!file) {
    return false;
  }

  if (!file->fsDir) {
    return false;
  }

  file->fsDir.close();
  return true;
}

bool storage_dir_read(FZFile* file, FileInfo* fileinfo, char* name, uint16_t name_length) {
  if (!file || !file->fsDir) {
    return false;
  }

  fs::File entry = file->fsDir.openNextFile();
  if (!entry) {
    return false;
  }

  if (fileinfo) {
    fileinfo->is_dir = entry.isDirectory();
    fileinfo->size = entry.size();
  }

  if (name && name_length > 0) {
    entry.getName(name, name_length);
  }

  entry.close();
  return true;
}

bool storage_dir_rewind(FZFile* file) {
  if (!file || !file->fsDir) {
    return false;
  }

  file->fsDir.rewind();
  return true;
}

bool storage_dir_exists(Storage* storage, const char* path) {
  fs::File entry = SPIFFS.open(path);
  bool exists = entry && entry.isDirectory();
  entry.close();
  return exists;
}

bool storage_common_exists(Storage* storage, const char* path) {
  fs::File entry = SPIFFS.open(path);
  bool exists = entry;
  entry.close();
  return exists;
}

const char* storage_error_get_desc(FS_Error error_id) {
  switch (error_id) {
    case FS_OK:
      return "No error";
    case FS_FILE_NOT_FOUND:
      return "File not found";
    case FS_PATH_TOO_LONG:
      return "Path too long";
    case FS_DIR_NOT_FOUND:
      return "Directory not found";
    case FS_ACCESS_DENIED:
      return "Access denied";
    case FS_ALREADY_EXISTS:
      return "File or directory already exists";
    case FS_INSUFFICIENT_SPACE:
      return "Insufficient space";
    case FS_NOT_SUPPORTED:
      return "Operation not supported";
    default:
      return "Unknown error";
  }
}

FS_Error storage_file_get_error(FZFile* file) {
  if (!file || !file->fsFile) {
    return FS_ERROR;
  }

  return file->fsFile.getLastError();
}

int32_t storage_file_get_internal_error(FZFile* file) {
  if (!file || !file->fsFile) {
    return -1;
  }

  return file->fsFile.getLastInternalError();
}

const char* storage_file_get_error_desc(FZFile* file) {
  if (!file || !file->fsFile) {
    return "Unknown error";
  }

  return file->fsFile.errorString();
}

FS_Error storage_common_stat(Storage* storage, const char* path, FileInfo* fileinfo) {
  if (!storage || !fileinfo) {
    return FS_ERROR;
  }

  fs::File entry = SPIFFS.open(path);
  if (!entry) {
    return FS_FILE_NOT_FOUND;
  }

  fileinfo->is_dir = entry.isDirectory();
  fileinfo->size = entry.size();

  entry.close();
  return FS_OK;
}

FS_Error storage_common_remove(Storage* storage, const char* path) {
  if (!storage) {
    return FS_ERROR;
  }

  bool success = SPIFFS.remove(path);
  if (success) {
    return FS_OK;
  } else {
    return FS_FILE_NOT_FOUND;
  }
}

FS_Error storage_common_rename(Storage* storage, const char* old_path, const char* new_path) {
  if (!storage) {
    return FS_ERROR;
  }

  bool success = SPIFFS.rename(old_path, new_path);
  if (success) {
    return FS_OK;
  } else {
    return FS_FILE_NOT_FOUND;
  }
}

FS_Error storage_common_copy(Storage* storage, const char* old_path, const char* new_path) {
  if (!storage) {
    return FS_ERROR;
  }

  fs::File srcFile = SPIFFS.open(old_path, "r");
  if (!srcFile) {
    return FS_FILE_NOT_FOUND;
  }

  fs::File dstFile = SPIFFS.open(new_path, "w");
  if (!dstFile) {
    srcFile.close();
    return FS_ACCESS_DENIED;
  }

  while (srcFile.available()) {
    uint8_t buffer[128];
    size_t bytesRead = srcFile.readBytes(reinterpret_cast<char*>(buffer), sizeof(buffer));
    dstFile.write(buffer, bytesRead);
  }

  srcFile.close();
  dstFile.close();

  return FS_OK;
}

FS_Error storage_common_merge(Storage* storage, const char* old_path, const char* new_path) {
  if (!storage) {
    return FS_ERROR;
  }

  fs::Dir dir = SPIFFS.openDir(old_path);
  while (dir.next()) {
    fs::File entry = dir.openFile("r");
    if (entry.isDirectory()) {
      String subDirPath = String(new_path) + "/" + entry.name();
      SPIFFS.mkdir(subDirPath);
      storage_common_merge(storage, entry.name(), subDirPath.c_str());
    } else {
      String dstFilePath = String(new_path) + "/" + entry.name();
      fs::File dstFile = SPIFFS.open(dstFilePath, "w");
      if (!dstFile) {
        entry.close();
        return FS_ACCESS_DENIED;
      }

      while (entry.available()) {
        uint8_t buffer[128];
        size_t bytesRead = entry.readBytes(reinterpret_cast<char*>(buffer), sizeof(buffer));
        dstFile.write(buffer, bytesRead);
      }

      entry.close();
      dstFile.close();
    }
  }

  dir.close();
  SPIFFS.rmdir(old_path);

  return FS_OK;
}

FS_Error storage_common_mkdir(Storage* storage, const char* path) {
  if (!storage) {
    return FS_ERROR;
  }

  bool success = SPIFFS.mkdir(path);
  if (success) {
    return FS_OK;
  } else {
    return FS_NOT_SUPPORTED;
  }
}

FS_Error storage_common_fs_info(
    Storage* storage,
    const char* fs_path,
    uint64_t* total_space,
    uint64_t* free_space) {
  if (!storage || !total_space || !free_space) {
    return FS_ERROR;
  }

  FSInfo fs_info;
  SPIFFS.info(fs_info);

  *total_space = fs_info.totalBytes;
  *free_space = fs_info.totalBytes - fs_info.usedBytes;

  return FS_OK;
}

void storage_common_resolve_path_and_ensure_app_directory(Storage* storage, FuriString* path) {
  if (!storage || !path) {
    return;
  }

  // Resolve aliases in path
  if (path->startsWith(STORAGE_INT_PATH_PREFIX)) {
    path->replace(STORAGE_INT_PATH_PREFIX, "/");
  } else if (path->startsWith(STORAGE_EXT_PATH_PREFIX)) {
    path->replace(STORAGE_EXT_PATH_PREFIX, "/sd");
  } else if (path->startsWith(STORAGE_ANY_PATH_PREFIX)) {
    path->replace(STORAGE_ANY_PATH_PREFIX, "/");
  } else if (path->startsWith(STORAGE_APP_DATA_PATH_PREFIX)) {
    path->replace(STORAGE_APP_DATA_PATH_PREFIX, "/data");
  } else if (path->startsWith(STORAGE_APP_ASSETS_PATH_PREFIX)) {
    path->replace(STORAGE_APP_ASSETS_PATH_PREFIX, "/assets");
  }

  // Ensure app directory exists
  if (!SPIFFS.exists("/data")) {
    SPIFFS.mkdir("/data");
  }
}

FS_Error storage_common_migrate(Storage* storage, const char* source, const char* dest) {
  if (!storage) {
    return FS_ERROR;
  }

  if (!SPIFFS.rename(source, dest)) {
    return FS_ERROR;
  }

  return FS_OK;
}

bool storage_simply_remove(Storage* storage, const char* path) {
  if (!storage) {
    return false;
  }

  return SPIFFS.remove(path) || !SPIFFS.exists(path);
}

bool storage_simply_remove_recursive(Storage* storage, const char* path) {
  if (!storage) {
    return false;
  }

  if (!SPIFFS.exists(path)) {
    return true;
  }

  if (!SPIFFS.remove(path)) {
    fs::Dir dir = SPIFFS.openDir(path);
    while (dir.next()) {
      String entryPath = String(path) + "/" + dir.fileName();
      if (dir.isDirectory()) {
        storage_simply_remove_recursive(storage, entryPath.c_str());
      } else {
        SPIFFS.remove(entryPath);
      }
    }
    dir.close();

    if (!SPIFFS.rmdir(path)) {
      return false;
    }
  }

  return true;
}

bool storage_simply_mkdir(Storage* storage, const char* path) {
  if (!storage) {
    return false;
  }

  if (!SPIFFS.exists(path)) {
    return SPIFFS.mkdir(path);
  }

  return true;
}

void storage_get_next_filename(
    Storage* storage,
    const char* dirname,
    const char* filename,
    const char* fileextension,
    FuriString* nextfilename,
    uint8_t max_len) {
  if (!storage || !nextfilename) {
    return;
  }

    
  nextfilename->clear();
  nextfilename->reserve(max_len);

  uint16_t index = 1;
  while (true) {
    nextfilename->copy(dirname);
    nextfilename->append("/");
    nextfilename->append(filename);
    nextfilename->append(index);
    nextfilename->append(".");
    nextfilename->append(fileextension);

    if (!SPIFFS.exists(nextfilename->c_str())) {
      break;
    }

    index++;
  }
}
*/
