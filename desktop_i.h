#pragma once

#include "desktop.h"
#include "animation_manager.h"
#include "desktop_view_pin_timeout.h"
#include "desktop_view_pin_input.h"
#include "desktop_view_locked.h"
#include "desktop_view_main.h"
#include "desktop_view_lock_menu.h"
#include "desktop_view_debug.h"
#include "desktop_view_slideshow.h"
#include "desktop_settings.h"

#include "furi.h"
#include "gui.h"
#include "view_stack.h"
#include "view_dispatcher.h"
#include "popup.h"
#include "scene_manager.h"

#include "loader.h"
// #include "notification_app.h"

#define STATUS_BAR_Y_SHIFT 13

typedef enum {
    DesktopViewIdMain,
    DesktopViewIdLockMenu,
    DesktopViewIdLocked,
    DesktopViewIdDebug,
    DesktopViewIdHwMismatch,
    DesktopViewIdPinInput,
    DesktopViewIdPinTimeout,
    DesktopViewIdSlideshow,
    DesktopViewIdTotal,
} DesktopViewId;

struct Desktop {
    // Scene
    FuriThread* scene_thread;
    // GUI
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;

    Popup* hw_mismatch_popup;
    DesktopLockMenuView* lock_menu;
    DesktopDebugView* debug_view;
    DesktopViewLocked* locked_view;
    DesktopMainView* main_view;
    DesktopViewPinTimeout* pin_timeout_view;
    DesktopSlideshowView* slideshow_view;

    ViewStack* main_view_stack;
    ViewStack* locked_view_stack;

    DesktopSettings settings;
    DesktopViewPinInput* pin_input_view;

    ViewPort* lock_icon_viewport;
    ViewPort* dummy_mode_icon_viewport;
    ViewPort* stealth_mode_icon_viewport;

    AnimationManager* animation_manager;

    Loader* loader;
    // NotificationApp* notification;

    FuriPubSubSubscription* app_start_stop_subscription;
    FuriPubSub* input_events_pubsub;
    FuriPubSubSubscription* input_events_subscription;
    FuriTimer* auto_lock_timer;

    FuriPubSub* status_pubsub;

    bool in_transition;
};

Desktop* desktop_alloc();

void desktop_free(Desktop* desktop);
void desktop_lock(Desktop* desktop);
void desktop_unlock(Desktop* desktop);
void desktop_set_dummy_mode_state(Desktop* desktop, bool enabled);
void desktop_set_stealth_mode_state(Desktop* desktop, bool enabled);

void desktop_setup();
void desktop_loop();