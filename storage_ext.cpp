#include <FS.h>
#include <SD.h>
#include "storage_ext.h"

// typedef u_int8_t SDFile;
// typedef u_int8_t SDDir;
// typedef u_int8_t SDFileInfo;
typedef u_int8_t SDError;

static FS_Error storage_ext_parse_error(SDError error) {
    FS_Error result;
    switch(error) {
    case 0:
        result = FSE_OK;
        break;
    case 1:
        result = FSE_NOT_READY;
        break;
    case 2:
    case 3:
    case 4:
        result = FSE_NOT_EXIST;
        break;
    case 5:
        result = FSE_EXIST;
        break;
    case 6:
        result = FSE_INVALID_NAME;
        break;
    case 7:
    case 8:
        result = FSE_INVALID_PARAMETER;
        break;
    case 9:
        result = FSE_DENIED;
        break;
    default:
        result = FSE_INTERNAL;
        break;
    }

    return result;
}



void storage_ext_init(StorageData* storage) {
    // if (!SD.begin(SD_CS)) {
    // pinMode(5, OUTPUT);
    // if (!SD.begin(5)){
    //     Serial.println(F("Failed to mount SD Card"));
    // } else {
    //     Serial.println(F("Mounted SD Card"));
    //     storage->status = StorageStatusOK;
    // }
    // storage->status = StorageStatusNotReady;
  
}

FS_Error sd_unmount_card(StorageData* storage) {
  // if (storage->status == StorageStatusOK) {
  //   storage->fs_api->unmount(storage->data);
  //   storage->status = StorageStatusNotReady;
  // }
  
  return FSE_OK;
}

FS_Error sd_format_card(StorageData* storage) {
  // if (storage->status != STORAGE_STATUS_INITIALIZED) {
  //   return FS_ERROR_INVALID_STATE;
  // }

  // storage->fs_api->format(storage->data);
  return FSE_OK;
}

FS_Error sd_card_info(StorageData* storage, SDInfo* sd_info) {
  // if (storage->status != StorageStatusOK) {
  //   return FS_ERROR_INVALID_STATE;
  // }

  uint8_t cardType = SD.cardType();
  sd_info->fs_type = cardType;

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);


  sd_info->kb_total = SD.totalBytes() / 1024;
  sd_info->kb_free = (SD.totalBytes() - SD.usedBytes()) / 1024;
  // sd_info->cluster_size = SD.blockSize();
  sd_info->sector_size = 512; // Assume sector size of 512 bytes

  // Read card information
  // SD.cardManufacturerID(sd_info->manufacturer_id);
  // SD.cardOemID(sd_info->oem_id);
  // SD.cardProductName(sd_info->product_name);
  // SD.cardProductRevision(sd_info->product_revision_major, sd_info->product_revision_minor);
  // SD.cardProductSerialNumber(sd_info->product_serial_number);
  // SD.cardManufacturingDate(sd_info->manufacturing_month, sd_info->manufacturing_year);

  // // Read card volume label
  // SD.getVolumeLabel(0, sd_info->label, SD_LABEL_LENGTH);

  return FSE_OK;
}
