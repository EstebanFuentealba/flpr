#pragma once
#include <stdlib.h>
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate string stream
 * @return Stream* 
 */
FZStream* string_stream_alloc();

#ifdef __cplusplus
}
#endif