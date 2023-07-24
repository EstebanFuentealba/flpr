#include <FS.h>
#include <SPI.h>
#include "SD.h"

#include "record.h"
#include "storage.h"
#include "storage_i.h"
#include "filesystem_api_internal.h"
#include "storage_message.h"
#include "storage_ext.h"
// #include <toolbox/stream/file_stream.h>
// #include <toolbox/dir_walk.h>
#include "path.h"

#define STORAGE_PATH_PREFIX_LEN 4u
// #define STORAGE_COUNT (ST_INT + 1)

#define APPS_DATA_PATH EXT_PATH("apps_data")
#define APPS_ASSETS_PATH EXT_PATH("apps_assets")


#define MAX_NAME_LENGTH 256
#define MAX_EXT_LEN 16
#define FILE_BUFFER_SIZE 512

#define TAG "StorageAPI"


typedef enum {
    StorageEventFlagFileClose = (1 << 0),
} StorageEventFlag;


static const char* cstr_path_without_vfs_prefix(FuriString* path) {
    const char* path_cstr = furi_string_get_cstr(path);
    return path_cstr + MIN(STORAGE_PATH_PREFIX_LEN, strlen(path_cstr));
}
FuriPubSub* storage_get_pubsub(Storage* storage) {
    return storage->pubsub;
}
FS_Error storage_common_stat(Storage* storage, const char* path, FileInfo* fileinfo) {
    if (!storage || !fileinfo) {
        return FSE_INVALID_PARAMETER;
    }

    fs::File entry = SD.open(path);
    if (!entry) {
        return FSE_NOT_EXIST;
    }
    if(entry.isDirectory()) {
        fileinfo->flags = FSF_DIRECTORY;
    }

    // fileinfo->is_dir = entry.isDirectory();
    fileinfo->size = entry.size();

    entry.close();
    return FSE_OK;
}

static bool storage_type_is_valid(StorageType type) {
    return type == ST_EXT;
}
static StorageType storage_get_type_by_path(FuriString* path) {
    StorageType type = ST_ERROR;
    const char* path_cstr = furi_string_get_cstr(path);

    if(furi_string_size(path) > STORAGE_PATH_PREFIX_LEN) {
        if(path_cstr[STORAGE_PATH_PREFIX_LEN] != '/') {
            return ST_ERROR;
        }
    }

    if(memcmp(path_cstr, STORAGE_EXT_PATH_PREFIX, strlen(STORAGE_EXT_PATH_PREFIX)) == 0) {
        type = ST_EXT;
    } else if(memcmp(path_cstr, STORAGE_ANY_PATH_PREFIX, strlen(STORAGE_ANY_PATH_PREFIX)) == 0) {
        type = ST_ANY;
    }

    return type;
}
static FS_Error storage_get_data(Storage* app, FuriString* path, StorageData** storage) {
    StorageType type = storage_get_type_by_path(path);

    if(storage_type_is_valid(type)) {
        *storage = &app->storage;
        return FSE_OK;
    } else {
        return FSE_INVALID_NAME;
    }
}
static FS_Error storage_process_common_stat(Storage* app, FuriString* path, FileInfo* fileinfo) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        ret = storage_common_stat(app, cstr_path_without_vfs_prefix(path), fileinfo );
    }

    return ret;
}
FS_Error storage_common_mkdir(Storage* storage, const char* path) {
    FuriString* spath = NULL;
    // spath = furi_string_alloc_set_str(path);
    // storage_process_alias(storage, spath, true);
    if(SD.mkdir(path)) {
        return FSE_OK;
    }
    return FSE_INTERNAL;
}
static FS_Error storage_process_common_mkdir(Storage* app, FuriString* path) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        storage_data_timestamp(storage);
        cstr_path_without_vfs_prefix(path);
        ret = storage_common_mkdir(app, cstr_path_without_vfs_prefix(path));
    }

    return ret;
}
void storage_process_alias(
    Storage* app,
    FuriString* path,
    // FuriThreadId thread_id,
    bool create_folders) {
    if(furi_string_start_with_str(path, STORAGE_APP_DATA_PATH_PREFIX)) {
        FuriString* apps_data_path_with_appsid = furi_string_alloc_set_str(APPS_DATA_PATH "/");
        // furi_string_cat(apps_data_path_with_appsid, furi_thread_get_appid(thread_id));

        // "/data" -> "/ext/apps_data/appsid"
        furi_string_replace_at(
            path,
            0,
            strlen(STORAGE_APP_DATA_PATH_PREFIX),
            furi_string_get_cstr(apps_data_path_with_appsid));

        // Create app data folder if not exists
        if(create_folders &&
           storage_process_common_stat(app, apps_data_path_with_appsid, NULL) != FSE_OK) {
            furi_string_set_str(apps_data_path_with_appsid, APPS_DATA_PATH);
            storage_process_common_mkdir(app, apps_data_path_with_appsid);
            furi_string_cat_str(apps_data_path_with_appsid, "/");
            // furi_string_cat(apps_data_path_with_appsid, furi_thread_get_appid(thread_id));
            storage_process_common_mkdir(app, apps_data_path_with_appsid);
        }

        furi_string_free(apps_data_path_with_appsid);
    } else if(furi_string_start_with_str(path, STORAGE_APP_ASSETS_PATH_PREFIX)) {
        FuriString* apps_assets_path_with_appsid = furi_string_alloc_set_str(APPS_ASSETS_PATH "/");
        // furi_string_cat(apps_assets_path_with_appsid, furi_thread_get_appid(thread_id));

        // "/assets" -> "/ext/apps_assets/appsid"
        furi_string_replace_at(
            path,
            0,
            strlen(STORAGE_APP_ASSETS_PATH_PREFIX),
            furi_string_get_cstr(apps_assets_path_with_appsid));

        furi_string_free(apps_assets_path_with_appsid);
    }
}
static FS_Error storage_process_common_fs_info(
    Storage* app,
    const char* path,
    uint64_t* total_space,
    uint64_t* free_space) {

    StorageData* storage;
    FS_Error ret = storage_get_data(app, furi_string_alloc_set_str(path), &storage);

    if(ret == FSE_OK) {
        // uint8_t cardType = SD.cardType();
        // sd_info->fs_type = cardType;

        uint64_t cardSize = SD.cardSize() / (1024 * 1024);


        *total_space = SD.totalBytes() / 1024;
        *free_space = (SD.totalBytes() - SD.usedBytes()) / 1024;
        // sd_info->cluster_size = SD.blockSize();


        // sd_info->sector_size = 512; // Assume sector size of 512 bytes
        ret = FSE_OK; //storage_common_fs_info(app, path, total_space, free_space);
    }

    return ret;
}

FS_Error storage_common_fs_info(
    Storage* storage,
    const char* fs_path,
    uint64_t* total_space,
    uint64_t* free_space) {
    FuriString* path = NULL;
    path = furi_string_alloc_set_str(fs_path);
    storage_process_alias(storage, path, false);
    return storage_process_common_fs_info(storage, fs_path, total_space, free_space);
}


FS_Error storage_common_remove(Storage* storage, const char* path) {
    FuriString* spath = NULL;
    spath = furi_string_alloc_set_str(path);
    storage_process_alias(storage, spath, false);
    if(SD.remove(furi_string_get_cstr(spath))){
        return FSE_OK; //torage_process_common_remove(storage, spath);
    } else {
        return FSE_INTERNAL;
    }
}

bool storage_common_exists(Storage* storage, const char* path) {
    FileInfo file_info;
    return storage_common_stat(storage, path, &file_info) == FSE_OK;
}



FS_Error storage_common_copy(Storage* storage, const char* old_path, const char* new_path) {
    FS_Error error;

    FileInfo fileinfo;
    error = storage_common_stat(storage, old_path, &fileinfo);

    if(error == FSE_OK) {
        if(file_info_is_dir(&fileinfo)) {
            // error = storage_copy_recursive(storage, old_path, new_path);
            error = storage_common_mkdir(storage, new_path);
  
            File file;

            do {
                if (error != FSE_OK) break;

                fs::File dir = SD.open(old_path);
                if (!dir) {
                error = FSE_NOT_EXIST;
                break;
                }

                if (!dir.isDirectory()) {
                    error = FSE_NOT_EXIST;
                    break;
                }

                while (true) {
                File entry = dir.openNextFile();
                if (!entry) {
                    break;
                }

                String entryName = entry.name();

                String newPath = String(new_path) + entryName.substring(strlen(old_path));
                // if (entry.isDirectory()) {
                //     error = storage_common_mkdir(storage, newPath.c_str());
                //     if (error != FSE_OK) break;
                //     error = storage_copy_recursive(storage, entryName.c_str(), newPath.c_str());
                //     if (error != FSE_OK) break;
                // // } else {
                    error = storage_common_copy(storage, entryName.c_str(), newPath.c_str());
                    if (error != FSE_OK) break;
                // }

                entry.close();
                }

                dir.close();

            } while (false);
        } else {

            File file_from = SD.open(old_path, FILE_READ);

            if (!file_from) {
                // Error al abrir el archivo de origen
                return FSE_NOT_EXIST;
            }

            
            File file_to = SD.open(new_path, FILE_WRITE);
            if (!file_to) {
                file_from.close();
                // Error al abrir el archivo de origen
                return FSE_NOT_EXIST;
            }


             while (file_from.available()) {
                uint8_t buffer[256];
                size_t bytesRead = file_from.read(buffer, sizeof(buffer));
                file_to.write(buffer, bytesRead);
            }

            file_from.close();
            file_to.close();
        }
    }

    return error;
}
static FS_Error storage_copy_recursive(Storage* storage, const char* old_path, const char* new_path) {
  FS_Error error = storage_common_mkdir(storage, new_path);
  
  File file;

  do {
    if (error != FSE_OK) break;

    fs::File dir = SD.open(old_path);
    if (!dir) {
      error = FSE_NOT_EXIST;
      break;
    }

    if (!dir.isDirectory()) {
        error = FSE_NOT_EXIST;
        break;
    }

    while (true) {
      File entry = dir.openNextFile();
      if (!entry) {
        break;
      }

      String entryName = entry.name();

      String newPath = String(new_path) + entryName.substring(strlen(old_path));
      if (entry.isDirectory()) {
        error = storage_common_mkdir(storage, newPath.c_str());
        if (error != FSE_OK) break;
        error = storage_copy_recursive(storage, entryName.c_str(), newPath.c_str());
        if (error != FSE_OK) break;
      } else {
        error = storage_common_copy(storage, entryName.c_str(), newPath.c_str());
        if (error != FSE_OK) break;
      }

      entry.close();
    }

    dir.close();

  } while (false);

  return error;
}
FS_Error storage_common_rename(Storage* storage, const char* old_path, const char* new_path) {
    FS_Error error;

    do {
        if(!storage_common_exists(storage, old_path)) {
            error = FSE_INVALID_NAME;
            break;
        }

        if(storage_dir_exists(storage, old_path)) {
            FuriString* dir_path = furi_string_alloc_set_str(old_path);
            if(!furi_string_end_with_str(dir_path, "/")) {
                furi_string_cat_str(dir_path, "/");
            }
            const char* dir_path_s = furi_string_get_cstr(dir_path);
            if(strncmp(new_path, dir_path_s, strlen(dir_path_s)) == 0) {
                error = FSE_INVALID_NAME;
                furi_string_free(dir_path);
                break;
            }
            furi_string_free(dir_path);
        }

        if(storage_file_exists(storage, new_path)) {
            storage_common_remove(storage, new_path);
        }

        error = storage_common_copy(storage, old_path, new_path);
        if(error != FSE_OK) {
            break;
        }

        if(!storage_simply_remove_recursive(storage, old_path)) {
            error = FSE_INTERNAL;
        }
    } while(false);

    return error;
}



FS_Error storage_merge_recursive(Storage* storage, const char* old_path, const char* new_path) {
  FS_Error error = FSE_OK;
  fs::File dir = SD.open(old_path);
  if (!dir.isDirectory()) {
    // El directorio de origen no existe o no es un directorio válido
    return FSE_NOT_EXIST;
  }

  if (!SD.mkdir(new_path)) {
    // Error al crear el directorio de destino
    return FSE_NOT_EXIST;
  }

  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No hay más entradas en el directorio
      break;
    }

    String entryName = entry.name();
    String entryPath = String(new_path) + "/" + entryName;

    if (entry.isDirectory()) {
      // Entrada actual es un directorio
      if (SD.exists(entryPath)) {
        // El directorio de destino ya existe
        entry.close();
        continue;
      }
      if (!SD.mkdir(entryPath)) {
        // Error al crear el directorio de destino
        entry.close();
        error = FSE_NOT_EXIST;
        break;
      }

      // Realiza la llamada recursiva para fusionar el directorio
      FS_Error mergeError = storage_merge_recursive(storage, entryName.c_str(), entryPath.c_str());
      if (mergeError != FSE_OK) {
        // Error al fusionar el directorio
        entry.close();
        error = mergeError;
        break;
      }
    } else {
      // Entrada actual es un archivo
      if (SD.exists(entryPath)) {
        // El archivo de destino ya existe
        entry.close();
        continue;
      }

      // Copia el archivo de origen al archivo de destino
      File destinationFile = SD.open(entryPath, FILE_WRITE);
      if (!destinationFile) {
        // Error al abrir el archivo de destino
        entry.close();
        error = FSE_NOT_EXIST;
        break;
      }

      while (entry.available()) {
        uint8_t buffer[256];
        size_t bytesRead = entry.read(buffer, sizeof(buffer));
        destinationFile.write(buffer, bytesRead);
      }

      destinationFile.close();
    }

    entry.close();
  }

  dir.close();

  return error;
}


FS_Error storage_common_merge(Storage* storage, const char* old_path, const char* new_path) {
    FS_Error error;
    const char* new_path_tmp = NULL;
    FuriString* new_path_next = NULL;
    new_path_next = furi_string_alloc();

    FileInfo fileinfo;
    error = storage_common_stat(storage, old_path, &fileinfo);

    if(error == FSE_OK) {
        if(file_info_is_dir(&fileinfo)) {
            error = storage_merge_recursive(storage, old_path, new_path);
        } else {
            error = storage_common_stat(storage, new_path, &fileinfo);
            if(error == FSE_OK) {
                furi_string_set_str(new_path_next, new_path);
                FuriString* dir_path;
                FuriString* filename;
                char extension[MAX_EXT_LEN] = {0};

                dir_path = furi_string_alloc();
                filename = furi_string_alloc();

                path_extract_filename(new_path_next, filename, true);
                path_extract_dirname(new_path, dir_path);
                path_extract_extension(new_path_next, extension, MAX_EXT_LEN);

                storage_get_next_filename(
                    storage,
                    furi_string_get_cstr(dir_path),
                    furi_string_get_cstr(filename),
                    extension,
                    new_path_next,
                    255);
                furi_string_cat_printf(
                    dir_path, "/%s%s", furi_string_get_cstr(new_path_next), extension);
                furi_string_set(new_path_next, dir_path);

                furi_string_free(dir_path);
                furi_string_free(filename);
                new_path_tmp = furi_string_get_cstr(new_path_next);
            } else {
                new_path_tmp = new_path;
            }

             File file_from = SD.open(old_path, FILE_READ);

            if (!file_from) {
                // Error al abrir el archivo de origen
                return FSE_NOT_EXIST;
            }

            
            File file_to = SD.open(new_path, FILE_WRITE);
            if (!file_to) {
                file_from.close();
                // Error al abrir el archivo de origen
                return FSE_NOT_EXIST;
            }


             while (file_from.available()) {
                uint8_t buffer[256];
                size_t bytesRead = file_from.read(buffer, sizeof(buffer));
                file_to.write(buffer, bytesRead);
            }

            file_from.close();
            file_to.close();

            
            
        }
    }

    furi_string_free(new_path_next);

    return error;
}

static bool storage_dir_open_internal(FZFile* file, const char* path) {
    fs::File root = SD.open(path);
    // Serial.println("root");
    // Serial.println(root.isDirectory());
    if(!root.isDirectory()){
        file->type = FileTypeOpenFile;
        return false;
    }
    file->fsFile = root;
    file->type = FileTypeOpenDir;
    return true;
}

static FS_Error storage_process_sd_status(Storage* app) {
    FS_Error ret;
    StorageStatus status = storage_data_status(&app->storage);

    switch(status) {
    case StorageStatusOK:
        ret = FSE_OK;
        break;
    case StorageStatusNotReady:
        ret = FSE_NOT_READY;
        break;
    default:
        ret = FSE_INTERNAL;
        break;
    }

    return ret;
}

static FS_Error storage_process_sd_info(Storage* app, SDInfo* info) {
    FS_Error ret = FSE_OK;

    if(storage_data_status(&app->storage) == StorageStatusNotReady) {
        ret = FSE_NOT_READY;
    } else {
        ret = sd_card_info(&app->storage, info);
    }

    return ret;
}





static FS_Error
    storage_process_common_timestamp(Storage* app, FuriString* path, uint32_t* timestamp) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    if(ret == FSE_OK) {
        *timestamp = storage_data_get_timestamp(storage);
    }

    return ret;
}


FS_Error storage_common_timestamp(Storage* storage, const char* path, uint32_t* timestamp) {
    FuriString* spath = NULL;
    spath = furi_string_alloc_set_str(path);
    return storage_process_common_timestamp(storage, spath, timestamp);
}


// static FS_Error storage_process_common_mkdir(Storage* app, FuriString* path) {
//     StorageData* storage;
//     FS_Error ret = storage_get_data(app, path, &storage);

//     if(ret == FSE_OK) {
//         storage_data_timestamp(storage);
//         ret = storage_common_mkdir(app, cstr_path_without_vfs_prefix(path));
//     }

//     return ret;
// }






FZFile* storage_file_alloc(Storage* storage) {
    return new FZFile();
}

void storage_file_free(FZFile* file) {
    delete file;
}

// FSE_OK, /**< No error */
// FSE_NOT_READY, /**< FS not ready */
// FSE_EXIST, /**< File/Dir already exist */
// FSE_NOT_EXIST, /**< File/Dir does not exist */
// FSE_INVALID_PARAMETER, /**< Invalid API parameter */
// FSE_DENIED, /**< Access denied */
// FSE_INVALID_NAME, /**< Invalid name/path */
// FSE_INTERNAL, /**< Internal error */
// FSE_NOT_IMPLEMENTED, /**< Function not implemented */
// FSE_ALREADY_OPEN, /**< File/Dir already opened */

bool storage_file_open(
    FZFile* file,
    const char* path,
    const char* open_mode) {
    fs::File fsfile = SD.open(path, open_mode);
    file->fsFile = fsfile;
    file->type = FileTypeOpenFile;
    file->error_id = FSE_OK;
    file->internal_error_id = 0;
    file->storage = nullptr;

    if (!file->fsFile) {
        file->error_id = FSE_INTERNAL;
        return false;
    }

    return true;
}

bool storage_file_close(FZFile* file) {
    if (file->type == FileTypeOpenFile) {
        file->fsFile.close();
    }
    file->type = FileTypeClosed;
    return true;
}

bool storage_file_is_open(FZFile* file) {
    return (file->type == FileTypeOpenFile || file->type == FileTypeOpenDir);
}

bool storage_file_is_dir(FZFile* file) {
    return (file->type == FileTypeOpenDir);
}

uint16_t storage_file_read(FZFile* file, void* buff, uint16_t bytes_to_read) {
    return file->fsFile.read(static_cast<uint8_t*>(buff), bytes_to_read);
}

uint16_t storage_file_write(FZFile* file, const void* buff, uint16_t bytes_to_write) {
    return file->fsFile.write(static_cast<const uint8_t*>(buff), bytes_to_write);
}

bool storage_file_seek(FZFile* file, uint32_t offset, bool from_start) {
    if (from_start) {
        return file->fsFile.seek(offset);
    } else {
        return file->fsFile.seek(offset, SeekCur);
    }
}

uint64_t storage_file_tell(FZFile* file) {
    return file->fsFile.position();
}

bool storage_file_truncate(FZFile* file) {
    // Get the current position of the file pointer
    uint64_t currentPosition = file->fsFile.position();

    // Truncate the file by seeking to the current position
    if (file->fsFile.seek(currentPosition)) {
        // If the seek operation was successful, truncate the file by writing an empty buffer
        if (file->fsFile.write(reinterpret_cast<const uint8_t*>(""), 0) == 0) {
        // If the write operation was successful, seek back to the original position
        file->fsFile.seek(currentPosition);
        return true;
        }
    }

    // If any operation failed, seek to the end of the file
    file->fsFile.seek(file->fsFile.size());
    return false;
}

uint64_t storage_file_size(FZFile* file) {
    return file->fsFile.size();
}

bool storage_file_sync(FZFile* file) {
if (file) {
    file->fsFile.flush();
    return true;
  }
  return false;
}

bool storage_file_eof(FZFile* file) {
    return file->fsFile.available() == false;
}

bool storage_file_exists(Storage* storage, const char* path) {
    return storage_common_stat(storage, path, nullptr) == FSE_OK;
}

bool storage_file_copy_to_file(FZFile* source, FZFile* destination, uint32_t size) {
    uint8_t buffer[256];
    uint32_t remaining_bytes = size;

    while (remaining_bytes > 0) {
        uint16_t bytes_to_read = min(remaining_bytes, sizeof(buffer));
        uint16_t bytes_read = storage_file_read(source, buffer, bytes_to_read);
        if (bytes_read == 0) {
            return false;
        }
        uint16_t bytes_written = storage_file_write(destination, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            return false;
        }
        remaining_bytes -= bytes_written;
    }

    return true;
}

static void storage_file_close_callback(const void* message, void* context) {
    const StorageEvent* storage_event = (StorageEvent*)message;

    if(storage_event->type == StorageEventTypeFileClose ||
       storage_event->type == StorageEventTypeDirClose) {
        // furi_assert(context);
        // FuriEventFlag* event = context;
        // furi_event_flag_set(event, StorageEventFlagFileClose);
    }
}

bool storage_dir_open(FZFile* file, const char* path) {
 
    bool result;
    // FuriEventFlag* event = furi_event_flag_alloc();
    FuriPubSubSubscription* subscription = furi_pubsub_subscribe(storage_get_pubsub((Storage*)file->storage), storage_file_close_callback, NULL);

    do {
        result = storage_dir_open_internal(file, path);
        // Serial.println("storage_dir_open_internal");
        // Serial.println(result);
        if(!result && file->error_id == FSE_ALREADY_OPEN) {
            // furi_event_flag_wait(event, StorageEventFlagFileClose, FuriFlagWaitAny, FuriWaitForever);
        } else {
            break;
        }
    } while(true);

    furi_pubsub_unsubscribe(storage_get_pubsub((Storage*)file->storage), subscription);
    // furi_event_flag_free(event);


    return result;
}

bool storage_dir_close(FZFile* file) {
    if (file->type == FileTypeOpenDir) {
        file->fsFile.close();
    }
    file->type = FileTypeClosed;
    return true;
}

bool storage_dir_read(FZFile* file, FileInfo* fileinfo, char* name, uint16_t name_length) {
    if (file->type != FileTypeOpenDir) {
        return false;
    }
    // Serial.print("dir name: ");
    // Serial.println(file->fsFile.name());
    fs::File entry = file->fsFile.openNextFile();
    // Serial.print("name: ");
    // Serial.println(entry.name());
    if (!entry) {
        file->error_id = FSE_NOT_EXIST;
        return false;
    }

    if (fileinfo != nullptr) {
        fileinfo->size = entry.size();
        // fileinfo->is_dir = entry.isDirectory();
        // fileinfo->is_readonly = false; // Modify as per your requirements
        // fileinfo->timestamp = entry.getLastWrite();
    }

    if (name != nullptr && name_length > 0) {
        strncpy(name, entry.name(), name_length);
    }

    entry.close();
    return true;
}

bool storage_dir_rewind(FZFile* file) {
    if (file->type != FileTypeOpenDir) {
        return false;
    }

    file->fsFile.rewindDirectory();
    return true;
}

bool storage_dir_exists(Storage* storage, const char* path) {
    bool exist = false;
    FileInfo fileinfo;
    FS_Error error = storage_common_stat(storage, path, &fileinfo);

    if(error == FSE_OK && file_info_is_dir(&fileinfo)) {
        exist = true;
    }

    return exist;
}


static FS_Error storage_process_common_remove(Storage* app, FuriString* path) {
    StorageData* storage;
    FS_Error ret = storage_get_data(app, path, &storage);

    do {
        if(storage_path_already_open(path, storage)) {
            ret = FSE_ALREADY_OPEN;
            break;
        }

        storage_data_timestamp(storage);
        ret = storage_common_remove(app, cstr_path_without_vfs_prefix(path));
    } while(false);

    return ret;
}




void storage_common_resolve_path_and_ensure_app_directory(Storage* storage, FuriString* path) {
    storage_process_alias(storage, path, true);
}

// FS_Error storage_sd_format(Storage* api) {
//     return storage_sd_api_format(api->data);
// }

// FS_Error storage_sd_unmount(Storage* api) {
//     return storage_sd_api_unmount(api->data);
// }

FS_Error storage_sd_info(Storage* api, SDInfo* info) {
    return storage_process_sd_info(api, info);
}

FS_Error storage_sd_status(Storage* api) {
    return storage_process_sd_status(api);
}

const char* storage_error_get_desc(FS_Error error_id) {
    return filesystem_api_error_get_desc(error_id);
}

FS_Error storage_file_get_error(FZFile* file) {
    return file->error_id;
}

int32_t storage_file_get_internal_error(FZFile* file) {
    return file->internal_error_id;
}

const char* storage_file_get_error_desc(FZFile* file) {
    return storage_error_get_desc(file->error_id);
}

bool storage_simply_remove(Storage* storage, const char* path) {
    FS_Error result;
    result = storage_common_remove(storage, path);
    return result == FSE_OK || result == FSE_NOT_EXIST;
}

bool storage_simply_remove_recursive(Storage* storage, const char* path) {
    // Remove all files and subdirectories in the directory
    FZFile dir;
    if (storage_dir_open(&dir, path)) {
        char name[256];
        while (storage_dir_read(&dir, nullptr, name, sizeof(name))) {
            if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
                char subpath[512];
                snprintf(subpath, sizeof(subpath), "%s/%s", path, name);
                if (storage_file_is_dir(&dir)) {
                    storage_simply_remove_recursive(storage, subpath);
                } else {
                    storage_simply_remove(storage, subpath);
                }
            }
        }
        storage_dir_close(&dir);
    }

    // Remove the directory itself
    return storage_simply_remove(storage, path);
}

bool storage_simply_mkdir(Storage* storage, const char* path) {
    FS_Error result;
    result = storage_common_mkdir(storage, path);
    return result == FSE_OK || result == FSE_EXIST;
}

void storage_get_next_filename(
    Storage* storage,
    const char* dirname,
    const char* filename,
    const char* fileextension,
    FuriString* nextfilename,
    uint8_t max_len) {
    FuriString* temp_str;
    uint16_t num = 0;

    temp_str = furi_string_alloc_printf("%s/%s%s", dirname, filename, fileextension);

    while(storage_common_stat(storage, furi_string_get_cstr(temp_str), NULL) == FSE_OK) {
        num++;
        furi_string_printf(temp_str, "%s/%s%d%s", dirname, filename, num, fileextension);
    }
    if(num && (max_len > strlen(filename))) {
        furi_string_printf(nextfilename, "%s%d", filename, num);
    } else {
        furi_string_printf(nextfilename, "%s", filename);
    }

    furi_string_free(temp_str);
}
