#include "MenuFunctions.h"
#include "lang_var.h"
//#include "icons.h"

#ifdef HAS_SCREEN

extern const unsigned char menu_icons[][66];
//PROGMEM lv_obj_t * slider_label;
//PROGMEM lv_obj_t * ta1;
//PROGMEM lv_obj_t * ta2;
//PROGMEM lv_obj_t * save_name;

MenuFunctions::MenuFunctions()
{
}

// LVGL Stuff
/* Interrupt driven periodic handler */


//// END LV_ARDUINO STUFF

void MenuFunctions::buttonNotSelected(uint8_t b) {
  Serial.print("buttonNotSelected setFont");
  display_obj.display.setFont(NULL);
  Serial.print("buttonNotSelected drawButton");
  display_obj.key[b].drawButton(current_menu->list->get(b).name);
}

void MenuFunctions::buttonSelected(uint8_t b) {
  Serial.println("buttonSelected setFont");
  display_obj.display.setFont(NULL);
  Serial.println("buttonSelected drawButton");
  display_obj.key[b].drawButton(current_menu->list->get(b).name);
}

// Function to check menu input
void MenuFunctions::main(uint32_t currentTime)
{
  // Some function exited and we need to go back to normal
  if (display_obj.exit_draw) {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    display_obj.exit_draw = false;
    this->orientDisplay();
  }
  if ((wifi_scan_obj.currentScanMode == WIFI_SCAN_OFF) ||
      (wifi_scan_obj.currentScanMode == OTA_UPDATE) ||
      (wifi_scan_obj.currentScanMode == ESP_UPDATE) ||
      (wifi_scan_obj.currentScanMode == SHOW_INFO)) {
    if (wifi_scan_obj.orient_display) {
      this->orientDisplay();
      wifi_scan_obj.orient_display = false;
    }
  }

  if (currentTime != 0) {
    if (currentTime - initTime >= BANNER_TIME) {
      this->initTime = millis();
      if ((wifi_scan_obj.currentScanMode != LV_JOIN_WIFI) &&
          (wifi_scan_obj.currentScanMode != LV_ADD_SSID))
        this->updateStatusBar();
    }
  }


  boolean pressed = false;
  // This is code from bodmer's keypad example
  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

  // Get the display buffer out of the way
  if ((wifi_scan_obj.currentScanMode != WIFI_SCAN_OFF ) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_BEACON_SPAM) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_AP_SPAM) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_AUTH) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_DEAUTH) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_DEAUTH_MANUAL) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_DEAUTH_TARGETED) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_MIMIC) &&
      (wifi_scan_obj.currentScanMode != WIFI_ATTACK_RICK_ROLL))
    display_obj.displayBuffer();


  // Pressed will be set true is there is a valid touch on the screen
  int pre_getTouch = millis();





  #ifdef HAS_BUTTONS

    bool c_btn_press = c_btn.justPressed();
    
    if ((c_btn_press) &&
        (wifi_scan_obj.currentScanMode != WIFI_SCAN_OFF) &&
        (wifi_scan_obj.currentScanMode != OTA_UPDATE) &&
        (wifi_scan_obj.currentScanMode != ESP_UPDATE) &&
        (wifi_scan_obj.currentScanMode != SHOW_INFO))
    {
      // Stop the current scan
      if ((wifi_scan_obj.currentScanMode == WIFI_SCAN_PROBE) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_RAW_CAPTURE) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_STATION) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_AP) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_SIG_STREN) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_TARGET_AP) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_TARGET_AP_FULL) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_PWN) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_ESPRESSIF) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_ALL) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_DEAUTH) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_BEACON_SPAM) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_AP_SPAM) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_AUTH) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_DEAUTH) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_DEAUTH_MANUAL) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_DEAUTH_TARGETED) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_MIMIC) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_RICK_ROLL) ||
          (wifi_scan_obj.currentScanMode == WIFI_ATTACK_BEACON_LIST) ||
          (wifi_scan_obj.currentScanMode == BT_SCAN_ALL) ||
          (wifi_scan_obj.currentScanMode == BT_SCAN_SKIMMERS) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_EAPOL) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_ACTIVE_EAPOL) ||
          (wifi_scan_obj.currentScanMode == WIFI_SCAN_ACTIVE_LIST_EAPOL) ||
          (wifi_scan_obj.currentScanMode == WIFI_PACKET_MONITOR))
      {
        wifi_scan_obj.StartScan(WIFI_SCAN_OFF);
  
        // If we don't do this, the text and button coordinates will be off
        //display_obj.display.init();
        //display_obj.display.begin();
  
        // Take us back to the menu
        changeMenu(current_menu);
      }
  
      x = -1;
      y = -1;
  
      return;
    }

  #endif



  // #ifdef HAS_BUTTONS
    // #ifndef MARAUDER_M5STICKC
    //   if (u_btn.justPressed()){
    //     if ((wifi_scan_obj.currentScanMode == WIFI_SCAN_OFF) ||
    //         (wifi_scan_obj.currentScanMode == OTA_UPDATE)) {
    //       if (current_menu->selected > 0) {
    //         current_menu->selected--;
    //         this->buttonSelected(current_menu->selected);
    //         if (!current_menu->list->get(current_menu->selected + 1).selected)
    //           this->buttonNotSelected(current_menu->selected + 1);
    //       }
    //     }
    //     else if ((wifi_scan_obj.currentScanMode == WIFI_PACKET_MONITOR) ||
    //             (wifi_scan_obj.currentScanMode == WIFI_SCAN_EAPOL)) {
    //       if (wifi_scan_obj.set_channel < 14)
    //         wifi_scan_obj.changeChannel(wifi_scan_obj.set_channel + 1);
    //     }
    //   }
    // #endif
    Serial.print("d_btn.justPressed(): ");
    Serial.println(d_btn.justPressed());
    if (d_btn.justPressed()){
      Serial.print("currentScanMode: ");
      Serial.println(wifi_scan_obj.currentScanMode);
      if ((wifi_scan_obj.currentScanMode == WIFI_SCAN_OFF) ||
          (wifi_scan_obj.currentScanMode == OTA_UPDATE)) {
        if (current_menu->selected < current_menu->list->size() - 1) {
          current_menu->selected++;
          this->buttonSelected(current_menu->selected);
          if (!current_menu->list->get(current_menu->selected - 1).selected)
            this->buttonNotSelected(current_menu->selected - 1);
        } else {
          current_menu->selected = 0;
          this->buttonSelected(current_menu->selected);
          if (!current_menu->list->get(current_menu->list->size() - 1).selected)
            this->buttonNotSelected(current_menu->list->size() - 1);
        }
      } else if ((wifi_scan_obj.currentScanMode == WIFI_PACKET_MONITOR) ||
               (wifi_scan_obj.currentScanMode == WIFI_SCAN_EAPOL)) {
        if (wifi_scan_obj.set_channel > 1)
          wifi_scan_obj.changeChannel(wifi_scan_obj.set_channel - 1);
      }
    }
    Serial.print("c_btn_press: ");
    Serial.print(c_btn_press);
    if(c_btn_press){
      current_menu->list->get(current_menu->selected).callable();
    }

  // #endif
}


void MenuFunctions::updateStatusBar()
{
  display_obj.display.setTextSize(1);
  
  display_obj.display.setTextColor(WHITE);

  // WiFi Channel Stuff
  if (wifi_scan_obj.set_channel != wifi_scan_obj.old_channel) {
    wifi_scan_obj.old_channel = wifi_scan_obj.set_channel;
    display_obj.display.fillRect(50, 0, 50, STATUS_BAR_WIDTH, WHITE);
    #ifdef HAS_ILI9341
      display_obj.display.drawString("CH: " + (String)wifi_scan_obj.set_channel, 50, 0, 2);
    #endif

    #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
      display_obj.display.drawString("CH: " + (String)wifi_scan_obj.set_channel, TFT_WIDTH/4, 0, 1);
    #endif
  }

  // RAM Stuff
  wifi_scan_obj.freeRAM();
  if (wifi_scan_obj.free_ram != wifi_scan_obj.old_free_ram) {
    wifi_scan_obj.old_free_ram = wifi_scan_obj.free_ram;
    display_obj.display.fillRect(100, 0, 60, STATUS_BAR_WIDTH, WHITE);
    #ifdef HAS_ILI9341
      display_obj.display.drawString((String)wifi_scan_obj.free_ram + "B", 100, 0, 2);
    #endif

    #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
      display_obj.display.drawString((String)wifi_scan_obj.free_ram + "B", TFT_WIDTH/1.75, 0, 1);
    #endif
  }

  // Draw battery info
  // MenuFunctions::battery(false);

  // Draw SD info
  

  display_obj.display.setTextColor(WHITE);
  display_obj.display.setTextSize(1);
  display_obj.display.setCursor(TFT_WIDTH - 12, 0);
  display_obj.display.println("SD");


}

void MenuFunctions::drawStatusBar()
{
  display_obj.display.setTextSize(1);
  #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
    display_obj.display.setFont(NULL);
  #endif
  display_obj.display.fillRect(0, 0, 240, STATUS_BAR_WIDTH, WHITE);
  //display_obj.display.fillRect(0, STATUS_BAR_WIDTH + 1, 240, 1, TFT_DARKGREY);
  display_obj.display.setTextColor(WHITE);
  //display_obj.display.setTextSize(2);

  uint16_t the_color;
  display_obj.display.setTextColor(WHITE);


  // WiFi Channel Stuff
  wifi_scan_obj.old_channel = wifi_scan_obj.set_channel;
  display_obj.display.fillRect(50, 0, 50, STATUS_BAR_WIDTH, WHITE);
  #ifdef HAS_ILI9341
    display_obj.display.drawString("CH: " + (String)wifi_scan_obj.set_channel, 50, 0, 2);
  #endif

  #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
    display_obj.display.drawString("CH: " + (String)wifi_scan_obj.set_channel, TFT_WIDTH/4, 0, 1);
  #endif

  // RAM Stuff
  wifi_scan_obj.freeRAM();
  wifi_scan_obj.old_free_ram = wifi_scan_obj.free_ram;
  display_obj.display.fillRect(100, 0, 60, STATUS_BAR_WIDTH, WHITE);
  #ifdef HAS_ILI9341
    display_obj.display.drawString((String)wifi_scan_obj.free_ram + "B", 100, 0, 2);
  #endif

  #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
    display_obj.display.drawString((String)wifi_scan_obj.free_ram + "B", TFT_WIDTH/1.75, 0, 1);
  #endif


  // MenuFunctions::battery2(true);

  // Draw SD info
  the_color = WHITE;

  #ifdef HAS_ILI9341
    display_obj.display.drawXBitmap(170,
                                0,
                                menu_icons[STATUS_SD],
                                16,
                                16,
                                WHITE,
                                the_color);
  #endif

  #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
    display_obj.display.setTextColor(the_color, WHITE);
    display_obj.display.drawString("SD", TFT_WIDTH - 12, 0, 1);
  #endif
}

void MenuFunctions::orientDisplay()
{
  Serial.println("orientDisplay");
  //display_obj.display.init();
  //display_obj.display.begin();

  display_obj.display.setRotation(0); // Portrait

  display_obj.display.setCursor(0, 0);

  changeMenu(current_menu);
}


String MenuFunctions::callSetting(String key) {
  specSettingMenu.name = key;
  
  String setting_type = settings_obj.getSettingType(key);

  if (setting_type == "bool") {
    return "bool";
  }
}

void MenuFunctions::displaySetting(String key, Menu* menu, int index) {
  specSettingMenu.name = key;

  bool setting_value = settings_obj.loadSetting<bool>(key);

  // Make a local copy of menu node
  MenuNode node = menu->list->get(index);

  display_obj.display.setTextWrap(false);
  display_obj.display.setFont(NULL);
  display_obj.display.setCursor(0, 100);
  display_obj.display.setTextSize(1);

  // Set local copy value
  if (!setting_value) {
    display_obj.display.setTextColor(WHITE);
    display_obj.display.println(F(text_table1[4]));
    node.selected = false;
  }
  else {
    display_obj.display.setTextColor(WHITE);
    display_obj.display.println(F(text_table1[5]));
    node.selected = true;
  }

  // Put local copy back into menu
  menu->list->set(index, node);
    
}


// Function to build the menus
void MenuFunctions::RunSetup()
{
  extern LinkedList<AccessPoint>* access_points;
  
   
  // root menu stuff
  mainMenu.list = new LinkedList<MenuNode>(); // Get list in first menu ready

  // Main menu stuff
  wifiMenu.list = new LinkedList<MenuNode>(); // Get list in second menu ready
  bluetoothMenu.list = new LinkedList<MenuNode>(); // Get list in third menu ready
  badusbMenu.list = new LinkedList<MenuNode>();
  generalMenu.list = new LinkedList<MenuNode>();
  deviceMenu.list = new LinkedList<MenuNode>();

  // Device menu stuff
  failedUpdateMenu.list = new LinkedList<MenuNode>();
  whichUpdateMenu.list = new LinkedList<MenuNode>();
  confirmMenu.list = new LinkedList<MenuNode>();
  espUpdateMenu.list = new LinkedList<MenuNode>();
  updateMenu.list = new LinkedList<MenuNode>();
  settingsMenu.list = new LinkedList<MenuNode>();
  specSettingMenu.list = new LinkedList<MenuNode>();
  infoMenu.list = new LinkedList<MenuNode>();
  languageMenu.list = new LinkedList<MenuNode>();

  // WiFi menu stuff
  wifiSnifferMenu.list = new LinkedList<MenuNode>();
  wifiAttackMenu.list = new LinkedList<MenuNode>();
  wifiGeneralMenu.list = new LinkedList<MenuNode>();
  wifiAPMenu.list = new LinkedList<MenuNode>();

  // Bluetooth menu stuff
  bluetoothSnifferMenu.list = new LinkedList<MenuNode>();
  bluetoothGeneralMenu.list = new LinkedList<MenuNode>();

  // Settings stuff
  shutdownWiFiMenu.list = new LinkedList<MenuNode>();
  shutdownBLEMenu.list = new LinkedList<MenuNode>();
  generateSSIDsMenu.list = new LinkedList<MenuNode>();
  clearSSIDsMenu.list = new LinkedList<MenuNode>();
  clearAPsMenu.list = new LinkedList<MenuNode>();

  // Work menu names
  mainMenu.name = text_table1[6];
  wifiMenu.name = text_table1[7];
  badusbMenu.name = text_table1[8];
  deviceMenu.name = text_table1[9];
  generalMenu.name = text_table1[10];
  failedUpdateMenu.name = text_table1[11];
  whichUpdateMenu.name = text_table1[12];
  confirmMenu.name = text_table1[13];
  espUpdateMenu.name = text_table1[14];
  updateMenu.name = text_table1[15];
  languageMenu.name = text_table1[16]; 
  infoMenu.name = text_table1[17];
  settingsMenu.name = text_table1[18];
  bluetoothMenu.name = text_table1[19];
  wifiSnifferMenu.name = text_table1[20];
  wifiAttackMenu.name = text_table1[21];
  wifiGeneralMenu.name = text_table1[22];
  bluetoothSnifferMenu.name = text_table1[23];
  bluetoothGeneralMenu.name = text_table1[24];
  shutdownWiFiMenu.name = text_table1[25];
  shutdownBLEMenu.name = text_table1[26];
  generateSSIDsMenu.name = text_table1[27];
  clearSSIDsMenu.name = text_table1[28];
  clearAPsMenu.name = text_table1[29];
  wifiAPMenu.name = "Access Points";
  

  // Build Main Menu
  mainMenu.parentMenu = NULL;
  addNodes(&mainMenu, text_table1[7], WHITE, NULL, WIFI, [this]() {
    changeMenu(&wifiMenu);
  });
  addNodes(&mainMenu, text_table1[19], WHITE, NULL, BLUETOOTH, [this]() {
    changeMenu(&bluetoothMenu);
  });

  addNodes(&mainMenu, text_table1[10], WHITE, NULL, GENERAL_APPS, [this]() {
    changeMenu(&generalMenu);
  });
  addNodes(&mainMenu, text_table1[9], WHITE, NULL, DEVICE, [this]() {
    changeMenu(&deviceMenu);
  });
  addNodes(&mainMenu, text_table1[30], WHITE, NULL, REBOOT, []() {
    ESP.restart();
  });

  // Build WiFi Menu
  wifiMenu.parentMenu = &mainMenu; // Main Menu is second menu parent
  addNodes(&wifiMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(wifiMenu.parentMenu);
  });
  addNodes(&wifiMenu, text_table1[31], WHITE, NULL, SNIFFERS, [this]() {
    changeMenu(&wifiSnifferMenu);
  });
  //addNodes(&wifiMenu, "Scanners", TFT_ORANGE, NULL, SCANNERS, [this]() {
  //  changeMenu(&wifiScannerMenu);
  //});
  addNodes(&wifiMenu, text_table1[32], WHITE, NULL, ATTACKS, [this]() {
    changeMenu(&wifiAttackMenu);
  });
  addNodes(&wifiMenu, text_table1[33], WHITE, NULL, GENERAL_APPS, [this]() {
    changeMenu(&wifiGeneralMenu);
  });

  // Build WiFi sniffer Menu
  wifiSnifferMenu.parentMenu = &wifiMenu; // Main Menu is second menu parent
  addNodes(&wifiSnifferMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(wifiSnifferMenu.parentMenu);
  });
  addNodes(&wifiSnifferMenu, text_table1[42], WHITE, NULL, PROBE_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_PROBE, WHITE);
  });
  addNodes(&wifiSnifferMenu, text_table1[43], WHITE, NULL, BEACON_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_AP, WHITE);
  });
  addNodes(&wifiSnifferMenu, text_table1[44], WHITE, NULL, DEAUTH_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_DEAUTH, WHITE);
  });
  #ifdef HAS_ILI9341
    addNodes(&wifiSnifferMenu, text_table1[46], WHITE, NULL, EAPOL, [this]() {
      wifi_scan_obj.StartScan(WIFI_SCAN_EAPOL, WHITE);
    });
    addNodes(&wifiSnifferMenu, text_table1[45], TFT_BLUE, NULL, PACKET_MONITOR, [this]() {
      wifi_scan_obj.StartScan(WIFI_PACKET_MONITOR, TFT_BLUE);
    });
  #else
    addNodes(&wifiSnifferMenu, text_table1[46], WHITE, NULL, EAPOL, [this]() {
      display_obj.clearScreen();
      this->drawStatusBar();
      wifi_scan_obj.StartScan(WIFI_SCAN_EAPOL, WHITE);
    });
    addNodes(&wifiSnifferMenu, text_table1[45], WHITE, NULL, PACKET_MONITOR, [this]() {
      display_obj.clearScreen();
      this->drawStatusBar();
      wifi_scan_obj.StartScan(WIFI_PACKET_MONITOR, WHITE);
    });
  #endif
  addNodes(&wifiSnifferMenu, text_table1[47], WHITE, NULL, PWNAGOTCHI, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_PWN, WHITE);
  });
  /*addNodes(&wifiSnifferMenu, text_table1[48], TFT_ORANGE, NULL, ESPRESSIF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_ESPRESSIF, TFT_ORANGE);
  });*/
  addNodes(&wifiSnifferMenu, text_table1[49], WHITE, NULL, BEACON_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_TARGET_AP, WHITE);
  });
  addNodes(&wifiSnifferMenu, text_table1[58], WHITE, NULL, PACKET_MONITOR, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_RAW_CAPTURE, WHITE);
  });
  addNodes(&wifiSnifferMenu, text_table1[59], WHITE, NULL, PACKET_MONITOR, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_SCAN_STATION, WHITE);
  });
  #ifdef HAS_ILI9341
    addNodes(&wifiSnifferMenu, "Signal Monitor", WHITE, NULL, PACKET_MONITOR, [this]() {
      display_obj.clearScreen();
      this->drawStatusBar();
      wifi_scan_obj.StartScan(WIFI_SCAN_SIG_STREN, WHITE);
    });
  #endif

  // Build WiFi attack menu
  wifiAttackMenu.parentMenu = &wifiMenu; // Main Menu is second menu parent
  addNodes(&wifiAttackMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(wifiAttackMenu.parentMenu);
  });
  addNodes(&wifiAttackMenu, text_table1[50], WHITE, NULL, BEACON_LIST, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_BEACON_LIST, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[51], WHITE, NULL, BEACON_SPAM, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_BEACON_SPAM, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[52], WHITE, NULL, RICK_ROLL, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_RICK_ROLL, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[53], WHITE, NULL, PROBE_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_AUTH, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[54], WHITE, NULL, DEAUTH_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_DEAUTH, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[57], WHITE, NULL, BEACON_LIST, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_AP_SPAM, WHITE);
  });
  addNodes(&wifiAttackMenu, text_table1[62], WHITE, NULL, DEAUTH_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(WIFI_ATTACK_DEAUTH_TARGETED, WHITE);
  });
  //addNodes(&wifiAttackMenu, "AP Mimic Flood", TFT_PURPLE, NULL, DEAUTH_SNIFF, [this]() {
  //  display_obj.clearScreen();
  //  this->drawStatusBar();
  //  wifi_scan_obj.StartScan(WIFI_ATTACK_MIMIC, TFT_PURPLE);
  //});

  // Build WiFi General menu
  wifiGeneralMenu.parentMenu = &wifiMenu;
  addNodes(&wifiGeneralMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(wifiGeneralMenu.parentMenu);
  });
  #ifdef HAS_ILI9341
    addNodes(&wifiGeneralMenu, text_table1[55], WHITE, NULL, JOIN_WIFI, [this](){
      display_obj.clearScreen(); 
      wifi_scan_obj.currentScanMode = LV_JOIN_WIFI; 
      wifi_scan_obj.StartScan(LV_JOIN_WIFI, WHITE); 
      joinWiFiGFX();
    });
  #endif
  addNodes(&wifiGeneralMenu, text_table1[25], WHITE, NULL, SHUTDOWN, [this]() {
    changeMenu(&shutdownWiFiMenu);
    wifi_scan_obj.RunShutdownWiFi();
  });
  addNodes(&wifiGeneralMenu, text_table1[27], WHITE, NULL, GENERATE, [this]() {
    changeMenu(&generateSSIDsMenu);
    wifi_scan_obj.RunGenerateSSIDs();
  });
  #ifdef HAS_ILI9341
    addNodes(&wifiGeneralMenu, text_table1[1], WHITE, NULL, KEYBOARD_ICO, [this](){
      display_obj.clearScreen(); 
      //wifi_scan_obj.currentScanMode = LV_ADD_SSID; 
      wifi_scan_obj.StartScan(LV_ADD_SSID, WHITE); 
      addSSIDGFX();
    });
  #endif
  addNodes(&wifiGeneralMenu, text_table1[28], WHITE, NULL, CLEAR_ICO, [this]() {
    changeMenu(&clearSSIDsMenu);
    wifi_scan_obj.RunClearSSIDs();
  });
  addNodes(&wifiGeneralMenu, text_table1[29], WHITE, NULL, CLEAR_ICO, [this]() {
    changeMenu(&clearAPsMenu);
    wifi_scan_obj.RunClearAPs();
  });
  addNodes(&wifiGeneralMenu, text_table1[60], WHITE, NULL, CLEAR_ICO, [this]() {
    changeMenu(&clearAPsMenu);
    wifi_scan_obj.RunClearStations();
  });
  #ifdef HAS_ILI9341
    // Select APs on OG
    addNodes(&wifiGeneralMenu, text_table1[56], WHITE, NULL, KEYBOARD_ICO, [this](){
      display_obj.clearScreen(); 
      wifi_scan_obj.currentScanMode = LV_ADD_SSID; 
      wifi_scan_obj.StartScan(LV_ADD_SSID, WHITE);  
      addAPGFX();
    });
    addNodes(&wifiGeneralMenu, text_table1[61], WHITE, NULL, KEYBOARD_ICO, [this](){
      display_obj.clearScreen(); 
      wifi_scan_obj.currentScanMode = LV_ADD_SSID; 
      wifi_scan_obj.StartScan(LV_ADD_SSID, WHITE);  
      addStationGFX();
    });
  #else
    // Select APs on Mini
    addNodes(&wifiGeneralMenu, text_table1[56], WHITE, NULL, KEYBOARD_ICO, [this](){
      wifiAPMenu.list->clear();
        addNodes(&wifiAPMenu, text09, WHITE, NULL, 0, [this]() {
        changeMenu(wifiAPMenu.parentMenu);
      });
      int menu_limit;
      if (access_points->size() <= BUTTON_ARRAY_LEN)
        menu_limit = access_points->size();
      else
        menu_limit = BUTTON_ARRAY_LEN;
      for (int i = 0; i < menu_limit - 1; i++) {
        addNodes(&wifiAPMenu, access_points->get(i).essid, WHITE, NULL, KEYBOARD_ICO, [this, i](){
        AccessPoint new_ap = access_points->get(i);
        new_ap.selected = !access_points->get(i).selected;

        // Change selection status of menu node
        MenuNode new_node = current_menu->list->get(i + 1);
        new_node.selected = !current_menu->list->get(i + 1).selected;
        current_menu->list->set(i + 1, new_node);

        // Change selection status of button key
        if (new_ap.selected) {
          this->buttonSelected(i + 1);
          //changeMenu(current_menu);
        } else {
          this->buttonNotSelected(i + 1);
          //changeMenu(current_menu);
        }
        access_points->set(i, new_ap);
        //changeMenu(wifiAPMenu.parentMenu);
        }, access_points->get(i).selected);
      }
      changeMenu(&wifiAPMenu);
    });

    wifiAPMenu.parentMenu = &wifiGeneralMenu;
    addNodes(&wifiAPMenu, text09, WHITE, NULL, 0, [this]() {
      changeMenu(wifiAPMenu.parentMenu);
    });
  #endif

  // Build shutdown wifi menu
  shutdownWiFiMenu.parentMenu = &wifiGeneralMenu;
  addNodes(&shutdownWiFiMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(shutdownWiFiMenu.parentMenu);
  });

  // Build generate ssids menu
  generateSSIDsMenu.parentMenu = &wifiGeneralMenu;
  addNodes(&generateSSIDsMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(generateSSIDsMenu.parentMenu);
  });

  // Build clear ssids menu
  clearSSIDsMenu.parentMenu = &wifiGeneralMenu;
  addNodes(&clearSSIDsMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(clearSSIDsMenu.parentMenu);
  });
  clearAPsMenu.parentMenu = &wifiGeneralMenu;
  addNodes(&clearAPsMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(clearAPsMenu.parentMenu);
  });


  // Build Bluetooth Menu
  bluetoothMenu.parentMenu = &mainMenu; // Second Menu is third menu parent
  addNodes(&bluetoothMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(bluetoothMenu.parentMenu);
  });
  addNodes(&bluetoothMenu, text_table1[31], WHITE, NULL, SNIFFERS, [this]() {
    changeMenu(&bluetoothSnifferMenu);
  });
  //addNodes(&bluetoothMenu, "Scanners", TFT_ORANGE, NULL, SCANNERS, [this]() {
  //  changeMenu(&bluetoothScannerMenu);
  //});
  addNodes(&bluetoothMenu, text_table1[33], WHITE, NULL, GENERAL_APPS, [this]() {
    changeMenu(&bluetoothGeneralMenu);
  });

  // Build bluetooth sniffer Menu
  bluetoothSnifferMenu.parentMenu = &bluetoothMenu; // Second Menu is third menu parent
  addNodes(&bluetoothSnifferMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(bluetoothSnifferMenu.parentMenu);
  });
  addNodes(&bluetoothSnifferMenu, text_table1[34], WHITE, NULL, BLUETOOTH_SNIFF, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(BT_SCAN_ALL, WHITE);
  });
  addNodes(&bluetoothSnifferMenu, text_table1[35], WHITE, NULL, CC_SKIMMERS, [this]() {
    display_obj.clearScreen();
    this->drawStatusBar();
    wifi_scan_obj.StartScan(BT_SCAN_SKIMMERS, WHITE);
  });

  // Build bluetooth scanner Menu
  //bluetoothScannerMenu.parentMenu = &bluetoothMenu; // Second Menu is third menu parent
  //addNodes(&bluetoothScannerMenu, text09, TFT_LIGHTGREY, NULL, 0, [this]() {
  //  changeMenu(bluetoothScannerMenu.parentMenu);
  //});

  // Build bluetooth general menu
  bluetoothGeneralMenu.parentMenu = &bluetoothMenu;
  addNodes(&bluetoothGeneralMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(bluetoothGeneralMenu.parentMenu);
  });
  addNodes(&bluetoothGeneralMenu, text_table1[26], WHITE, NULL, SHUTDOWN, [this]() {
    changeMenu(&shutdownBLEMenu);
    wifi_scan_obj.RunShutdownBLE();
  });

  // Build shutdown BLE menu
  shutdownBLEMenu.parentMenu = &bluetoothGeneralMenu;
  addNodes(&shutdownBLEMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(shutdownBLEMenu.parentMenu);
  });

  // Bad USB Menu
  badusbMenu.parentMenu = &mainMenu;
  addNodes(&badusbMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(badusbMenu.parentMenu);
  });
  addNodes(&badusbMenu, text_table1[36], WHITE, NULL, TEST_BAD_USB_ICO, [this]() {
    // a32u4_obj.test();
  });
  #ifdef HAS_ILI9341
    addNodes(&badusbMenu, text_table1[37], WHITE, NULL, BAD_USB_ICO, [this](){
      display_obj.clearScreen(); 
      wifi_scan_obj.currentScanMode = LV_ADD_SSID; 
      wifi_scan_obj.StartScan(LV_ADD_SSID, WHITE); 
      writeBadUSB();
    });
  #endif

  // General apps menu
  generalMenu.parentMenu = &mainMenu;
  addNodes(&generalMenu, text09, WHITE, NULL, 0, [this]() {
    display_obj.draw_tft = false;
    changeMenu(generalMenu.parentMenu);
  });
  addNodes(&generalMenu, text_table1[38], WHITE, NULL, DRAW, [this]() {
    display_obj.clearScreen();
    display_obj.setupDraw();
    display_obj.draw_tft = true;
  });

  // Device menu
  deviceMenu.parentMenu = &mainMenu;
  addNodes(&deviceMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(deviceMenu.parentMenu);
  });
  //addNodes(&deviceMenu, "Update Firmware", TFT_ORANGE, NULL, UPDATE, [this](){wifi_scan_obj.currentScanMode = OTA_UPDATE; changeMenu(&updateMenu); web_obj.setupOTAupdate();});
  addNodes(&deviceMenu, text_table1[15], WHITE, NULL, UPDATE, [this]() {
    wifi_scan_obj.currentScanMode = OTA_UPDATE;
    changeMenu(&whichUpdateMenu);
  });

  addNodes(&deviceMenu, text_table1[16], WHITE, NULL, LANGUAGE, [this]() {

    wifi_scan_obj.currentScanMode = SHOW_INFO;
   changeMenu(&languageMenu);   
  });
  addNodes(&deviceMenu, text_table1[17], WHITE, NULL, DEVICE_INFO, [this]() {
    wifi_scan_obj.currentScanMode = SHOW_INFO;
    changeMenu(&infoMenu);
    wifi_scan_obj.RunInfo();
  });
  addNodes(&deviceMenu, text08, WHITE, NULL, KEYBOARD_ICO, [this]() {
    changeMenu(&settingsMenu);
  });
  /*addNodes(&deviceMenu, text08, WHITE, NULL, KEYBOARD_ICO, [this](){
    display_obj.clearScreen(); 
    wifi_scan_obj.currentScanMode = LV_ADD_SSID; 
    wifi_scan_obj.StartScan(LV_ADD_SSID, TFT_RED);  
    displaySettingsGFX();
  });*/

  // Settings menu
  // Device menu
  settingsMenu.parentMenu = &deviceMenu;
  addNodes(&settingsMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(settingsMenu.parentMenu);
  });
  for (int i = 0; i < settings_obj.getNumberSettings(); i++) {
    if (this->callSetting(settings_obj.setting_index_to_name(i)) == "bool")
      addNodes(&settingsMenu, settings_obj.setting_index_to_name(i), WHITE, NULL, 0, [this, i]() {
      settings_obj.toggleSetting(settings_obj.setting_index_to_name(i));
      changeMenu(&specSettingMenu);
      //this->callSetting(settings_obj.setting_index_to_name(i));
      this->displaySetting(settings_obj.setting_index_to_name(i), &settingsMenu, i + 1);
    }, settings_obj.loadSetting<bool>(settings_obj.setting_index_to_name(i)));
  }

  // Specific setting menu
  specSettingMenu.parentMenu = &settingsMenu;
  addNodes(&specSettingMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(specSettingMenu.parentMenu);
  });
 
  // Select update
  whichUpdateMenu.parentMenu = &deviceMenu;
  addNodes(&whichUpdateMenu, text09, WHITE, NULL, 0, [this]() {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    changeMenu(whichUpdateMenu.parentMenu);
  });
  addNodes(&whichUpdateMenu, text_table1[39], WHITE, NULL, WEB_UPDATE, [this]() {
    wifi_scan_obj.currentScanMode = OTA_UPDATE;
    changeMenu(&updateMenu);
    web_obj.setupOTAupdate();
  });
  #ifndef WRITE_PACKETS_SERIAL
    if (sd_obj.supported) addNodes(&whichUpdateMenu, text_table1[40], WHITE, NULL, SD_UPDATE, [this]() {
      wifi_scan_obj.currentScanMode = OTA_UPDATE;
      changeMenu(&confirmMenu);
    });
  #endif
  addNodes(&whichUpdateMenu, text_table1[41], WHITE, NULL, ESP_UPDATE_ICO, [this]() {
    wifi_scan_obj.currentScanMode = ESP_UPDATE;
    changeMenu(&espUpdateMenu);
    esp_obj.RunUpdate();
  });

  // ESP Update Menu
  espUpdateMenu.parentMenu = &whichUpdateMenu;
  addNodes(&espUpdateMenu, text09, WHITE, NULL, 0, [this]() {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    esp_obj.bootRunMode();
    changeMenu(espUpdateMenu.parentMenu);
  });

  // Confirm SD update menu
  confirmMenu.parentMenu = &whichUpdateMenu;
  addNodes(&confirmMenu, text09, WHITE, NULL, 0, [this]() {
    changeMenu(confirmMenu.parentMenu);
  });
  //addNodes(&confirmMenu, "Yes", TFT_ORANGE, NULL, UPDATE, [this](){wifi_scan_obj.currentScanMode = OTA_UPDATE; changeMenu(&updateMenu); sd_obj.runUpdate();});
  addNodes(&confirmMenu, text14, WHITE, NULL, UPDATE, [this]() {
    wifi_scan_obj.currentScanMode = OTA_UPDATE;
    changeMenu(&failedUpdateMenu);
    sd_obj.runUpdate();
  });

  // Web Update
  updateMenu.parentMenu = &deviceMenu;
  addNodes(&updateMenu, text09, WHITE, NULL, 0, [this]() {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    changeMenu(updateMenu.parentMenu);
    WiFi.softAPdisconnect(true);
    web_obj.shutdownServer();
  });
  //addNodes(&updateMenu, text09, WHITE, NULL, 0, [this](){wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF; changeMenu(updateMenu.parentMenu);});

  // Failed update menu
  failedUpdateMenu.parentMenu = &whichUpdateMenu;
  addNodes(&failedUpdateMenu, text09, WHITE, NULL, 0, [this]() {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    changeMenu(failedUpdateMenu.parentMenu);
  });

  // Device info menu
  infoMenu.parentMenu = &deviceMenu;
  addNodes(&infoMenu, text09, WHITE, NULL, 0, [this]() {
    wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
    changeMenu(infoMenu.parentMenu);
  });
  //language info menu
  languageMenu.parentMenu = &deviceMenu;
    addNodes(&languageMenu, text09, WHITE, NULL, 0, [this]() {
      wifi_scan_obj.currentScanMode = WIFI_SCAN_OFF;
      changeMenu(infoMenu.parentMenu);
    });
  // Set the current menu to the mainMenu
  changeMenu(&mainMenu);

  this->initTime = millis();
}

// Function to change menu
void MenuFunctions::changeMenu(Menu * menu)
{
  Serial.println("initScrollValues");
  display_obj.initScrollValues();
  Serial.println("setupScrollArea");
  display_obj.setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);
  //display_obj.display.init();
  // display_obj.display.begin();

  current_menu = menu;
  Serial.println("buildButtons");

  buildButtons(menu);
  
  Serial.println("displayCurrentMenu");
  displayCurrentMenu();
}

// Function to show all MenuNodes in a Menu
void MenuFunctions::showMenuList(Menu * menu, int layer)
{
  // Iterate through all of the menu nodes in the menu
  for (uint8_t i = 0; i < menu->list->size(); i++)
  {
    // Depending on layer, indent
    for (uint8_t x = 0; x < layer * 4; x++)
      Serial.print(" ");
    Serial.print("Node: ");
    Serial.println(menu->list->get(i).name);
  }
  Serial.println();
}


// Function to add MenuNodes to a menu
void MenuFunctions::addNodes(Menu * menu, String name, uint16_t color, Menu * child, int place, std::function<void()> callable, bool selected, String command)
{
  Adafruit_GFX_Button new_button;
  menu->list->add(MenuNode{name, command, color, place, &new_button, selected, callable});
  //menu->list->add(MenuNode{name, color, place, callable});
}

void MenuFunctions::buildButtons(Menu * menu, int starting_index)
{
  Serial.println("Bulding buttons...");
  if (menu->list != NULL)
  {
    //for (int i = 0; i < sizeof(key); i++)
    //  key[i] = NULL;
    for (uint8_t i = 0; i < menu->list->size(); i++)
    {
      Adafruit_GFX_Button new_button;
      char buf[menu->list->get(starting_index + i).name.length() + 1] = {};
      menu->list->get(starting_index + i).name.toCharArray(buf, menu->list->get(starting_index + i).name.length() + 1);
      display_obj.key[i].initButton(&display_obj.display,
                                    KEY_X + 0 * (KEY_W + KEY_SPACING_X),
                                    KEY_Y + i * (KEY_H + KEY_SPACING_Y), // x, y, w, h, outline, fill, text
                                    KEY_W,
                                    KEY_H,
                                    WHITE, // Outline
                                    WHITE, // Fill
                                    menu->list->get(starting_index + i).color, // Text
                                    buf,
                                    KEY_TEXTSIZE);

      //display_obj.key[i].setLabelDatum(BUTTON_PADDING - (KEY_W / 2), 2, ML_DATUM);
    }
  }
}


void MenuFunctions::displayCurrentMenu()
{
  //Serial.println(F("Displaying current menu..."));
  display_obj.clearScreen();
  display_obj.display.setTextColor(WHITE);
  this->drawStatusBar();

  if (current_menu->list != NULL)
  {
    #ifdef HAS_ILI9341
      display_obj.display.setFont(MENU_FONT);
    #endif

    #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
      display_obj.display.setFont(NULL);
      display_obj.display.setTextSize(1);
    #endif
    for (uint8_t i = 0; i < current_menu->list->size(); i++)
    {
      #ifdef HAS_ILI9341
        if (!current_menu->list->get(i).selected)
          display_obj.key[i].drawButton(current_menu->list->get(i).name);
        else
          display_obj.key[i].drawButton(true, current_menu->list->get(i).name);
        
        if (current_menu->list->get(i).name != text09)
          display_obj.display.drawXBitmap(0,
                                      KEY_Y + i * (KEY_H + KEY_SPACING_Y) - (ICON_H / 2),
                                      menu_icons[current_menu->list->get(i).icon],
                                      ICON_W,
                                      ICON_H,
                                      TFT_BLACK,
                                      current_menu->list->get(i).color);

      #endif

      #if defined(MARAUDER_MINI) || defined(MARAUDER_M5STICKC)
        if ((current_menu->selected == i) || (current_menu->list->get(i).selected))
          display_obj.key[i].drawButton(true, current_menu->list->get(i).name);
        else 
          display_obj.key[i].drawButton(current_menu->list->get(i).name);
      #endif
    }
    display_obj.display.setFont(NULL);
  }
}

#endif
