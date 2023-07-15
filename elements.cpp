#include "elements.h"
#include "canvas.h"
#include "icon_i.h"
#include "canvas_i.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t leading_min;
    uint8_t leading_default;
    uint8_t height;
    uint8_t descender;
    uint8_t len;
    const char* text;
} ElementTextBoxLine;

void elements_progress_bar(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, float progress) {
    // assert(oled);
    // assert((progress >= 0) && (progress <= 1.0));
    uint8_t height = 9;

    uint8_t progress_length = roundf(progress * (width - 2));

    canvas_set_color(canvas, WHITE);
    canvas_draw_box(canvas, x + 1, y + 1, width - 2, height - 2);
    canvas_set_color(canvas, BLACK);
    canvas_draw_rframe(canvas, x, y, width, height, 3);
    canvas_draw_box(canvas, x + 1, y + 1, progress_length, height - 2);
}

void elements_progress_bar_with_text(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    float progress,
    const char* text) {
    uint8_t height = 11;

    uint8_t progress_length = roundf(progress * (width - 2));

    canvas_set_color(canvas, WHITE);
    canvas_draw_box(canvas, x + 1, y + 1, width - 2, height - 2);
    canvas_set_color(canvas, INVERSE);
    canvas_draw_rframe(canvas, x, y, width, height, 3);

    canvas_draw_box_invert(canvas, x + 1, y + 1, progress_length, height - 2);

    canvas_set_color(canvas, WHITE);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, x + width / 2, y + 2, AlignCenter, AlignTop, text);
}
uint8_t max(uint8_t a, uint8_t b) {
    return (a > b) ? a : b;
}
void elements_scrollbar_pos(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t height,
    uint16_t pos,
    uint16_t total) {
    // assert(oled);
    // assert(height > 2);
    // assert(pos <= max_pos);

    // prevent overflows
    canvas_set_color(canvas, WHITE);
    canvas_draw_box(canvas, x - 3, y, 3, height);
    // dot line
    canvas_set_color(canvas, BLACK);
    for(uint8_t i = y; i < height + y; i += 2) {
        canvas_draw_dot(canvas, x - 2, i);
    }
    // Position block
    if(total) {
        float block_h = ((float)height) / total;
        canvas_draw_box(canvas, x - 3, y + (block_h * pos), 3, max(block_h, 1));
    }
}

void elements_scrollbar(Canvas* canvas, uint16_t pos, uint16_t total) {
    // furi_assert(canvas);

    uint8_t width = 128; //canvas_width(canvas);
    uint8_t height = 64; //canvas_height(canvas);

    // Serial.print("width: ");
    // Serial.println(width);
    // Serial.print("height: ");
    // Serial.println(height);
    // Serial.print("offset_x: ");
    // Serial.println(canvas->offset_x);
    // Serial.print("offset_y: ");
    // Serial.println(canvas->offset_y);
    
    // prevent overflows
    // canvas_set_color(canvas, WHITE);
    // canvas_draw_box(canvas, width - 3, 0, 3, height);
    // dot line
    // canvas_set_color(canvas, WHITE);
    // for(uint8_t i = 0; i < height; i += 2) {
    //     canvas_draw_dot(canvas, width - 2, i);
    // }
    canvas_draw_bitmap(canvas, width - 8, 0, 8, 64, frames_scrollbar[0]);
    // Position block
    if(total) {
        float block_h = ((float)height) / total;
        canvas_draw_box_invert(canvas, width - 3, block_h * pos, 3, max(block_h, 1));
    }
}


void elements_text_box(Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    Align horizontal,
    Align vertical,
    const char* text,
    bool strip_to_dots) {
    // assert(oled);
    // assert(text);

    ElementTextBoxLine line[ELEMENTS_MAX_LINES_NUM];
    bool bold = false;
    bool mono = false;
    bool inversed = false;
    bool inversed_present = false;
    Font current_font = FontSecondary;
    Font prev_font = FontSecondary;
    const CanvasFontParameters* font_params = canvas_get_font_params(canvas, current_font);

    // Fill line parameters
    uint8_t line_leading_min = font_params->leading_min;
    uint8_t line_leading_default = font_params->leading_default;
    uint8_t line_height = font_params->height;
    uint8_t line_descender = font_params->descender;
    uint8_t line_num = 0;
    uint8_t line_width = 0;
    uint8_t line_len = 0;
    uint8_t total_height_min = 0;
    uint8_t total_height_default = 0;
    uint16_t i = 0;
    bool full_text_processed = false;
    uint16_t dots_width = canvas_string_width(canvas, "...");

    canvas_set_font(canvas, FontSecondary);

    // Fill all lines
    line[0].text = text;
    for(i = 0; !full_text_processed; i++) {
        line_len++;
        // Identify line height
        if(prev_font != current_font) {
            font_params = canvas_get_font_params(canvas, current_font);
            line_leading_min = max(line_leading_min, font_params->leading_min);
            line_leading_default = max(line_leading_default, font_params->leading_default);
            line_height = max(line_height, font_params->height);
            line_descender = max(line_descender, font_params->descender);
            prev_font = current_font;
        }
        // Set the font
        if(text[i] == '\e' && text[i + 1]) {
            i++;
            line_len++;
            if(text[i] == ELEMENTS_BOLD_MARKER) {
                if(bold) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontPrimary;
                }
                canvas_set_font(canvas, current_font);
                bold = !bold;
            }
            if(text[i] == ELEMENTS_MONO_MARKER) {
                if(mono) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontKeyboard;
                }
                canvas_set_font(canvas, FontKeyboard);
                mono = !mono;
            }
            if(text[i] == ELEMENTS_INVERSED_MARKER) {
                inversed_present = true;
            }
            continue;
        }
        if(text[i] != '\n') {
            line_width += canvas_glyph_width(canvas, text[i]);
        }
        // Process new line
        if(text[i] == '\n' || text[i] == '\0' || line_width > width) {
            if(line_width > width) {
                line_width -= canvas_glyph_width(canvas, text[i--]);
                line_len--;
            }
            if(text[i] == '\0') {
                full_text_processed = true;
            }
            if(inversed_present) {
                line_leading_min += 1;
                line_leading_default += 1;
                inversed_present = false;
            }
            line[line_num].leading_min = line_leading_min;
            line[line_num].leading_default = line_leading_default;
            line[line_num].height = line_height;
            line[line_num].descender = line_descender;
            if(total_height_min + line_leading_min > height) {
                break;
            }
            total_height_min += line_leading_min;
            total_height_default += line_leading_default;
            line[line_num].len = line_len;
            if(horizontal == AlignCenter) {
                line[line_num].x = x + (width - line_width) / 2;
            } else if(horizontal == AlignRight) {
                line[line_num].x = x + (width - line_width);
            } else {
                line[line_num].x = x;
            }
            line[line_num].y = total_height_min;
            line_num++;
            if(text[i + 1]) {
                line[line_num].text = &text[i + 1];
            }

            line_leading_min = font_params->leading_min;
            line_height = font_params->height;
            line_descender = font_params->descender;
            line_width = 0;
            line_len = 0;
        }
    }

    // Set vertical alignment for all lines
    if(total_height_default < height) {
        if(vertical == AlignTop) {
            line[0].y = y + line[0].height;
        } else if(vertical == AlignCenter) {
            line[0].y = y + line[0].height + (height - total_height_default) / 2;
        } else if(vertical == AlignBottom) {
            line[0].y = y + line[0].height + (height - total_height_default);
        }
        if(line_num > 1) {
            for(uint8_t i = 1; i < line_num; i++) {
                line[i].y = line[i - 1].y + line[i - 1].leading_default;
            }
        }
    } else if(line_num > 1) {
        uint8_t free_pixel_num = height - total_height_min;
        uint8_t fill_pixel = 0;
        uint8_t j = 1;
        line[0].y = y + line[0].height;
        while(fill_pixel < free_pixel_num) {
            line[j].y = line[j - 1].y + line[j - 1].leading_min + 1;
            fill_pixel++;
            j = j % (line_num - 1) + 1;
        }
    }

    // Draw line by line
    canvas_set_font(canvas, FontSecondary);
    bold = false;
    mono = false;
    inversed = false;
    for(uint8_t i = 0; i < line_num; i++) {
        for(uint8_t j = 0; j < line[i].len; j++) {
            // Process format symbols
            if(line[i].text[j] == ELEMENTS_BOLD_MARKER) {
                if(bold) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontPrimary;
                }
                canvas_set_font(canvas, current_font);
                bold = !bold;
                continue;
            }
            if(line[i].text[j] == ELEMENTS_MONO_MARKER) {
                if(mono) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontKeyboard;
                }
                canvas_set_font(canvas, current_font);
                mono = !mono;
                continue;
            }
            if(line[i].text[j] == ELEMENTS_INVERSED_MARKER) {
                inversed = !inversed;
                continue;
            }
            if(inversed) {
                canvas_draw_box(
                    canvas,
                    line[i].x - 1,
                    line[i].y - line[i].height - 1,
                    canvas_glyph_width(canvas, line[i].text[j]) + 1,
                    line[i].height + line[i].descender + 2);
                canvas_invert_color(canvas);
                canvas_draw_glyph(canvas, line[i].x, line[i].y, line[i].text[j]);
                canvas_invert_color(canvas);
            } else {
                if((i == line_num - 1) && strip_to_dots) {
                    uint8_t next_symbol_width = canvas_glyph_width(canvas, line[i].text[j]);
                    if(line[i].x + next_symbol_width + dots_width > x + width) {
                        canvas_draw_str(canvas, line[i].x, line[i].y, "...");
                        break;
                    }
                }
                canvas_draw_glyph(canvas, line[i].x, line[i].y, line[i].text[j]);
            }
            line[i].x += canvas_glyph_width(canvas, line[i].text[j]);
        }
    }
    canvas_set_font(canvas, FontSecondary);
}

void elements_frame(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // furi_assert(canvas);
    canvas_draw_line(canvas, x + 2, y, x + width - 2, y);
    canvas_draw_line(canvas, x + 1, y + height - 1, x + width, y + height - 1);
    canvas_draw_line(canvas, x + 2, y + height, x + width - 1, y + height);

    canvas_draw_line(canvas, x, y + 2, x, y + height - 2);
    canvas_draw_line(canvas, x + width - 1, y + 1, x + width - 1, y + height - 2);
    canvas_draw_line(canvas, x + width, y + 2, x + width, y + height - 2);

    canvas_draw_dot(canvas, x + 1, y + 1);
}


void elements_string_fit_width(Canvas* canvas, FuriString* string, uint8_t width) {
    uint16_t len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
    if(len_px > width) {
        width -= canvas_string_width(canvas, "...");
        do {
            furi_string_left(string, furi_string_size(string) - 1);
            len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
        } while(len_px > width);
        furi_string_cat_str(string, "...");
    }
}

void elements_slightly_rounded_box(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height) {
    // furi_assert(canvas);
    canvas_draw_rbox(canvas, x, y, width, height, 1);
}