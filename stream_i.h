#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STREAM_CACHE_SIZE 512u

typedef struct StreamVTable StreamVTable;

typedef void (*StreamFreeFn)(FZStream* stream);
typedef bool (*StreamEOFFn)(FZStream* stream);
typedef void (*StreamCleanFn)(FZStream* stream);
typedef bool (*StreamSeekFn)(FZStream* stream, int32_t offset, StreamOffset offset_type);
typedef size_t (*StreamTellFn)(FZStream* stream);
typedef size_t (*StreamSizeFn)(FZStream* stream);
typedef size_t (*StreamWriteFn)(FZStream* stream, const uint8_t* data, size_t size);
typedef size_t (*StreamReadFn)(FZStream* stream, uint8_t* data, size_t count);
typedef bool (*StreamDeleteAndInsertFn)(
    FZStream* stream,
    size_t delete_size,
    StreamWriteCB write_cb,
    const void* ctx);

struct StreamVTable {
    const StreamFreeFn free;
    const StreamEOFFn eof;
    const StreamCleanFn clean;
    const StreamSeekFn seek;
    const StreamTellFn tell;
    const StreamSizeFn size;
    const StreamWriteFn write;
    const StreamReadFn read;
    const StreamDeleteAndInsertFn delete_and_insert;
};

struct FZStream {
    const StreamVTable* vtable;
};

#ifdef __cplusplus
}
#endif