#include "Display.h"
#include "lang_var.h"
#include "canvas_i.h"


#ifdef HAS_SCREEN

Display::Display()
{
}

// Function to prepare the display and the menus
void Display::RunSetup()
{
  run_setup = false;

  // Need to declare new
  display_buffer = new LinkedList<String>();

#ifdef SCREEN_BUFFER
  screen_buffer = new LinkedList<String>();
#endif

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  // Your setup code here...

  display.display();
}

void Display::drawFrame()
{
  display.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, SSD1306_BLACK);
}


void Display::tftDrawGraphObjects(byte x_scale)
{
  //draw the graph objects
  display.fillRect(11, 5, x_scale + 1, 120, SSD1306_BLACK); // positive start point
  display.fillRect(11, 121, x_scale + 1, 119, SSD1306_BLACK); // negative start point
  display.drawFastVLine(10, 5, 230, SSD1306_WHITE); // y axis
  display.drawFastHLine(10, HEIGHT_1 - 1, 310, SSD1306_WHITE); // x axis
  display.setTextColor(SSD1306_WHITE); 
  display.setTextSize(1); // set parameters for y axis labels
  display.setCursor(3, 6); display.println("+"); // "+" at top of y axis
  display.setCursor(3, 228); display.println("0"); // "-" at bottom of y axis
}

void Display::tftDrawEapolColorKey()
{
  //Display color key
  display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.fillRect(14, 0, 15, 8, WHITE); 
  display.setCursor(30, 0); display.println(" - EAPOL");
}

void Display::tftDrawColorKey()
{
  //Display color key
  display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.fillRect(14, 0, 15, 8, WHITE); display.setCursor(30, 0); display.println(" - Beacons");
  display.fillRect(14, 8, 15, 8, WHITE); display.setCursor(30, 8); display.println(" - Deauths");
  display.fillRect(14, 16, 15, 8, WHITE); display.setCursor(30, 16); display.println(" - Probes");
}

void Display::tftDrawXScaleButtons(byte x_scale)
{
  display.drawFastVLine(234, 0, 20, SSD1306_WHITE);
  display.setCursor(208, 21); display.setTextColor(SSD1306_WHITE); display.setTextSize(1); display.println("X Scale:"); display.println(x_scale);

  key[0].initButton(&display, // x - box
    220,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "-",
    2);
  key[1].initButton(&display, // x + box
    249,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "+",
    2);

  //key[0].setLabelDatum(1, 5, MC_DATUM);
  //key[1].setLabelDatum(1, 5, MC_DATUM);

  key[0].drawButton(&display);
  key[1].drawButton(&display);
}

void Display::tftDrawYScaleButtons(byte y_scale)
{
  display.drawFastVLine(290, 0, 20, SSD1306_WHITE);
  display.setCursor(265, 21); display.setTextColor(SSD1306_WHITE); display.setTextSize(1); display.println("Y Scale:"); display.println(y_scale);

  key[2].initButton(&display, // y - box
    276,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "-",
    2);
  key[3].initButton(&display, // y + box
    305,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "+",
    2);

  //ey[2].setLabelDatum(1, 5, MC_DATUM);
  //key[3].setLabelDatum(1, 5, MC_DATUM);

  key[2].drawButton(&display);
  key[3].drawButton(&display);
}

void Display::tftDrawChannelScaleButtons(int set_channel)
{
  display.drawFastVLine(178, 0, 20, SSD1306_WHITE);
  display.setCursor(145, 21); display.setTextColor(SSD1306_WHITE); display.setTextSize(1); display.println(text10); display.println(set_channel);

  key[4].initButton(&display, // channel - box
    164,
10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "-",
    2);
  key[5].initButton(&display, // channel + box
    193,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    SSD1306_BLACK, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "+",
    2);

  //key[4].setLabelDatum(1, 5, MC_DATUM);
  //key[5].setLabelDatum(1, 5, MC_DATUM);

  key[4].drawButton(&display);
  key[5].drawButton(&display);
}

void Display::tftDrawExitScaleButtons()
{
  key[6].initButton(&display, // Exit box
    137,
    10, // x, y, w, h, outline, fill, text
    20,
    20,
    WHITE, // Outline
    WHITE, // Fill
    SSD1306_BLACK, // Text
    "X",
    2);

  //key[6].setLabelDatum(1, 5, MC_DATUM);

  key[6].drawButton(&display);
}

void Display::twoPartDisplay(String center_text)
{
  display.setTextColor(SSD1306_BLACK, WHITE);
  display.fillRect(0, 16, HEIGHT_1, 144, WHITE);
  display.setTextWrap(true);
  //display.setFreeFont(NULL);
  display.setCursor(0, 82);
  display.println(center_text);
  //display.setFreeFont(MENU_FONT);
  display.setTextWrap(false);
}

void Display::touchToExit()
{
  display.setTextColor(SSD1306_BLACK, WHITE);
  display.fillRect(0, 32, HEIGHT_1, 16, WHITE);
  //display.drawCentreString(text11, 120, 32, 2);

  display.setCursor((display.width() - (strlen(text11) * 6)) / 2, 32);
  display.print(text11);
}

// Function to just draw the screen black
void Display::clearScreen()
{
  display.clearDisplay();
  display.display();
}

#ifdef SCREEN_BUFFER
void Display::scrollScreenBuffer(bool down) {
  // Scroll screen normal direction (Up)
  if (!down) {
    this->screen_buffer->shift();
  }
}
#endif

void Display::displayBuffer(bool do_clear)
{
  if (this->display_buffer->size() > 0)
  {
    delay(1);

    while (display_buffer->size() > 0)
    {

#ifndef SCREEN_BUFFER
      xPos = 0;
      if ((display_buffer->size() > 0) && (!loading))
      {
        printing = true;
        delay(print_delay_1);
        yDraw = scroll_line(WHITE);
        display.setCursor(xPos, yDraw);
        display.setTextColor(WHITE, SSD1306_BLACK);
        display.println(display_buffer->shift());
        printing = false;
        delay(print_delay_2);
      }
      if (!tteBar)
        blank[(18 + (yStart - TOP_FIXED_AREA) / TEXT_HEIGHT) % 19] = xPos;
      else
        blank[(18 + (yStart - TOP_FIXED_AREA_2) / TEXT_HEIGHT) % 19] = xPos;
#else
      xPos = 0;
      if (this->screen_buffer->size() >= MAX_SCREEN_BUFFER)
        this->scrollScreenBuffer();

      screen_buffer->add(display_buffer->shift());

      for (int i = 0; i < this->screen_buffer->size(); i++) {
        display.setCursor(xPos, (i * 12) + (64 / 6));
        for (int x = 0; x < SSD1306_WIDTH / CHAR_WIDTH; x++)
          display.println(" ");
        display.setCursor(xPos, (i * 12) + (64 / 6));
        display.setTextColor(SSD1306_GREEN, SSD1306_BLACK);
        display.println(this->screen_buffer->get(i));
      }
#endif
    }
  }
}

void Display::showCenterText(String text, int y)
{
  display.setCursor((128 - (text.length() * 6)) / 2, y);
  display.println(text);
}

void Display::initScrollValues(bool tte)
{
  yDraw = yStart; // Store the old yStart, this is where we draw the next line
  // Use the record of line lengths to optimise the rectangle size we need to erase the top line

  // Check if we have the "touch to exit bar"
  if (!tte)
  {
    yStart = TOP_FIXED_AREA;

    yArea = YMAX - TOP_FIXED_AREA - BOT_FIXED_AREA;
  }
  else
  {
    yStart = TOP_FIXED_AREA_2;

    yArea = YMAX - TOP_FIXED_AREA_2 - BOT_FIXED_AREA;
  }

  for (uint8_t i = 0; i < 18; i++) blank[i] = 0;
}

// Function to execute hardware scroll for TFT screen
int Display::scroll_line(uint32_t color) {
  int yTemp = yStart; // Store the old yStart, this is where we draw the next line
  // Use the record of line lengths to optimise the rectangle size we need to erase the top line

  // Check if we have the "touch to exit bar"
  if (!tteBar)
  {
    display.fillRect(0, yStart, blank[(yStart - TOP_FIXED_AREA) / TEXT_HEIGHT], TEXT_HEIGHT, color);

    // Change the top of the scroll area
    yStart += TEXT_HEIGHT;
    // The value must wrap around asthe screen memory is a circular buffer
  if (yStart >= YMAX - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA + (yStart - YMAX + BOT_FIXED_AREA);
}
  else
  {
    display.fillRect(0, yStart, blank[(yStart - TOP_FIXED_AREA_2) / TEXT_HEIGHT], TEXT_HEIGHT, color);

    // Change the top of the scroll area
    yStart += TEXT_HEIGHT;
    // The value must wrap around as the screen memory is a circular buffer
    if (yStart >= YMAX - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA_2 + (yStart - YMAX + BOT_FIXED_AREA);
  }
  // Now we can scroll the display
  scrollAddress(yStart);
  return yTemp;
}

// Function to setup hardware scroll for TFT screen
void Display::setupScrollArea(uint16_t tfa, uint16_t bfa) {
  display.startscrollleft(tfa, bfa);
}

void Display::scrollAddress(uint16_t vsp) {
  display.startscrollleft(vsp, 0);
}

void Display::setupDraw() {
  display.drawLine(0, 0, 10, 0, WHITE);
  display.drawLine(0, 0, 0, 10, WHITE);
  display.drawLine(0, 0, 0, 0, WHITE);
}

uint16_t xlast;
uint16_t ylast;
uint32_t AH;

void Display::drawStylus()
{
  uint16_t x = 0, y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  boolean pressed = false; //display.getTouch(&x, &y);

  if ((x <= 10) && (y <= 10) && (pressed)) {
    this->draw_tft = false;
    this->exit_draw = true;
    return;
  }

  // Draw a white spot at the detected coordinates
  if (pressed) {
    if (xlast > 0 && ylast > 0) {
      uint16_t the_color = SSD1306_WHITE;
      uint16_t wd = 1;
      int xlast2;
      int ylast2;
      int x2;
      int y2;
      int n;
      int n2 = -wd;
      xlast2 = xlast - wd;
      x2 = x - wd;
      for (n = -wd; n <= wd; n++) {
        ylast2 = ylast + n;
        y2 = y + n;
        display.drawLine(xlast2, ylast2, x2, y2, the_color);
      }
      for (n2 = -wd; n2 <= wd; n2++) {
        xlast2 = xlast + n2;
        x2 = x + n2;
        display.drawLine(xlast2, ylast2, x2, y2, the_color);
      }
      for (n = wd; n >= -wd; n--) {
        ylast2 = ylast + n;
        y2 = y + n;
        display.drawLine(xlast2, ylast2, x2, y2, the_color);
      }
      for (n2 = wd; n2 >= -wd; n2--) {
        xlast2 = xlast + n2;
        x2 = x + n2;
        display.drawLine(xlast2, ylast2, x2, y2, the_color);
      }
    }
    xlast = x;
    ylast = y;
    AH = 0;
  }
  else if (AH < 5) {
    AH++;
  }
  else if (AH == 5) {
    xlast = 0;
    ylast = 0;
  }
}


// SPIFFS_functions

//====================================================================================

#ifdef ESP32

void Display::listFiles(void) {
  listDir(SPIFFS, "/", 0);
}

void Display::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {

  Serial.println();
  Serial.println(F("SPIFFS files found:"));

  Serial.printf("Listing directory: %s\n", "/");
  String  line = "=====================================";

  Serial.println(line);
  Serial.println(F("  File name               Size"));
  Serial.println(line);

  fs::File root = fs.open(dirname);
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      int spaces = 20 - fileName.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      String fileSize = (String)file.size();
      spaces = 10 - fileSize.length(); // Tabulate nicely
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
}
#endif
/*
void Display::updateBanner(String msg)
{
  this->img.clearDisplay();
  this->img.setTextSize(1);
  this->img.setTextColor(SSD1306_WHITE);
  this->img.setCursor(0, 0);
  this->img.println(msg);
  this->img.display();
}

void Display::buildBanner(String msg, int xpos)
{
  int h = TEXT_HEIGHT;

  this->img.clearDisplay();

  // Fill with rainbow stripes
  //while (h--) img.drawFastHLine(0, h, SCREEN_WIDTH, 255);

  // Draw some graphics, the text will apear to scroll over these
  //img.fillRect  (SCREEN_WIDTH / 2 - 20, TEXT_HEIGHT / 2 - 10, 40, 20, TFT_YELLOW);
  //img.fillCircle(SCREEN_WIDTH / 2, TEXT_HEIGHT / 2, 10, TFT_ORANGE);

  // Now print text on top of the graphics
  img.setTextSize(BANNER_TEXT_SIZE);           // Font size scaling is x1
  img.setTextColor(SSD1306_WHITE);  // Black text, no background color
  img.setTextWrap(false);       // Turn off wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  img.setCursor(xpos, 2);  // Print text at xpos
  img.println(msg);

  img.setCursor(xpos - 128, 2); // Print text at xpos - sprite width
  img.println(msg);
}
*/
void Display::main(uint8_t scan_mode)
{ 
  /*
  if ((scan_mode == LV_JOIN_WIFI) ||
    (scan_mode == LV_ADD_SSID))
    lv_task_handler();
    */
  return;
}
// End SPIFFS_functions

#endif