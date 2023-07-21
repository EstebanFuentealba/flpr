#pragma once
#include <stdlib.h>
#include "storage.h"
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate a file stream with buffered read operations
 * @return FZStream*
 */
FZStream* buffered_file_stream_alloc(Storage* storage);

/**
 * Opens an existing file or creates a new one.
 * @param stream pointer to file stream object.
 * @param path path to file
 * @param access_mode access mode from FS_AccessMode
 * @param open_mode open mode from FS_OpenMode
 * @return True on success, False on failure. You need to close the file even if the open operation failed.
 */
bool buffered_file_stream_open(
    FZStream* stream,
    const char* path,
    const char* open_mode);

/**
 * Closes the file.
 * @param stream pointer to file stream object.
 * @return True on success, False on failure.
 */
bool buffered_file_stream_close(FZStream* stream);

/**
 * Forces write from cache to the underlying file.
 * @param stream pointer to file stream object.
 * @return True on success, False on failure.
 */
bool buffered_file_stream_sync(FZStream* stream);

/**
 * Retrieves the error id from the file object
 * @param stream pointer to stream object.
 * @return FS_Error error id
 */
FS_Error buffered_file_stream_get_error(FZStream* stream);

#ifdef __cplusplus
}
#endif