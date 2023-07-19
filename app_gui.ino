#include "gui_i.h"



// static void gui_redraw(Gui* gui) {
//     // furi_assert(gui);
//     gui_lock(gui);

//     do {
//         if(gui->direct_draw) break;

//         canvas_reset(gui->canvas);

//         if(gui->lockdown) {
//             gui_redraw_desktop(gui);
//             bool need_attention =
//                 (gui_view_port_find_enabled(gui->layers[GuiLayerWindow]) != 0 ||
//                  gui_view_port_find_enabled(gui->layers[GuiLayerFullscreen]) != 0);
//             gui_redraw_status_bar(gui, need_attention);
//         } else {
//             if(!gui_redraw_fs(gui)) {
//                 if(!gui_redraw_window(gui)) {
//                     gui_redraw_desktop(gui);
//                 }
//                 gui_redraw_status_bar(gui, false);
//             }
//         }

//         canvas_commit(gui->canvas);
//         for
//             M_EACH(p, gui->canvas_callback_pair, CanvasCallbackPairArray_t) {
//                 p->callback(
//                     canvas_get_buffer(gui->canvas),
//                     canvas_get_buffer_size(gui->canvas),
//                     canvas_get_orientation(gui->canvas),
//                     p->context);
//             }
//     } while(false);

//     gui_unlock(gui);
// }


Gui* principal_gui;
void app_gui_setup() {
    principal_gui = gui_setup();
    principal_gui->direct_draw = false;
    principal_gui->lockdown = true;

    // canvas_set_color(principal_gui->canvas, WHITE);
    // canvas_clear(principal_gui->canvas);
    // canvas_draw_str_aligned(principal_gui->canvas, 128/2, 64/2,  AlignCenter, AlignTop, "Hola Mundo");
    // canvas_commit(principal_gui->canvas);
}

void app_gui_loop() {
    gui_loop(principal_gui);
}