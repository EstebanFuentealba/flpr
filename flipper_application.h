/**
 * @file flipper_application.h
 * Flipper application
 */
#pragma once

#include "application_manifest.h"
// #include "elf/elf_api_interface.h"

#include "furi.h"
#include "storage.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FlipperApplicationPreloadStatusSuccess = 0,
    FlipperApplicationPreloadStatusUnspecifiedError,
    FlipperApplicationPreloadStatusInvalidFile,
    FlipperApplicationPreloadStatusInvalidManifest,
    FlipperApplicationPreloadStatusApiMismatch,
    FlipperApplicationPreloadStatusTargetMismatch,
} FlipperApplicationPreloadStatus;

typedef enum {
    FlipperApplicationLoadStatusSuccess = 0,
    FlipperApplicationLoadStatusUnspecifiedError,
    FlipperApplicationLoadStatusNoFreeMemory,
    FlipperApplicationLoadStatusMissingImports,
} FlipperApplicationLoadStatus;
typedef struct FlipperApplication FlipperApplication;

/**
 * @brief Initialize FlipperApplication object
 * @param storage Storage instance
 * @param api_interface ELF API interface to use for pre-loading and symbol resolving
 * @return Application instance
 */
FlipperApplication*
    flipper_application_alloc(Storage* storage);

/**
 * @brief Destroy FlipperApplication object
 * @param app Application pointer
 */
void flipper_application_free(FlipperApplication* app);

/**
 * @brief Validate elf file and load application metadata 
 * @param app Application pointer
 * @return Preload result code
 */
FlipperApplicationPreloadStatus
    flipper_application_preload(FlipperApplication* app, const char* path);

/**
 * @brief Get text description of preload status
 * @param status Status code
 * @return String pointer to description
 */
const char* flipper_application_preload_status_to_string(FlipperApplicationPreloadStatus status);

/**
 * @brief Get text description of load status
 * @param status Status code
 * @return String pointer to description
 */
const char* flipper_application_load_status_to_string(FlipperApplicationLoadStatus status);

/**
 * @brief Load sections and process relocations for already pre-loaded application
 * @param app Application pointer
 * @return Load result code
 */
FlipperApplicationLoadStatus flipper_application_map_to_memory(FlipperApplication* app);


/**
 * @brief Load name and icon from FAP file.
 * 
 * @param path Path to FAP file.
 * @param storage Storage instance.
 * @param icon_ptr Icon pointer.
 * @param item_name Application name.
 * @return true if icon and name were loaded successfully.
 */
bool flipper_application_load_name_and_icon(
    FuriString* path,
    Storage* storage,
    uint8_t** icon_ptr,
    FuriString* item_name);

#ifdef __cplusplus
}
#endif