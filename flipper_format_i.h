#pragma once
#include "stream.h"
#include "flipper_format.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns the underlying stream instance.
 * Use only if you know what you are doing.
 * @param flipper_format 
 * @return Stream* 
 */
FZStream* flipper_format_get_raw_stream(FlipperFormat* flipper_format);

#ifdef __cplusplus
}
#endif