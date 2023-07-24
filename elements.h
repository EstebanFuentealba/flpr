#pragma once

#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <stdint.h>
#include "canvas.h"
#include "furi_string.h"
// #include "core2/string.h"


#define ELEMENTS_MAX_LINES_NUM (7)
#define ELEMENTS_BOLD_MARKER '#'
#define ELEMENTS_MONO_MARKER '*'
#define ELEMENTS_INVERSED_MARKER '!'

const unsigned char bitmap_scrollbar_background [] PROGMEM = {
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 
  0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00
};
const uint8_t* const frames_scrollbar[] = { bitmap_scrollbar_background };



void elements_progress_bar(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, float progress);

void elements_progress_bar_with_text(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    float progress,
    const char* text);

void elements_scrollbar_pos(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t height,
    uint16_t pos,
    uint16_t total);

void elements_scrollbar(Canvas* canvas, uint16_t pos, uint16_t total);

void elements_frame(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void elements_button_left(Canvas* canvas, const char* str);

void elements_button_right(Canvas* canvas, const char* str);

void elements_button_center(Canvas* canvas, const char* str);

void elements_multiline_text_aligned(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical,
    const char* text);

void elements_multiline_text(Canvas* canvas, uint8_t x, uint8_t y, const char* text);

void elements_multiline_text_framed(Canvas* canvas, uint8_t x, uint8_t y, const char* text);

void elements_slightly_rounded_frame(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height);

void elements_slightly_rounded_box(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height);

void elements_bold_rounded_frame(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height);

void elements_bubble(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void elements_bubble_str(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    const char* text,
    Align horizontal,
    Align vertical);

/** Trim string buffer to fit width in pixels
 *
 * @param   canvas  Canvas instance
 * @param   string  string to trim
 * @param   width   max width
 */
void elements_string_fit_width(Canvas* canvas, FuriString* string, uint8_t width);

void elements_scrollable_text_line(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    FuriString* string,
    size_t scroll,
    bool ellipsis);

void elements_text_box(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    Align horizontal,
    Align vertical,
    const char* text,
    bool strip_to_dots);


#endif  // ELEMENTS_H
