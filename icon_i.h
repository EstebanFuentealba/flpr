/**
 * @file icon_i.h
 * GUI: internal Icon API
 */

#ifndef ICON_I_H
#define ICON_I_H

#include "icon.h"

struct Icon {
    const uint8_t width;
    const uint8_t height;
    const uint8_t frame_count;
    const uint8_t frame_rate;
    const uint8_t* const* frames;
};

#endif  // ICON_I_H