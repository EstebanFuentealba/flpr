#include "configs.h"
// #include <WiFi.h>
// #include <Wire.h>
#include <SwitchLib.h>
// #include "esp_wifi.h"
// #include "esp_wifi_types.h"
// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
#include <vector>
#include "canvas_i.h"
#include "record.h"
#include "pubsub.h"

#include "variable_item_list.h"
// #include <view_dispatcher.h>
// #include <gui_i.h>

#include "Assets.h"
// #include "WiFiScan.h"
#include "SDInterface.h"
// #include "Web.h"
#include "Buffer.h"
//#include "LedInterface.h"
// #include "esp_interface.h"
#include "settings.h"
// #include "CommandLine.h"
#include "lang_var.h"
// #include "Display.h"
// #include "MenuFunctions.h"

#define GUI_DISPLAY_WIDTH 128
#define GUI_DISPLAY_HEIGHT  64

#define INPUT_DEBOUNCE_TICKS 4
#define INPUT_DEBOUNCE_TICKS_HALF (INPUT_DEBOUNCE_TICKS / 2)
#define INPUT_PRESS_TICKS 150
#define INPUT_LONG_PRESS_COUNTS 2

#ifndef COUNT_OF
#define COUNT_OF(x) (sizeof(x) / sizeof((x)[0]))
#endif


#define BUTTON_UP_PIN  25
#define BUTTON_DOWN_PIN  32
#define BUTTON_LEFT_PIN  16
#define BUTTON_RIGHT_PIN  26
#define BUTTON_OK_PIN  27
#define BUTTON_BACK_PIN  33


#define SHOW_STOPSCAN_TIP (true)
#define NO_TIP (false)

#define MAX_OPTIONS (9)
#define NUM_MENU_ITEMS (5) //(19)

typedef enum { NO_ARGS = 0, INPUT_ARGS, TOGGLE_ARGS } InputArgs;
typedef enum { FOCUS_CONSOLE_END = 0, FOCUS_CONSOLE_START, FOCUS_CONSOLE_TOGGLE } FocusConsole;




/* Input Related Constants */
#define INPUT_DEBOUNCE_TICKS 4


/**
 * Gpio structure
 */
typedef struct {
    // uint16_t port;
    gpio_num_t pin;
} GpioPin;

typedef struct {
    const GpioPin* gpio;
    const InputKey key;
    const bool inverted;
    const char* name;
} InputPin;

/** Input pin state */
typedef struct {
    const InputPin* pin;
    // State
    volatile bool state;
    volatile uint8_t debounce;
    volatile uint8_t press_counter;
    volatile uint32_t counter;
} InputPinState;




typedef struct {
    FuriPubSub* event_pubsub;
    // InputPinState* pin_states;
    InputPinState* pin_states;
    volatile uint32_t counter;
} Input;

// const size_t gpio_pins_count = 1; //sizeof(gpio_pins) / sizeof(GpioPinRecord);
const GpioPin gpio_button_down = {.pin = BUTTON_DOWN_PIN };
const GpioPin gpio_button_up = {.pin = BUTTON_UP_PIN };
const GpioPin gpio_button_left = {.pin = BUTTON_LEFT_PIN };
const GpioPin gpio_button_right = {.pin = BUTTON_RIGHT_PIN };
const GpioPin gpio_button_ok = {.pin = BUTTON_OK_PIN };
const GpioPin gpio_button_back = {.pin = BUTTON_BACK_PIN };

static Input *input = NULL;

const InputPin input_pins[] = {
    {.gpio = &gpio_button_up, .key = InputKeyUp, .inverted = true, .name = "Up"},
    {.gpio = &gpio_button_down, .key = InputKeyDown, .inverted = true, .name = "Down"},
    {.gpio = &gpio_button_left, .key = InputKeyLeft, .inverted = true, .name = "Left"},
    {.gpio = &gpio_button_right, .key = InputKeyRight, .inverted = true, .name = "Right"},
    {.gpio = &gpio_button_ok, .key = InputKeyOk, .inverted = true, .name = "Ok"},
    {.gpio = &gpio_button_back, .key = InputKeyBack, .inverted = true, .name = "Back"},
};
const size_t gpio_pins_count = COUNT_OF(input_pins);
const size_t input_pins_count = sizeof(input_pins) / sizeof(InputPin);


// SwitchLib u_btn = SwitchLib(BUTTON_UP_PIN, 1000, true);
// SwitchLib d_btn = SwitchLib(BUTTON_DOWN_PIN, 1000, true);
// SwitchLib l_btn = SwitchLib(BUTTON_LEFT_PIN, 1000, true);
// SwitchLib r_btn = SwitchLib(BUTTON_RIGHT_PIN, 1000, true);
// SwitchLib o_btn = SwitchLib(BUTTON_OK_PIN, 1000, true);
// SwitchLib c_btn = SwitchLib(BUTTON_BACK_PIN, 1000, true);

// WiFiScan wifi_scan_obj;
SDInterface sd_obj;
// Web web_obj;
Buffer buffer_obj;
// EspInterface esp_obj;
// Settings settings_obj;


Canvas* canvas;
VariableItemList* var_item_list;
uint32_t currentTime  = 0;
int selected_menu_index;
int selected_option_index[NUM_MENU_ITEMS];
bool is_command;
bool is_custom_tx_string;
bool focus_console_start;
bool show_stopscan_tip;
bool is_writing_pcap;
bool is_writing_log;
const char* selected_tx_string;
// ViewDispatcher* view_dispatcher;
// Gui* gui;


typedef struct {
    const char* item_string;
    const char* options_menu[MAX_OPTIONS];
    int num_options_menu;
    const char* actual_commands[MAX_OPTIONS];
    InputArgs needs_keyboard;
    FocusConsole focus_console;
    bool show_stopscan_tip;
} WifiMarauderItem;

// NUM_MENU_ITEMS defined in wifi_marauder_app_i.h - if you add an entry here, increment it!
const WifiMarauderItem items[NUM_MENU_ITEMS] = {
    {"View Log from", {"start", "end"}, 2, {"", ""}, NO_ARGS, FOCUS_CONSOLE_TOGGLE, NO_TIP},
    {"Scan",
     {"ap", "station"},
     2,
     {"scanap", "scansta"},
     NO_ARGS,
     FOCUS_CONSOLE_END,
     SHOW_STOPSCAN_TIP},
    {"SSID",
     {"add rand", "add name", "remove"},
     3,
     {"ssid -a -g", "ssid -a -n", "ssid -r"},
     INPUT_ARGS,
     FOCUS_CONSOLE_START,
     NO_TIP},
    {"List",
     {"ap", "ssid", "station"},
     3,
     {"list -a", "list -s", "list -c"},
     NO_ARGS,
     FOCUS_CONSOLE_START,
     NO_TIP},
    {"Select",
     {"ap", "ssid", "station"},
     3,
     {"select -a", "select -s", "select -c"},
     INPUT_ARGS,
     FOCUS_CONSOLE_END,
     NO_TIP},
    // {"Clear List",
    //  {"ap", "ssid", "station"},
    //  3,
    //  {"clearlist -a", "clearlist -s", "clearlist -c"},
    //  NO_ARGS,
    //  FOCUS_CONSOLE_END,
    //  NO_TIP},
    // {"Attack",
    //  {"deauth", "probe", "rickroll"},
    //  3,
    //  {"attack -t deauth", "attack -t probe", "attack -t rickroll"},
    //  NO_ARGS,
    //  FOCUS_CONSOLE_END,
    //  SHOW_STOPSCAN_TIP},
    // {"Targeted Deauth",
    //  {"station", "manual"},
    //  2,
    //  {"attack -t deauth -c", "attack -t deauth -s"},
    //  TOGGLE_ARGS,
    //  FOCUS_CONSOLE_END,
    //  SHOW_STOPSCAN_TIP},
    // {"Beacon Spam",
    //  {"ap list", "ssid list", "random"},
    //  3,
    //  {"attack -t beacon -a", "attack -t beacon -l", "attack -t beacon -r"},
    //  NO_ARGS,
    //  FOCUS_CONSOLE_END,
    //  SHOW_STOPSCAN_TIP},
    // {"Sniff",
    //  {"beacon", "deauth", "esp", "pmkid", "probe", "pwn", "raw", "bt", "skim"},
    //  9,
    //  {"sniffbeacon",
    //   "sniffdeauth",
    //   "sniffesp",
    //   "sniffpmkid",
    //   "sniffprobe",
    //   "sniffpwn",
    //   "sniffraw",
    //   "sniffbt",
    //   "sniffskim"},
    //  NO_ARGS,
    //  FOCUS_CONSOLE_END,
    //  SHOW_STOPSCAN_TIP},
    // {"Signal Monitor", {""}, 1, {"sigmon"}, NO_ARGS, FOCUS_CONSOLE_END, SHOW_STOPSCAN_TIP},
    // {"Channel",
    //  {"get", "set"},
    //  2,
    //  {"channel", "channel -s"},
    //  TOGGLE_ARGS,
    //  FOCUS_CONSOLE_END,
    //  NO_TIP},
    // {"Settings",
    //  {"display", "restore", "ForcePMKID", "ForceProbe", "SavePCAP", "EnableLED", "other"},
    //  7,
    //  {"settings",
    //   "settings -r",
    //   "settings -s ForcePMKID enable",
    //   "settings -s ForceProbe enable",
    //   "settings -s SavePCAP enable",
    //   "settings -s EnableLED enable",
    //   "settings -s"},
    //  TOGGLE_ARGS,
    //  FOCUS_CONSOLE_START,
    //  NO_TIP},
    // {"Update", {"ota", "sd"}, 2, {"update -w", "update -s"}, NO_ARGS, FOCUS_CONSOLE_END, NO_TIP},
    // {"Reboot", {""}, 1, {"reboot"}, NO_ARGS, FOCUS_CONSOLE_END, NO_TIP},
    // {"Help", {""}, 1, {"help"}, NO_ARGS, FOCUS_CONSOLE_START, SHOW_STOPSCAN_TIP},
    // {"Reflash ESP32 (WIP)", {""}, 1, {""}, NO_ARGS, FOCUS_CONSOLE_END, NO_TIP},
    // {"Scripts", {""}, 1, {""}, NO_ARGS, FOCUS_CONSOLE_END, NO_TIP},
    // {"Save to flipper sdcard", // keep as last entry or change logic in callback below
    //  {""},
    //  1,
    //  {""},
    //  NO_ARGS,
    //  FOCUS_CONSOLE_START,
    //  NO_TIP},
};


// void drawFrame()
// {
//   canvas_draw_box(canvas, 0, 0, FRAME_W, FRAME_H);
// }

// void tftDrawGraphObjects(byte x_scale)
// {
//   //draw the graph objects
//   canvas_draw_box_invert(canvas, 11, 5, x_scale + 1, 120);
//   canvas_draw_box_invert(canvas, 11, 121, x_scale + 1, 119);
//   // display.fillRect(11, 5, x_scale + 1, 120, INVERSE); // positive start point
//   // display.fillRect(11, 121, x_scale + 1, 119, INVERSE); // negative start point
//   canvas_draw_fast_v_line(canvas, 10, 5, 230); // y axis
//   canvas_draw_fast_h_line(canvas, 10, HEIGHT_1 - 1, 310); // x axis
//   canvas_set_color(canvas, WHITE);
//   canvas_set_font(canvas, FontPrimary); // set parameters for y axis labels
//   canvas_draw_str(canvas, 3, 6, "+"); // "+" at top of y axis
//   canvas_draw_str(canvas, 3, 228, "0"); // "-" at bottom of y axis
// }

// void tftDrawEapolColorKey()
// {
//   //Display color key
//   canvas_set_font(canvas, FontPrimary);
//   canvas_set_color(canvas, WHITE);
//   canvas_draw_box(canvas, 14, 0, 15, 8);
//   canvas_draw_str(canvas, 30, 0, " - EAPOL");
// }
// void tftDrawColorKey()
// {
//   //Display color key
//   canvas_set_font(canvas, FontPrimary);
//   canvas_set_color(canvas, WHITE);
//   canvas_draw_box(canvas, 14, 0, 15, 8);
//   canvas_draw_str(canvas, 30, 0, " - Beacons");

//   canvas_draw_box(canvas, 14, 8, 15, 8);
//   canvas_draw_str(canvas, 30, 8, " - Deauths");

//   canvas_draw_box(canvas, 14, 16, 15, 8);
//   canvas_draw_str(canvas, 30, 16, " - Probes");

// }

static void wifi_marauder_scene_start_var_list_enter_callback(void* context, uint32_t index) {

    // furi_assert(context);
    // WifiMarauderApp* app = context;

    // furi_assert(index < NUM_MENU_ITEMS);
    const WifiMarauderItem* item = &items[index];

    // const int selected_option_index = selected_option_index[index];
    // furi_assert(selected_option_index < item->num_options_menu);
    selected_tx_string = item->actual_commands[selected_option_index[index]];
    is_command = (1 <= index);
    // is_custom_tx_string = false;
    selected_menu_index = index;
    focus_console_start = (item->focus_console == FOCUS_CONSOLE_TOGGLE) ?
                                   (selected_option_index[index] == 0) :
                                   item->focus_console;
    show_stopscan_tip = item->show_stopscan_tip;


    Serial.print("[selected_menu_index]: ");
    Serial.println(selected_menu_index);


    Serial.print("[focus_console_start]: ");
    Serial.println(focus_console_start);

    Serial.print("[show_stopscan_tip]: ");
    Serial.println(show_stopscan_tip);

    Serial.print("[is_command]: ");
    Serial.println(is_command);

    Serial.print("[selected_tx_string]: ");
    Serial.println(selected_tx_string);

    if(strcmp(selected_tx_string, "scanap") == 0) {
      Serial.println("Starting AP scan.");
      // wifi_scan_obj.StartScan(WIFI_SCAN_TARGET_AP);
    }


    // // TODO cleanup
    // if(index == NUM_MENU_ITEMS - 3) {
    //     // flasher
    //     app->is_command = false;
    //     app->flash_mode = true;
    //     view_dispatcher_send_custom_event(app->view_dispatcher, WifiMarauderEventStartFlasher);
    //     return;
    // }

    // app->flash_mode = false;

    // if(!app->is_command && selected_option_index[index] == 0) {
    //     // View Log from start
    //     view_dispatcher_send_custom_event(app->view_dispatcher, WifiMarauderEventStartLogViewer);
    //     return;
    // }

    // if(app->selected_tx_string &&
    //    strncmp("sniffpmkid", app->selected_tx_string, strlen("sniffpmkid")) == 0) {
    //     // sniffpmkid submenu
    //     view_dispatcher_send_custom_event(
    //         app->view_dispatcher, WifiMarauderEventStartSniffPmkidOptions);
    //     return;
    // }

    // // Select automation script
    // if(index == NUM_MENU_ITEMS - 2) {
    //     view_dispatcher_send_custom_event(
    //         app->view_dispatcher, WifiMarauderEventStartScriptSelect);
    //     return;
    // }

    // if(index == NUM_MENU_ITEMS - 1) {
    //     // "Save to flipper sdcard" special case - start SettingsInit widget
    //     view_dispatcher_send_custom_event(
    //         app->view_dispatcher, WifiMarauderEventStartSettingsInit);
    //     return;
    // }

    // bool needs_keyboard = (item->needs_keyboard == TOGGLE_ARGS) ? (selected_option_index[index] != 0) :
    //                                                               item->needs_keyboard;
    // if(needs_keyboard) {
    //     view_dispatcher_send_custom_event(app->view_dispatcher, WifiMarauderEventStartKeyboard);
    // } else {
    //     view_dispatcher_send_custom_event(app->view_dispatcher, WifiMarauderEventStartConsole);
    // }
}

static void wifi_marauder_scene_start_var_list_change_callback(VariableItem* item) {
  const WifiMarauderItem* menu_item = &items[selected_menu_index];
  uint8_t item_index = variable_item_get_current_value_index(item);
  variable_item_set_current_value_text(item, menu_item->options_menu[item_index]);
  selected_option_index[selected_menu_index] = item_index;
}
void RunSetup() {
  //  RunSetup
  canvas = canvas_init();

  Serial.print("getFreeHeap: ");
  Serial.println(ESP.getFreeHeap());
  var_item_list = variable_item_list_alloc();
  

  Serial.print("getFreeHeap: ");
  Serial.println(ESP.getFreeHeap());

  canvas_set_font(canvas, FontPrimary);
  canvas_set_color(canvas, WHITE);
  canvas_clear(canvas);
  canvas_frame_set(canvas, 0, 0, GUI_DISPLAY_WIDTH, GUI_DISPLAY_HEIGHT);

  // canvas_draw_str_aligned(canvas, 128/2, 64/2,  AlignCenter, AlignTop, "Hola Mundo");

  
  variable_item_list_set_enter_callback(var_item_list, wifi_marauder_scene_start_var_list_enter_callback, NULL);
  VariableItem* item;
  for(int i = 0; i < NUM_MENU_ITEMS; ++i) {
      item = variable_item_list_add(
          var_item_list,
          items[i].item_string,
          items[i].num_options_menu,
          wifi_marauder_scene_start_var_list_change_callback,
          NULL);
      variable_item_set_current_value_index(item, selected_option_index[i]);
      variable_item_set_current_value_text(
          item, items[i].options_menu[selected_option_index[i]]);
  }
  variable_item_list_set_selected_item(var_item_list, 0);
  VariableItemListModel* model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
  var_item_list->view->draw_callback(canvas, model);
  // view_draw(var_item_list->view, canvas);

  Serial.print("getFreeHeap: ");
  Serial.println(ESP.getFreeHeap());

  canvas_commit(canvas);
}
void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  furi_record_init();

  input = (Input *)malloc(sizeof(Input));
  input->event_pubsub = furi_pubsub_alloc();
  furi_record_create(RECORD_INPUT_EVENTS, input->event_pubsub);
  Serial.println("[input] furi_record_create ");


  input->pin_states = (InputPinState *)malloc(input_pins_count * sizeof(InputPinState));

  
  //  Input Mode
  // for (size_t i = 0; i < gpio_pins_count; i++)
  // {
  //   pinMode(input_pins[i].gpio->pin, INPUT_PULLUP);
  // }
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  delay(10);


  Serial.print("gpio_pins_count: ");
  Serial.println(gpio_pins_count);

  // InputPinState* pin_state = (InputPinState*)malloc(sizeof(InputPinState));
  // pin_state->state = (bool)digitalRead(BUTTON_DOWN_PIN);
  // pin_state->pin = BUTTON_DOWN_PIN;
  // pin_state->debounce = INPUT_DEBOUNCE_TICKS_HALF;
  // pin_state->press_counter = 0;
  // input->pin_states.push_back(pin_state);

  for (size_t i = 0; i < input_pins_count; i++)
  {
      input->pin_states[i].pin = &input_pins[i];
      input->pin_states[i].state = (bool)digitalRead(input->pin_states[i].pin->gpio->pin);
      input->pin_states[i].debounce = INPUT_DEBOUNCE_TICKS_HALF;
      input->pin_states[i].press_counter = 0;
  }
  Serial.println("[input] set input_pins");


  // pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  // if(sd_obj.initSD()) {
  //   Serial.println("SD Card Initialized");
  // } else {
  //   Serial.println(F("SD Card NOT Supported"));
  // }

  RunSetup();
}
void loop() {
  currentTime = millis();
  /*
  InputEvent* event = malloc(sizeof(InputEvent));
  event->type = InputTypeShort;
  if (u_btn.justReleased()){
      Serial.println("Up");
      VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      event->key = InputKeyUp;
      var_item_list->view->input_callback(event, var_item_list);
      var_item_list->view->draw_callback(canvas, model);
      selected_menu_index = variable_item_list_get_selected_item_index(var_item_list);
  }
  if (d_btn.justReleased()){
      Serial.println("Down");
      VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      event->key = InputKeyDown;
      var_item_list->view->input_callback(event, var_item_list);
      var_item_list->view->draw_callback(canvas, model);
      selected_menu_index = variable_item_list_get_selected_item_index(var_item_list);
  }
  if (l_btn.justReleased()){
      Serial.println("Left");
      VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      event->key = InputKeyLeft;
      var_item_list->view->input_callback(event, var_item_list);
      var_item_list->view->draw_callback(canvas, model);
      selected_menu_index = variable_item_list_get_selected_item_index(var_item_list);
  }
  if (r_btn.justReleased()){
      Serial.println("Right");

      VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      event->key = InputKeyRight;
      var_item_list->view->input_callback(event, var_item_list);
      var_item_list->view->draw_callback(canvas, model);
      selected_menu_index = variable_item_list_get_selected_item_index(var_item_list);
  }
  if (o_btn.justReleased()){
      Serial.println("OK");
      // VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      VariableItemListModel * model = static_cast<VariableItemListModel*>(view_get_model(var_item_list->view));
      event->key = InputKeyOk;
      var_item_list->view->input_callback(event, var_item_list);
      var_item_list->view->draw_callback(canvas, model);
      selected_menu_index = variable_item_list_get_selected_item_index(var_item_list);
  }
  if (c_btn.justReleased()){
      Serial.println("Back");
  }
  */
  bool is_changing = false;
  for (size_t i = 0; i < input_pins_count; i++)
  {
      int state = digitalRead(input->pin_states[i].pin->gpio->pin);
      if (state == LOW)
      {
          // Serial.println("READ pin: " + String(input->pin_states[i].pin->gpio->pin) + " state:" + String(state));
          if (input->pin_states[i].debounce < INPUT_DEBOUNCE_TICKS)
              input->pin_states[i].debounce += 1;
      }
      else
      {
          if (input->pin_states[i].debounce > 0)
              input->pin_states[i].debounce -= 1;
      }

      if (input->pin_states[i].debounce > 0 && input->pin_states[i].debounce < INPUT_DEBOUNCE_TICKS)
      {
          is_changing = true;
      }
          else if (input->pin_states[i].state != state)
      {
          input->pin_states[i].state = state;
          
          if (state == HIGH)
          {
              InputEvent event;
              event.sequence_source = INPUT_SEQUENCE_SOURCE_HARDWARE;
              event.key = input->pin_states[i].pin->key;
              event.sequence_counter = input->pin_states[i].counter;
              event.type = InputTypeShort;
              // Botón liberado
              if (input->pin_states[i].press_counter >= INPUT_LONG_PRESS_COUNTS)
              {
                  // Emitir evento largo
                  // ...
                  input->pin_states[i].press_counter = 0;

                  Serial.println("Clickeado LARGO!");
              }
              else {
                  // Emitir evento corto
                  // ...
                  input->pin_states[i].press_counter = 0;
                  Serial.println("Clickeado CORTO!");

                    
              }

              Serial.println("[input] InputTypeRelease");
              // event.type = input->pin_states[i].state ? InputTypePress : InputTypeRelease;
              // Serial.println("[input] try furi_pubsub_publish");
              furi_pubsub_publish(input->event_pubsub, &event);
              Serial.println("[input] furi_pubsub_publish");
              // Detener el temporizador
              // input_timer_stop();
          }
          else
          {
              // Serial.println("Clickeado !");
              // Botón presionado
              // input_timer_start(INPUT_LONG_PRESS_COUNTS);
              input->pin_states[i].press_counter = 0;
          }
      }
  }
  if (is_changing)
  {
      delay(1);
  }
  else
  {
      
  }
}


// SwitchLib u_btn = SwitchLib(U_BTN, 1000, true);
// SwitchLib d_btn = SwitchLib(D_BTN, 1000, true);
// SwitchLib l_btn = SwitchLib(L_BTN, 1000, true);
// SwitchLib r_btn = SwitchLib(R_BTN, 1000, true);
// SwitchLib c_btn = SwitchLib(C_BTN, 1000, true);







// const String PROGMEM version_number = MARAUDER_VERSION;

// uint32_t currentTime  = 0;


// void setup() {
//   pinMode(SD_CS, OUTPUT);
//   digitalWrite(SD_CS, HIGH);
//   Serial.begin(115200);


//   Serial.println("ESP-IDF version is: " + String(esp_get_idf_version()));

//   display_obj.RunSetup();
//   display_obj.display.setTextColor(WHITE);
//   display_obj.display.setCursor((display_obj.display.width() - display_obj.version_number.length() * 12) / 2, 32);
//   display_obj.display.println(display_obj.version_number);
//   display_obj.display.display();


//   settings_obj.begin();
//   wifi_scan_obj.RunSetup();

//   if(sd_obj.initSD()) {
    
//   } else {
//     Serial.println(F("SD Card NOT Supported"));
//   }

//   // menu_function_obj.RunSetup();

//   Serial.println("CLI Ready");

// }
// void loop() {
//   currentTime = millis();
//   bool mini = false;
//   bool do_draw = false;
  
//   // Serial.print("currentScanMode: ");
//   // Serial.println(wifi_scan_obj.currentScanMode);

//   // Serial.print("do_draw: ");
//   // Serial.println(do_draw);

//   // Serial.print("ESP_UPDATE: ");
//   // Serial.println(ESP_UPDATE);

//   // Serial.print("OTA_UPDATE: ");
//   // Serial.println(OTA_UPDATE);


//   if ((!do_draw) && (wifi_scan_obj.currentScanMode != ESP_UPDATE))
//   {
//     // cli_obj.main(currentTime);
//     wifi_scan_obj.main(currentTime);
//     sd_obj.main();
    
//     if (((wifi_scan_obj.currentScanMode != WIFI_PACKET_MONITOR) && (wifi_scan_obj.currentScanMode != WIFI_SCAN_EAPOL)) || (mini)) {
//       menu_function_obj.main(currentTime);
//       //cli_obj.main(currentTime);
//     }
//     if (wifi_scan_obj.currentScanMode == OTA_UPDATE) {
//       //web_obj.main();
//     } 
//     delay(1);
//   } /*else if ((display_obj.draw_tft) && (wifi_scan_obj.currentScanMode != OTA_UPDATE)) {
//     display_obj.drawStylus();
//   } else if (wifi_scan_obj.currentScanMode == ESP_UPDATE) {
//     display_obj.main(wifi_scan_obj.currentScanMode);
//     menu_function_obj.main(currentTime);
//     //cli_obj.main(currentTime);
//     delay(1);
//   }*/


//   // display_obj.tftDrawColorKey();
// }