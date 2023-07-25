#include "input.h"
#include "record.h"
#include "pubsub.h"


// FuriPubSub* input_events;


void gui_input_events_callback(const void* value, void* ctx) {
    // ////furi_assert(value);
    // ////furi_assert(ctx);
    // Serial.println("[marauder] gui_input_events_callback");
    // // Serial.print("value: ");
    // // Serial.println(value);
    // InputEvent* input_event = (InputEvent*)value;
    // if(input_event->type = InputTypeShort) {
    //   if(input_event->key == InputKeyDown) {
    //     Serial.println("[marauder] DOWN");
    //   }
    // }
}


void marauder_setup() {

    // Serial.println("Marauder Setup");
    // input_events = furi_record_open(RECORD_INPUT_EVENTS);
    // Serial.println("[marauder] furi_record_open RECORD_INPUT_EVENTS");

    // furi_pubsub_subscribe(input_events, gui_input_events_callback, NULL);
    // Serial.println("[marauder] furi_pubsub_subscribe");
}

void marauder_loop() {
    // Serial.println("Marauder Loop");
    // InputEvent input_event;
    // while(furi_message_queue_get(gui->input_queue, &input_event, 0) == FuriStatusOk) {
    //     gui_input(gui, &input_event);
    // }
}