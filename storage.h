#pragma once
#include <stdint.h>
#include "filesystem_api_defines.h"
#include "storage_sd_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STORAGE_INT_PATH_PREFIX "/int"
#define STORAGE_EXT_PATH_PREFIX "/ext"
#define STORAGE_ANY_PATH_PREFIX "/any"
#define STORAGE_APP_DATA_PATH_PREFIX "/data"
#define STORAGE_APP_ASSETS_PATH_PREFIX "/assets"

#define INT_PATH(path) STORAGE_INT_PATH_PREFIX "/" path
#define EXT_PATH(path) STORAGE_EXT_PATH_PREFIX "/" path
#define ANY_PATH(path) STORAGE_ANY_PATH_PREFIX "/" path
#define APP_DATA_PATH(path) STORAGE_APP_DATA_PATH_PREFIX "/" path
#define APP_ASSETS_PATH(path) STORAGE_APP_ASSETS_PATH_PREFIX "/" path

#define RECORD_STORAGE "storage"

typedef struct Storage Storage;

/** Allocates and initializes a file descriptor
 * @return FZFile*
 */
FZFile* storage_file_alloc(Storage* storage);

/** Frees the file descriptor. Closes the file if it was open.
 */
void storage_file_free(FZFile* file);

typedef enum {
    StorageEventTypeCardMount,
    StorageEventTypeCardUnmount,
    StorageEventTypeCardMountError,
    StorageEventTypeFileClose,
    StorageEventTypeDirClose,
} StorageEventType;

typedef struct {
    StorageEventType type;
} StorageEvent;

/**
 * Get storage pubsub.
 * Storage will send StorageEvent messages.
 * @param storage 
 * @return FuriPubSub* 
 */
FuriPubSub* storage_get_pubsub(Storage* storage);

/******************* File Functions *******************/

/** Opens an existing file or create a new one.
 * @param file pointer to file object.
 * @param path path to file 
 * @param access_mode access mode from FS_AccessMode 
 * @param open_mode open mode from FS_OpenMode 
 * @return success flag. You need to close the file even if the open operation failed.
 */
bool storage_file_open(
    FZFile* file,
    const char* path,
    // FS_AccessMode access_mode,
    const char*  open_mode);

/** Close the file.
 * @param file pointer to a file object, the file object will be freed.
 * @return success flag
 */
bool storage_file_close(FZFile* file);

/** Tells if the file is open
 * @param file pointer to a file object
 * @return bool true if file is open
 */
bool storage_file_is_open(FZFile* file);

/** Tells if the file is a directory
 * @param file pointer to a file object
 * @return bool true if file is a directory
 */
bool storage_file_is_dir(FZFile* file);

/** Reads bytes from a file into a buffer
 * @param file pointer to file object.
 * @param buff pointer to a buffer, for reading
 * @param bytes_to_read how many bytes to read. Must be less than or equal to the size of the buffer.
 * @return uint16_t how many bytes were actually read
 */
uint16_t storage_file_read(FZFile* file, void* buff, uint16_t bytes_to_read);

/** Writes bytes from a buffer to a file
 * @param file pointer to file object.
 * @param buff pointer to buffer, for writing
 * @param bytes_to_write how many bytes to write. Must be less than or equal to the size of the buffer.
 * @return uint16_t how many bytes were actually written
 */
uint16_t storage_file_write(FZFile* file, const void* buff, uint16_t bytes_to_write);

/** Moves the r/w pointer 
 * @param file pointer to file object.
 * @param offset offset to move the r/w pointer
 * @param from_start set an offset from the start or from the current position
 * @return success flag
 */
bool storage_file_seek(FZFile* file, uint32_t offset, bool from_start);

/** Gets the position of the r/w pointer 
 * @param file pointer to file object.
 * @return uint64_t position of the r/w pointer 
 */
uint64_t storage_file_tell(FZFile* file);

/** Truncates the file size to the current position of the r/w pointer
 * @param file pointer to file object.
 * @return bool success flag
 */
bool storage_file_truncate(FZFile* file);

/** Gets the size of the file
 * @param file pointer to file object.
 * @return uint64_t size of the file
 */
uint64_t storage_file_size(FZFile* file);

/** Writes file cache to storage
 * @param file pointer to file object.
 * @return bool success flag
 */
bool storage_file_sync(FZFile* file);

/** Checks that the r/w pointer is at the end of the file
 * @param file pointer to file object.
 * @return bool success flag
 */
bool storage_file_eof(FZFile* file);

/**
 * @brief Check that file exists
 * 
 * @param storage 
 * @param path 
 * @return true if file exists
 */
bool storage_file_exists(Storage* storage, const char* path);

/**
 * @brief Copy data from one opened file to another opened file
 * Size bytes will be copied from current position of source file to current position of destination file
 * 
 * @param source source file
 * @param destination destination file
 * @param size size of data to copy
 * @return bool success flag
 */
bool storage_file_copy_to_file(FZFile* source, FZFile* destination, uint32_t size);

/******************* Dir Functions *******************/

/** Opens a directory to get objects from it
 * @param app pointer to the api
 * @param file pointer to file object.
 * @param path path to directory
 * @return bool success flag. You need to close the directory even if the open operation failed.
 */
bool storage_dir_open(FZFile* file, const char* path);

/** Close the directory. Also free file handle structure and point it to the NULL.
 * @param file pointer to a file object.
 * @return bool success flag
 */
bool storage_dir_close(FZFile* file);

/** Reads the next object in the directory
 * @param file pointer to file object.
 * @param fileinfo pointer to the read FileInfo, may be NULL
 * @param name pointer to name buffer, may be NULL
 * @param name_length name buffer length
 * @return success flag (if the next object does not exist, it also returns false and sets the file error id to FSE_NOT_EXIST)
 */
bool storage_dir_read(FZFile* file, FileInfo* fileinfo, char* name, uint16_t name_length);

/** Rewinds the read pointer to first item in the directory
 * @param file pointer to file object.
 * @return bool success flag
 */
bool storage_dir_rewind(FZFile* file);

/**
 * @brief Check that dir exists
 * 
 * @param storage 
 * @param path 
 * @return bool 
 */
bool storage_dir_exists(Storage* storage, const char* path);


/******************* Error Functions *******************/

/** Retrieves the error text from the error id
 * @param error_id error id
 * @return const char* error text
 */
const char* storage_error_get_desc(FS_Error error_id);

/** Retrieves the error id from the file object
 * @param file pointer to file object. Pointer must not point to NULL. YOU CANNOT RETRIEVE THE ERROR ID IF THE FILE HAS BEEN CLOSED
 * @return FS_Error error id
 */
FS_Error storage_file_get_error(FZFile* file);

/** Retrieves the internal (storage-specific) error id from the file object
 * @param file pointer to file object. Pointer must not point to NULL. YOU CANNOT RETRIEVE THE INTERNAL ERROR ID IF THE FILE HAS BEEN CLOSED
 * @return FS_Error error id
 */
int32_t storage_file_get_internal_error(FZFile* file);

/** Retrieves the error text from the file object
 * @param file pointer to file object. Pointer must not point to NULL. YOU CANNOT RETRIEVE THE ERROR TEXT IF THE FILE HAS BEEN CLOSED
 * @return const char* error text
 */
const char* storage_file_get_error_desc(FZFile* file);

/******************* SD Card Functions *******************/

/** Formats SD Card
 * @param api pointer to the api
 * @return FS_Error operation result
 */
FS_Error storage_sd_format(Storage* api);

/** Will unmount the SD card
 * @param api pointer to the api
 * @return FS_Error operation result
 */
FS_Error storage_sd_unmount(Storage* api);

/** Retrieves SD card information
 * @param api pointer to the api
 * @param info pointer to the info
 * @return FS_Error operation result
 */
FS_Error storage_sd_info(Storage* api, SDInfo* info);

/** Retrieves SD card status
 * @param api pointer to the api
 * @return FS_Error operation result
 */
FS_Error storage_sd_status(Storage* api);

/******************* Internal LFS Functions *******************/

typedef void (*Storage_name_converter)(FuriString*);

/** Backs up internal storage to a tar archive
 * @param api pointer to the api
 * @param dstmane destination archive path
 * @return FS_Error operation result
 */
FS_Error storage_int_backup(Storage* api, const char* dstname);

/** Restores internal storage from a tar archive
 * @param api pointer to the api
 * @param dstmane archive path
 * @param converter pointer to filename conversion function, may be NULL
 * @return FS_Error operation result
 */
FS_Error storage_int_restore(Storage* api, const char* dstname, Storage_name_converter converter);

/***************** Simplified Functions ******************/

/**
 * Removes a file/directory, the directory must be empty and the file/directory must not be open
 * @param storage pointer to the api
 * @param path 
 * @return true on success or if file/dir is not exist
 */
bool storage_simply_remove(Storage* storage, const char* path);

/**
 * Recursively removes a file/directory, the directory can be not empty
 * @param storage pointer to the api
 * @param path
 * @return true on success or if file/dir is not exist
 */
bool storage_simply_remove_recursive(Storage* storage, const char* path);

/**
 * Creates a directory
 * @param storage 
 * @param path 
 * @return true on success or if directory is already exist
 */
bool storage_simply_mkdir(Storage* storage, const char* path);

/**
 * @brief Get next free filename.
 * 
 * @param storage
 * @param dirname 
 * @param filename 
 * @param fileextension 
 * @param nextfilename return name
 * @param max_len  max len name
 */
void storage_get_next_filename(
    Storage* storage,
    const char* dirname,
    const char* filename,
    const char* fileextension,
    FuriString* nextfilename,
    uint8_t max_len);

#ifdef __cplusplus
}
#endif