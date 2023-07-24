/**
 * @file canvas_i.h
 * GUI: internal Canvas API
 */

#pragma once

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "compress.h"
#include "canvas.h"

// OLED display width, in pixels
#define SCREEN_WIDTH 128
// OLED display height, in pixels
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_RESET -1


typedef struct Adafruit_SSD1306* display_t;

/** Canvas structure */
struct Canvas {
    display_t display;
    CanvasOrientation orientation;
    uint8_t offset_x;
    uint8_t offset_y;
    uint8_t width;
    uint8_t height;
    CompressIcon* compress_icon;
};

/** Allocate memory and initialize canvas
 *
 * @return     Canvas instance
 */
Canvas* canvas_init();

/** Free canvas memory
 *
 * @param      canvas  Canvas instance
 */
void canvas_free(Canvas* canvas);

/** Get canvas buffer.
 *
 * @param      canvas  Canvas instance
 *
 * @return     pointer to buffer
 */
uint8_t* canvas_get_buffer(Canvas* canvas);

/** Get canvas buffer size.
 *
 * @param      canvas  Canvas instance
 *
 * @return     size of canvas in bytes
 */
size_t canvas_get_buffer_size(const Canvas* canvas);

/** Set drawing region relative to real screen buffer
 *
 * @param      canvas    Canvas instance
 * @param      offset_x  x coordinate offset
 * @param      offset_y  y coordinate offset
 * @param      width     width
 * @param      height    height
 */
void canvas_frame_set(
    Canvas* canvas,
    uint8_t offset_x,
    uint8_t offset_y,
    uint8_t width,
    uint8_t height);

/** Set canvas orientation
 *
 * @param      canvas       Canvas instance
 * @param      orientation  CanvasOrientation
 */
void canvas_set_orientation(Canvas* canvas, CanvasOrientation orientation);

/** Get canvas orientation
 *
 * @param      canvas  Canvas instance
 *
 * @return     CanvasOrientation
 */
CanvasOrientation canvas_get_orientation(const Canvas* canvas);

/** Draw a u8g2 bitmap
 *
 * @param      canvas   Canvas instance
 * @param      x        x coordinate
 * @param      y        y coordinate
 * @param      width    width
 * @param      height   height
 * @param      bitmap   bitmap
 * @param      rotation rotation
 */
// void canvas_draw_u8g2_bitmap(
//     Adafruit_SSD1306* display,
//     uint8_t x,
//     uint8_t y,
//     uint8_t width,
//     uint8_t height,
//     const uint8_t* bitmap,
//     uint8_t rotation);
void canvas_set_color_white(Canvas* canvas);

/** Draw a u8g2 bitmap
 *
 * @param      canvas   Canvas instance
 * @param      x        x coordinate
 * @param      y        y coordinate
 * @param      width    width
 * @param      height   height
 * @param      bitmap   bitmap
 * @param      rotation rotation
 */
void canvas_draw_u8g2_bitmap(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t w,
    uint8_t h,
    const uint8_t* bitmap,
    IconRotation rotation);