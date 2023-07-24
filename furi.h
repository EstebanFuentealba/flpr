#pragma once

#ifndef FURI_H
#define FURI_H

#include <stdlib.h>
#include "common_defines.h"
#include "event_flag.h"
#include "kernel.h"
// #include "memmgr.h"
// #include "memmgr_heap.h"
// #include "message_queue.h"
#include "mutex.h"
#include "pubsub.h"
#include "record.h"
// #include "semaphore.h"
#include "thread.h"
#include "timer.h"

#include "furi_hal_gpio.h"
#include "furi_string.h"

#ifdef __cplusplus
extern "C" {
#endif

void furi_init();

void furi_run();

#ifdef __cplusplus
}
#endif

#endif // FURI_H