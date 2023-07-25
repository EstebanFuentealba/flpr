#include "saved_struct.h"
#include "furi.h"
#include <stdint.h>
#include "storage.h"

#define TAG "SavedStruct"

typedef struct {
    uint8_t magic;
    uint8_t version;
    uint8_t checksum;
    uint8_t flags;
    uint32_t timestamp;
} SavedStructHeader;

bool saved_struct_save(const char* path, void* data, size_t size, uint8_t magic, uint8_t version) {
    // furi_assert(path);
    // furi_assert(data);
    // furi_assert(size);
    SavedStructHeader header;

    Serial.printf("Saving \"%s\"\n", path);

    // Store
    Storage* storage = (Storage*)furi_record_open(RECORD_STORAGE);
    FZFile* file = storage_file_alloc(storage);
    bool result = true;
    bool saved = storage_file_open(file, path, FILE_WRITE);
    if(!saved) {
        Serial.printf("Open failed \"%s\". Error: \'%s\'\n", path, storage_file_get_error_desc(file));
        result = false;
    }

    if(result) {
        // Calculate checksum
        uint8_t checksum = 0;
        uint8_t* source = (uint8_t*)data;
        for(size_t i = 0; i < size; i++) {
            checksum += source[i];
        }
        // Set header
        header.magic = magic;
        header.version = version;
        header.checksum = checksum;
        header.flags = 0;
        header.timestamp = 0;

        uint16_t bytes_count = storage_file_write(file, &header, sizeof(header));
        bytes_count += storage_file_write(file, data, size);

        if(bytes_count != (size + sizeof(header))) {
           Serial.printf("Write failed \"%s\". Error: \'%s\'\n", path, storage_file_get_error_desc(file));
            result = false;
        }
    }

    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    return result;
}

bool saved_struct_load(const char* path, void* data, size_t size, uint8_t magic, uint8_t version) {
    Serial.printf("Loading \"%s\"\n", path);

    SavedStructHeader header;

    uint8_t* data_read = (uint8_t*)malloc(size);
    Storage* storage = (Storage*)furi_record_open(RECORD_STORAGE);
    Serial.printf("is open storage.\n");
    FZFile* file = storage_file_alloc(storage);
    bool result = true;
    bool loaded = storage_file_open(file, path, FILE_WRITE);
    if(!loaded) {
        Serial.printf("Failed to read \"%s\".\n", path);
        Serial.printf("Failed to read \"%s\". Error: %s\n", path, storage_file_get_error_desc(file));
        result = false;
    } else {
         Serial.printf("read \"%s\".\n", path);
    }

    if(result) {
        uint16_t bytes_count = storage_file_read(file, &header, sizeof(SavedStructHeader));
        bytes_count += storage_file_read(file, data_read, size);

        if(bytes_count != (sizeof(SavedStructHeader) + size)) {
            Serial.printf("Size mismatch of file \"%s\"\n", path);
            result = false;
        }
    }

    if(result && (header.magic != magic || header.version != version)) {
        Serial.printf("Magic(%d != %d) or Version(%d != %d) mismatch of file \"%s\"\n",
            header.magic,
            magic,
            header.version,
            version,
            path);
        result = false;
    }

    if(result) {
        uint8_t checksum = 0;
        const uint8_t* source = (const uint8_t*)data_read;
        for(size_t i = 0; i < size; i++) {
            checksum += source[i];
        }

        if(header.checksum != checksum) {
            Serial.printf("Checksum(%d != %d) mismatch of file \"%s\"\n", header.checksum, checksum, path);
            result = false;
        }
    }

    if(result) {
        memcpy(data, data_read, size);
    }

    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
    free(data_read);

    return result;
}

bool saved_struct_get_payload_size(
    const char* path,
    uint8_t magic,
    uint8_t version,
    size_t* payload_size) {
    // furi_assert(path);
    // furi_assert(payload_size);

    SavedStructHeader header;
    Storage* storage = (Storage*)furi_record_open(RECORD_STORAGE);
    FZFile* file = storage_file_alloc(storage);

    bool result = false;
    do {
        if(!storage_file_open(file, path, FILE_WRITE)) {
            Serial.printf("Failed to read \"%s\". Error: %s\n", path, storage_file_get_error_desc(file));
            break;
        }

        uint16_t bytes_count = storage_file_read(file, &header, sizeof(SavedStructHeader));
        if(bytes_count != sizeof(SavedStructHeader)) {
            Serial.printf("Failed to read header\n");
            break;
        }

        if((header.magic != magic) || (header.version != version)) {
            Serial.printf("Magic(%d != %d) or Version(%d != %d) mismatch of file \"%s\"\n",
                header.magic,
                magic,
                header.version,
                version,
                path);
            break;
        }

        uint64_t file_size = storage_file_size(file);
        *payload_size = file_size - sizeof(SavedStructHeader);

        result = true;
    } while(false);

    storage_file_close(file);
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);

    return result;
}