#include "canvas_i.h"
#include "icon_i.h"
#include "icon_animation_i.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "muMatrix8ptRegular.h"

const CanvasFontParameters canvas_font_params[FontTotalNumber] = {
    [FontPrimary] = {.leading_default = 12, .leading_min = 11, .height = 8, .descender = 2},
    [FontSecondary] = {.leading_default = 11, .leading_min = 9, .height = 7, .descender = 2},
    [FontKeyboard] = {.leading_default = 11, .leading_min = 9, .height = 7, .descender = 2},
    [FontBigNumbers] = {.leading_default = 18, .leading_min = 16, .height = 15, .descender = 0},
};



// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(SSD1306_SWITCHCAPVCC, /* cs=/ 5, / dc=/ 32, / reset=*/ 4);
 Canvas* canvas_init() {
    Canvas* canvas = new Canvas;
    
    canvas->compress_icon = compress_icon_alloc();
    canvas->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    // canvas->display = new u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE);
    // u8g2.begin();
    // Wire.begin();
    canvas->offset_x = 0;
    canvas->offset_y = 0;
    canvas->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // canvas->fb->clearDisplay();
    // canvas->display->setTextColor(WHITE);
    // canvas->fb->setCursor(128/2, 64/2);
    // canvas->fb->println(F("Hola Mundo"));
    // canvas->display->display();

    // Clear buffer and send to device
    canvas_clear(canvas);
    canvas_commit(canvas);
    // canvas_draw_str_aligned(canvas, 128/2, 64/2,  AlignCenter, AlignTop, "Hola Mundo");

    return canvas;
}

void canvas_reset(Canvas* canvas) {
    // canvas_clear(canvas);
    // canvas_set_color(canvas, ColorBlack);
    // canvas_set_font(canvas, FontSecondary);
    // canvas_set_font_direction(canvas, CanvasDirectionLeftToRight);

    // display->clearDisplay();
    // display->setTextColor(SSD1306_WHITE);
    // display->setTextWrap(false);
}

void canvas_commit(Canvas* canvas) {
    canvas->display->display();
}

void canvas_clear(Canvas* canvas) {
    canvas->display->clearDisplay();
}

void canvas_set_color(Canvas* canvas, uint16_t color) {
    canvas->display->setTextColor(color);
}
void canvas_set_color_white(Canvas* canvas) {
    canvas->display->setTextColor(WHITE);
}
void canvas_set_font(Canvas* canvas, Font font) {
    canvas->display->setFont(&muMatrix8ptRegular);
    canvas->display->setTextSize(1);
    // if (font == FontPrimary) {
    //     canvas->display->setTextSize(0.5);
    // } else if (font == FontSecondary) {
    //     canvas->display->setTextSize(1);
    // } else if (font == FontKeyboard) {
    //     canvas->display->setTextSize(1);
    // } else if (font == FontBigNumbers) {
    //     canvas->display->setTextSize(2);
    // } else {
    //     // Manejar el error de fuente desconocida
    // }
}

void canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str) {
    if (!str) {
        return;
    }
    canvas->display->setCursor(x, y);
    canvas->display->print(str);
}

void canvas_draw_str_aligned(Canvas* canvas, uint8_t x, uint8_t y, Align horizontal, Align vertical, const char* str) {
    if (!str) {
        return;
    }

    uint8_t strWidth = strlen(str) * (6 * 1); // 6 is the character width in pixels
    uint8_t strHeight = 8;

    switch (horizontal) {
        case AlignLeft:
            break;
        case AlignRight:
            x -= strWidth;
            break;
        case AlignCenter:
            x -= strWidth / 2;
            break;
        default:
            return;
    }

    switch (vertical) {
        case AlignTop:
            break;
        case AlignBottom:
            y -= strHeight;
            break;
        case AlignCenter:
            y -= strHeight / 2;
            break;
        default:
            return;
    }

    canvas->display->setCursor(x, y);
    canvas->display->print(str);
}

uint16_t canvas_string_width(Canvas* canvas, const char* str) {
    if (!str) return 0;
    int16_t x, y; // Variáveis para armazenar as coordenadas do retângulo delimitador
    uint16_t w, h; // Variáveis para armazenar a largura e altura do retângulo delimitador
    canvas->display->getTextBounds(str, 0, 0, &x, &y, &w, &h);

    return w;
}

// void canvas_draw_bitmap(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* bitmap) {
//     canvas->fb.display->drawBitmap( x, y, bitmap, width, height, WHITE);
// }

// void canvas_draw_icon_animation(Canvas* canvas, uint8_t x, uint8_t y, const IconAnimation* icon_animation) {
//     if (!icon_animation) {
//         return;
//     }
//     x += canvas->offset_x;
//     y += canvas->offset_y;
//     uint8_t* icon_data = NULL;
//     compress_icon_decode(canvas->compress_icon, icon_animation_get_data(icon_animation), &icon_data);
//     canvas_draw_bitmap(canvas, x, y, icon_animation_get_width(icon_animation), icon_animation_get_height(icon_animation), icon_data);
// }

void canvas_draw_icon(Canvas* canvas, uint8_t x, uint8_t y, const Icon* icon) {
    if (!icon) {
        return;
    }
     x += canvas->offset_x;
    y += canvas->offset_y;
    uint8_t* icon_data = NULL;
    compress_icon_decode(canvas->compress_icon, icon_get_data(icon), &icon_data);
    // canvas_draw_bitmap(canvas, x, y, icon->width, icon->height, icon_data);
    canvas_draw_u8g2_bitmap(canvas, x, y, icon_get_width(icon), icon_get_height(icon), icon_data, IconRotation0);
}

static void canvas_draw_u8g2_bitmap_int(
    Adafruit_SSD1306* display,
    int16_t x,
    int16_t y,
    uint16_t w,
    uint16_t h,
    bool mirror,
    bool rotation,
    const uint8_t* bitmap) {

    int16_t blen = w;
    blen += 7;
    blen >>= 3;

    if(rotation && !mirror) {
        x += w + 1;
    } else if(mirror && !rotation) {
        y += h - 1;
    }

    while(h > 0) {
        const uint8_t* b = bitmap;
        uint16_t len = w;
        int16_t x0 = x;
        int16_t y0 = y;
        uint8_t mask;
        uint16_t color = WHITE; // Set the color you want to use for drawing
        uint16_t ncolor = BLACK; // Set the color for the background
        mask = 1;

        while(len > 0) {
            if(pgm_read_byte(b) & mask) {
                display->drawPixel(x0, y0, color);
            } else {
                if(display->getRotation() == 0) {
                    display->drawPixel(x0, y0, ncolor);
                }
            }

            if(rotation) {
                y0++;
            } else {
                x0++;
            }

            mask <<= 1;
            if(mask == 0) {
                mask = 1;
                b++;
            }
            len--;
        }

        bitmap += blen;

        if(mirror) {
            if(rotation) {
                x++;
            } else {
                y--;
            }
        } else {
            if(rotation) {
                x--;
            } else {
                y++;
            }
        }
        h--;
    }
}


void canvas_draw_u8g2_bitmap(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t w,
    uint8_t h,
    const uint8_t* bitmap,
    IconRotation rotation) {
    uint8_t blen;
    blen = w;
    blen += 7;
    blen >>= 3;
// #ifdef U8G2_WITH_INTERSECTION
//     if(u8g2_IsIntersection(u8g2, x, y, x + w, y + h) == 0) return;
// #endif /* U8G2_WITH_INTERSECTION */

    switch(rotation) {
    case IconRotation0:
        canvas_draw_u8g2_bitmap_int(canvas->display, x, y, w, h, 0, 0, bitmap);
        break;
    case IconRotation90:
        canvas_draw_u8g2_bitmap_int(canvas->display, x, y, w, h, 0, 1, bitmap);
        break;
    case IconRotation180:
        canvas_draw_u8g2_bitmap_int(canvas->display, x, y, w, h, 1, 0, bitmap);
        break;
    case IconRotation270:
        canvas_draw_u8g2_bitmap_int(canvas->display, x, y, w, h, 1, 1, bitmap);
        break;
    default:
        break;
    }
}


void canvas_draw_box(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->drawRect(x, y, width, height, WHITE);
    // u8g2_DrawBox(&canvas->fb, x, y, width, height);
}
void canvas_draw_box_invert(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->fillRect(x, y, width, height, WHITE);
    // u8g2_DrawBox(&canvas->fb, x, y, width, height);
}

void canvas_draw_circle(Canvas* canvas, uint8_t x, uint8_t y, uint8_t radius) {
    // furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->drawCircle(x, y, radius, INVERSE);
    // u8g2_DrawCircle(&canvas->fb, x, y, radius, U8G2_DRAW_ALL);
}

void canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y) {
    // furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->drawRect(x, y, 1, 1, WHITE);
    // u8g2_DrawPixel(&canvas->fb, x, y);
}
void canvas_draw_line(Canvas* canvas, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    // furi_assert(canvas);
    // x1 += canvas->offset_x;
    // y1 += canvas->offset_y;
    // x2 += canvas->offset_x;
    // y2 += canvas->offset_y;
    canvas->display->drawLine(x1, y1, x2, y2, WHITE);
    // canvas_commit(canvas);
    // u8g2_DrawLine(&canvas->fb, x1, y1, x2, y2);
}


void canvas_draw_frame(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // furi_assert(canvas);
    uint8_t x1 = x + canvas->offset_x;
    uint8_t y1 = y + canvas->offset_y;
    uint8_t x2 = x + width;
    uint8_t y2 = y1; 
    canvas->display->drawLine(x1, y1, x2, y2, WHITE);
    x1 = x2;
    y1 = y2;
    x2 = x2;
    y2 = y1 + height; 
    canvas->display->drawLine(x1, y1, x2, y2, WHITE);

    x1 = x + canvas->offset_x;
    y1 = y + canvas->offset_y + height;
    x2 = x2;
    y2 = y + canvas->offset_y + height; 
    canvas->display->drawLine(x1, y1, x2, y2, WHITE);


    x1 = x + canvas->offset_x;
    y1 = y + canvas->offset_y + height;
    x2 =  x + canvas->offset_x;
    y2 =  y + canvas->offset_y;
    canvas->display->drawLine(x1, y1, x2, y2, WHITE);
}

void canvas_draw_disc(Canvas* canvas, uint8_t x, uint8_t y, uint8_t radius) {
    // furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->drawCircle(x, y, radius, INVERSE);
}

void canvas_draw_rframe(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius) {
    
    // Dibuja el marco del rectángulo con esquinas redondeadas
    canvas->display->drawRoundRect(x, y, width, height, radius, WHITE);
    
}
void canvas_draw_rframe_fill(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius) {
    
    // Dibuja el marco del rectángulo con esquinas redondeadas
    canvas->display->drawRoundRect(x, y, width, height, radius, INVERSE);
    
}
uint8_t canvas_width(const Canvas* canvas) {
    // furi_assert(canvas);
    return canvas->width;
}

uint8_t canvas_height(const Canvas* canvas) {
    // furi_assert(canvas);
    return canvas->height;
}

// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #include "canvas_i.h"
// #include "icon_i.h"
// #include "icon_animation_i.h"

// // #include <splash.h>



// const uint8_t canvas_font_params[][4] = {
//     {12, 11, 8, 2},    // FontPrimary
//     {11, 9, 7, 2},     // FontSecondary
//     {11, 9, 7, 2},     // FontKeyboard
//     {18, 16, 15, 0},   // FontBigNumbers
// };

//  Canvas* canvas_init() {
//    Canvas* canvas = new Canvas;
//     // canvas->compress_icon = compress_icon_alloc();
//     canvas->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);;
//     // Wire.begin();
//     canvas->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
//     // canvas->display->clearDisplay();
//     // canvas->display->display();

//     // Clear buffer and send to device
//     canvas_clear(canvas);
//     canvas_commit(canvas);

//     return canvas;
// }

// void canvas_reset(Canvas* canvas) {
//     canvas_clear(canvas);
//     canvas_set_color(canvas, ColorBlack);
//     canvas_set_font(canvas, FontSecondary);
//     canvas_set_font_direction(canvas, CanvasDirectionLeftToRight);

//     // display->clearDisplay();
//     // display->setTextColor(SSD1306_WHITE);
//     // display->setTextWrap(false);
// }

// void canvas_commit(Canvas* canvas) {
//     display->display();
// }

// void canvas_clear(Canvas* canvas) {
//     display->clearDisplay();
// }

// void canvas_set_color(Canvas* canvas, uint16_t color) {
//     display->setTextColor(color);
// }

// void canvas_set_font(Canvas* canvas, uint8_t font) {
//     if (font >= FontTotalNumber) {
//         return;
//     }

//     display->setTextSize(1);
//     display->setFont();
// }

// void canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str) {
//     if (!str) {
//         return;
//     }

//     display->setCursor(x, y);
//     display->print(str);
// }

// void canvas_draw_str_aligned(Canvas* canvas, uint8_t x, uint8_t y, uint8_t horizontal, uint8_t vertical, const char* str) {
//     if (!str) {
//         return;
//     }

//     uint8_t strWidth = strlen(str) * (6 * 1); // 6 is the character width in pixels
//     uint8_t strHeight = 8;

//     switch (horizontal) {
//         case AlignLeft:
//             break;
//         case AlignRight:
//             x -= strWidth;
//             break;
//         case AlignCenter:
//             x -= strWidth / 2;
//             break;
//         default:
//             return;
//     }

//     switch (vertical) {
//         case AlignTop:
//             break;
//         case AlignBottom:
//             y -= strHeight;
//             break;
//         case AlignCenter:
//             y -= strHeight / 2;
//             break;
//         default:
//             return;
//     }

//     display->setCursor(x, y);
//     display->print(str);
// }

// uint16_t canvas_string_width(Canvas* canvas, const char* str) {
//     if (!str) {
//         return 0;
//     }

//     return strlen(str) * (6 * 1); // 6 is the character width in pixels
// }

void canvas_draw_bitmap(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* bitmap) {
    canvas->display->drawBitmap( x, y, bitmap, width, height, WHITE);
}
void canvas_frame_set(
    Canvas* canvas,
    uint8_t offset_x,
    uint8_t offset_y,
    uint8_t width,
    uint8_t height) {
    // furi_assert(canvas);
    canvas->offset_x = offset_x;
    canvas->offset_y = offset_y;
    canvas->width = width;
    canvas->height = height;
}
void canvas_set_bitmap_mode(Canvas* canvas, bool alpha) {
    // Serial.println("TODO: canvas_set_bitmap_mode");
    // canvas->display->bitmapModeAlpha = alpha;
}
size_t canvas_get_buffer_size(const Canvas* canvas) {
    // furi_assert(canvas);
    // return u8g2_GetBufferTileWidth(&canvas->fb) * u8g2_GetBufferTileHeight(&canvas->fb) * 8;
    return canvas->display->width() * canvas->display->height() / 8;
}

void canvas_set_orientation(Canvas* canvas, CanvasOrientation orientation) {
    const uint8_t* rotate_cb = NULL;
    bool need_swap = false;

    if (canvas->orientation != orientation) {
        switch (orientation) {
            case CanvasOrientationHorizontal:
                need_swap = canvas->orientation == CanvasOrientationVertical ||
                            canvas->orientation == CanvasOrientationVerticalFlip;
                rotate_cb = (const uint8_t*)0;
                break;
            case CanvasOrientationHorizontalFlip:
                need_swap = canvas->orientation == CanvasOrientationVertical ||
                            canvas->orientation == CanvasOrientationVerticalFlip;
                rotate_cb = (const uint8_t*)2;
                break;
            case CanvasOrientationVertical:
                need_swap = canvas->orientation == CanvasOrientationHorizontal ||
                            canvas->orientation == CanvasOrientationHorizontalFlip;
                rotate_cb = (const uint8_t*)3;
                break;
            case CanvasOrientationVerticalFlip:
                need_swap = canvas->orientation == CanvasOrientationHorizontal ||
                            canvas->orientation == CanvasOrientationHorizontalFlip;
                rotate_cb = (const uint8_t*)1;
                break;
            default:
                // Error case
                return;
        }

        if (need_swap) {
            uint16_t temp = canvas->width;
            canvas->width = canvas->height;
            canvas->height = temp;
        }

        canvas->display->setRotation(orientation);
        canvas->orientation = orientation;
    }
}


CanvasOrientation canvas_get_orientation(const Canvas* canvas) {
    return canvas->orientation;
}

uint8_t* canvas_get_buffer(Canvas* canvas) {
    // furi_assert(canvas);
    return canvas->display->getBuffer();
}

void canvas_draw_rbox(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius) {
    // assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->display->fillRoundRect(x, y, width, height, radius, INVERSE);
}

void canvas_draw_fast_v_line(Canvas* canvas, uint8_t x, uint8_t y, uint8_t height) {
    canvas->display->drawFastVLine(x, y, height, WHITE);
}

void canvas_draw_fast_h_line(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width){
    canvas->display->drawFastHLine(x, y, width, WHITE);
}

void canvas_draw_icon_animation(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    IconAnimation* icon_animation) {
    // furi_assert(canvas);
    // furi_assert(icon_animation);

    x += canvas->offset_x;
    y += canvas->offset_y;
    uint8_t* icon_data = NULL;
    compress_icon_decode(canvas->compress_icon, icon_animation_get_data(icon_animation), &icon_data);
    canvas_draw_bitmap(canvas, x, y, icon_animation_get_width(icon_animation), icon_animation_get_height(icon_animation), icon_data);
}

// void canvas_draw_icon_animation(Canvas* canvas, uint8_t x, uint8_t y, const IconAnimation* icon_animation) {
//     if (!icon_animation) {
//         return;
//     }
//     x += canvas->offset_x;
//     y += canvas->offset_y;
//     uint8_t* icon_data = NULL;
//     compress_icon_decode(canvas->compress_icon, icon_animation_get_data(icon_animation), &icon_data);
//     canvas_draw_bitmap(canvas, x, y, icon_animation_get_width(icon_animation), icon_animation_get_height(icon_animation), icon_data);
// }

// void canvas_draw_icon(Canvas* canvas, uint8_t x, uint8_t y, const Icon* icon) {
//     if (!icon) {
//         return;
//     }

//     canvas_draw_bitmap(canvas, x, y, icon_get_width(icon), icon_get_height(icon), icon_get_data(icon));
// }
void canvas_draw_icon_ex(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    const Icon* icon,
    IconRotation rotation) {
    // furi_assert(canvas);
    // furi_assert(icon);

    x += canvas->offset_x;
    y += canvas->offset_y;
    uint8_t* icon_data = NULL;
    compress_icon_decode(canvas->compress_icon, icon_get_data(icon), &icon_data);
    canvas_draw_u8g2_bitmap(canvas, x, y, icon_get_width(icon), icon_get_height(icon), icon_data, rotation);
}
uint8_t canvas_current_font_height(const Canvas* canvas) {
   // Adafruit_SSD1306 doesn't have a direct function to get font height,
    // so we need to use a workaround using the getTextBounds() function.

    // Backup the current font and set a known font (to ensure consistent results).
    // const GFXfont* original_font = canvas->display->getFont();
    canvas->display->setTextSize(1); // Set a small text size to get accurate bounds.
    canvas->display->setFont();      // Use the default font.

    // Get the bounding box of a capital 'M' character (since it usually has the maximum height).
    int16_t x, y;
    uint16_t w, h;
    canvas->display->getTextBounds("M", 0, 0, &x, &y, &w, &h);

    // Restore the original font.
    canvas->display->setFont(&muMatrix8ptRegular);

    // Add 1 to the height if using a specific font.
    // if (original_font == &muMatrix8ptRegular) {
    //     h += 1;
    // }

    return h;
}
void canvas_invert_color(Canvas* canvas) {
    //  TODO: Implement
    // canvas->display->draw_color = !canvas->display->draw_color;
}