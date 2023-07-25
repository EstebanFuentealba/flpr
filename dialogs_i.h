#pragma once
#include "dialogs.h"
#include "dialogs_message.h"
#include "view_holder.h"
#include "file_browser.h"

#ifdef __cplusplus
extern "C" {
#endif
struct DialogsApp {
    FuriMessageQueue* message_queue;
};

#ifdef __cplusplus
}
#endif