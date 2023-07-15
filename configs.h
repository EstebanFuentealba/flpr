#include <FS.h>
#include <functional>
#include <LinkedList.h>
#include "SPIFFS.h"
#include "Assets.h"


#ifndef configs_h
#define configs_h

  //Indicates that it must redirect the stream with the captured packets to serial (1)
  //If not defined, will write packages to SD card if supported
  // #define WRITE_PACKETS_SERIAL

#define GENERIC_ESP32 
#define HAS_BUTTONS 


// Keypad start position, key sizes and spacing
#define KEY_X 120 // Centre of key
#define KEY_Y 50
#define KEY_W 128 // Width and height
#define KEY_H 22
#define KEY_SPACING_X 0 // X and Y gap
#define KEY_SPACING_Y 1
#define KEY_TEXTSIZE 1   // Font size multiplier
#define ICON_W 22
#define ICON_H 22
#define BUTTON_PADDING 22


#define L_BTN 16  
#define C_BTN 27
#define U_BTN 25
#define R_BTN 26
#define D_BTN 32

#define MARAUDER_VERSION "v0.10.7"



//// DISPLAY DEFINITIONS
//  TODO
#define BUTTON_ARRAY_LEN 5
#define BANNER_TIME 100
//#define SCREEN_WIDTH_PX 128
//#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define TFT_WIDTH 128
#define TFT_HEIGHT 64

#define CHAR_WIDTH 6
#define HEIGHT_1 TFT_HEIGHT
#define WIDTH_1 TFT_WIDTH
#define STANDARD_FONT_CHAR_LIMIT (TFT_WIDTH/6) // number of characters on a single line with normal font
#define TEXT_HEIGHT (TFT_HEIGHT/10) // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0 // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 48 // Number of lines in top fixed area (lines counted from top of screen)
#define YMAX TFT_HEIGHT // Bottom of screen area
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
//#define MENU_FONT NULL
#define MENU_FONT &FreeMono9pt7b // Winner
//#define MENU_FONT &FreeMonoBold9pt7b
//#define MENU_FONT &FreeSans9pt7b
//#define MENU_FONT &FreeSansBold9pt7b
//#define BUTTON_ARRAY_LEN 11
#define STATUS_BAR_WIDTH (TFT_HEIGHT/16)
#define LVGL_TICK_PERIOD 6

#define FRAME_X 100
#define FRAME_Y 64
#define FRAME_W 120
#define FRAME_H 50


//// END MENU DEFINITIONS


#ifdef GENERIC_ESP32
  // #define HAS_BT
  // #define HAS_SCREEN
  #define SD_CS 5
#endif
//// END SCREEN STUFF




#endif
