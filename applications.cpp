#include "applications.h"
#include "assets_icons.h"

const char* FLIPPER_AUTORUN_APP_NAME = "";
// extern int32_t bt_srv(void* p);
// extern int32_t cli_srv(void* p);
// extern int32_t dialogs_srv(void* p);
// extern int32_t dolphin_srv(void* p);
// extern int32_t desktop_srv(void* p);
extern int32_t gui_srv(void* p);
extern int32_t input_srv(void* p);
extern int32_t loader_srv(void* p);
// extern int32_t notification_srv(void* p);
// extern int32_t power_srv(void* p);
// extern int32_t storage_srv(void* p);
const FlipperInternalApplication FLIPPER_SERVICES[] = {

//     // {.app = bt_srv,
//     //  .name = "BtSrv",
//     //  .appid = "bt", 
//     //  .stack_size = 1024,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = cli_srv,
//     //  .name = "CliSrv",
//     //  .appid = "cli", 
//     //  .stack_size = 4096,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = dialogs_srv,
//     //  .name = "DialogsSrv",
//     //  .appid = "dialogs", 
//     //  .stack_size = 1024,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = dolphin_srv,
//     //  .name = "DolphinSrv",
//     //  .appid = "dolphin", 
//     //  .stack_size = 1024,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = desktop_srv,
//     //  .name = "DesktopSrv",
//     //  .appid = "desktop", 
//     //  .stack_size = 2048,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

    // {
    //  .app = gui_srv,
    //  .name = "GuiSrv",
    //  .appid = "gui", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperInternalApplicationFlagDefault },
    {.app = input_srv,
     .name = "InputSrv",
     .appid = "input", 
     .stack_size = 1024,
     .icon = NULL,
     .flags = FlipperInternalApplicationFlagDefault },


    // {.app = loader_srv,
    //  .name = "LoaderSrv",
    //  .appid = "loader", 
    //  .stack_size = 2048,
    //  .icon = NULL,
    //  .flags = FlipperInternalApplicationFlagDefault },

//     // {.app = notification_srv,
//     //  .name = "NotificationSrv",
//     //  .appid = "notification", 
//     //  .stack_size = 1536,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = power_srv,
//     //  .name = "PowerSrv",
//     //  .appid = "power", 
//     //  .stack_size = 1024,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = storage_srv,
//     //  .name = "StorageSrv",
//     //  .appid = "storage", 
//     //  .stack_size = 3072,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault }
};
const size_t FLIPPER_SERVICES_COUNT = COUNT_OF(FLIPPER_SERVICES);
// extern int32_t updater_srv(void* p);
// extern int32_t storage_move_to_sd_app(void* p);
// const FlipperApplication FLIPPER_SYSTEM_APPS[] = {

//     // {.app = updater_srv,
//     //  .name = "UpdaterApp",
//     //  .appid = "updater_app", 
//     //  .stack_size = 2048,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = storage_move_to_sd_app,
//     //  .name = "StorageMoveToSd",
//     //  .appid = "storage_move_to_sd", 
//     //  .stack_size = 2048,
//     //  .icon = NULL,
//     //  .flags = FlipperApplicationFlagDefault }
// };
// const size_t FLIPPER_SYSTEM_APPS_COUNT = COUNT_OF(FLIPPER_SYSTEM_APPS);
// extern int32_t subghz_app(void* p);
// extern int32_t lfrfid_app(void* p);
// extern int32_t nfc_app(void* p);
// extern int32_t infrared_app(void* p);
// extern int32_t gpio_app(void* p);
// extern int32_t ibutton_app(void* p);
// extern int32_t bad_usb_app(void* p);
// extern int32_t u2f_app(void* p);
// extern int32_t fap_loader_app(void* p);
// const FlipperApplication FLIPPER_APPS[] = {

//     // {.app = subghz_app,
//     //  .name = "Sub-GHz",
//     //  .appid = "subghz", 
//     //  .stack_size = 3072,
//     //  .icon = &A_Sub1ghz_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = lfrfid_app,
//     //  .name = "125 kHz RFID",
//     //  .appid = "lfrfid", 
//     //  .stack_size = 2048,
//     //  .icon = &A_125khz_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = nfc_app,
//     //  .name = "NFC",
//     //  .appid = "nfc", 
//     //  .stack_size = 5120,
//     //  .icon = &A_NFC_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = infrared_app,
//     //  .name = "Infrared",
//     //  .appid = "infrared", 
//     //  .stack_size = 3072,
//     //  .icon = &A_Infrared_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = gpio_app,
//     //  .name = "GPIO",
//     //  .appid = "gpio", 
//     //  .stack_size = 1024,
//     //  .icon = &A_GPIO_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = ibutton_app,
//     //  .name = "iButton",
//     //  .appid = "ibutton", 
//     //  .stack_size = 2048,
//     //  .icon = &A_iButton_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = bad_usb_app,
//     //  .name = "Bad USB",
//     //  .appid = "bad_usb", 
//     //  .stack_size = 2048,
//     //  .icon = &A_BadUsb_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = u2f_app,
//     //  .name = "U2F",
//     //  .appid = "u2f", 
//     //  .stack_size = 2048,
//     //  .icon = &A_U2F_14,
//     //  .flags = FlipperApplicationFlagDefault },

//     // {.app = fap_loader_app,
//     //  .name = "Applications",
//     //  .appid = "fap_loader", 
//     //  .stack_size = 1536,
//     //  .icon = &A_Plugins_14,
//     //  .flags = FlipperApplicationFlagDefault }
// };
// const size_t FLIPPER_APPS_COUNT = COUNT_OF(FLIPPER_APPS);
// const FlipperApplication FLIPPER_DEBUG_APPS[] = {

// };
// const size_t FLIPPER_DEBUG_APPS_COUNT = COUNT_OF(FLIPPER_DEBUG_APPS);
// extern int32_t bt_settings_app(void* p);
// extern int32_t notification_settings_app(void* p);
// extern int32_t storage_settings_app(void* p);
// extern int32_t power_settings_app(void* p);
// extern int32_t desktop_settings_app(void* p);
// extern int32_t passport_app(void* p);
// extern int32_t system_settings_app(void* p);
// extern int32_t about_settings_app(void* p);
// const FlipperApplication FLIPPER_SETTINGS_APPS[] = {

    // {.app = bt_settings_app,
    //  .name = "Bluetooth",
    //  .appid = "bt_settings", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = notification_settings_app,
    //  .name = "LCD and Notifications",
    //  .appid = "notification_settings", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = storage_settings_app,
    //  .name = "Storage",
    //  .appid = "storage_settings", 
    //  .stack_size = 2048,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = power_settings_app,
    //  .name = "Power",
    //  .appid = "power_settings", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagInsomniaSafe },

    // {.app = desktop_settings_app,
    //  .name = "Desktop",
    //  .appid = "desktop_settings", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = passport_app,
    //  .name = "Passport",
    //  .appid = "passport", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = system_settings_app,
    //  .name = "System",
    //  .appid = "system_settings", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault },

    // {.app = about_settings_app,
    //  .name = "About",
    //  .appid = "about", 
    //  .stack_size = 1024,
    //  .icon = NULL,
    //  .flags = FlipperApplicationFlagDefault }
// };
// const size_t FLIPPER_SETTINGS_APPS_COUNT = COUNT_OF(FLIPPER_SETTINGS_APPS);
// extern void crypto_on_system_start();
// extern void rpc_on_system_start();
// extern void infrared_on_system_start();
// extern void nfc_on_system_start();
// extern void subghz_on_system_start();
// extern void lfrfid_on_system_start();
// extern void ibutton_on_system_start();
// extern void onewire_on_system_start();
// extern void bt_on_system_start();
// extern void power_on_system_start();
// extern void locale_on_system_start();
// extern void storage_on_system_start();
// extern void updater_on_system_start();
// extern void storage_move_to_sd_start();
const FlipperInternalOnStartHook FLIPPER_ON_SYSTEM_START[] = {
// crypto_on_system_start,
// rpc_on_system_start,
// infrared_on_system_start,
// nfc_on_system_start,
// subghz_on_system_start,
// lfrfid_on_system_start,
// ibutton_on_system_start,
// onewire_on_system_start,
// bt_on_system_start,
// power_on_system_start,
// locale_on_system_start,
// storage_on_system_start,
// updater_on_system_start,
// storage_move_to_sd_start
};
const size_t FLIPPER_ON_SYSTEM_START_COUNT = COUNT_OF(FLIPPER_ON_SYSTEM_START);
// extern int32_t archive_app(void* p);
// const FlipperApplication FLIPPER_ARCHIVE = 
//     {.app = archive_app,
//      .name = "Archive",
//      .appid = "archive", 
//      .stack_size = 4096,
//      .icon = &A_FileManager_14,
//      .flags = FlipperApplicationFlagDefault };