#include "view_i.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

View* view_alloc() {
    View* view = (View*)malloc(sizeof(View));
    view->orientation = ViewOrientationHorizontal;
    return view;
}

void view_free(View* view) {
    //furi_assert(view);
    view_free_model(view);
    // free(view);
}
void view_icon_animation_callback(IconAnimation* instance, void* context) {
    // UNUSED(instance);
    // furi_assert(context);
    View* view = (View*)context;
    if(view->update_callback) {
        view->update_callback(view, view->update_callback_context);
    }
}
void view_tie_icon_animation(View* view, IconAnimation* icon_animation) {
    //furi_assert(view);
    icon_animation_set_update_callback(icon_animation, view_icon_animation_callback, view);
}

void view_set_draw_callback(View* view, ViewDrawCallback callback) {
    //furi_assert(view);
    view->draw_callback = callback;
}

void view_set_input_callback(View* view, ViewInputCallback callback) {
    //furi_assert(view);
    view->input_callback = callback;
}

void view_set_custom_callback(View* view, ViewCustomCallback callback) {
    //furi_assert(view);
    view->custom_callback = callback;
}

void view_set_previous_callback(View* view, ViewNavigationCallback callback) {
    //furi_assert(view);
    view->previous_callback = callback;
}

void view_set_enter_callback(View* view, ViewCallback callback) {
    //furi_assert(view);
    view->enter_callback = callback;
}

void view_set_exit_callback(View* view, ViewCallback callback) {
    //furi_assert(view);
    view->exit_callback = callback;
}

void view_set_update_callback(View* view, ViewUpdateCallback callback) {
    //furi_assert(view);
    view->update_callback = callback;
}

void view_set_update_callback_context(View* view, void* context) {
    //furi_assert(view);
    view->update_callback_context = context;
}

void view_set_context(View* view, void* context) {
    //furi_assert(view);
    view->context = context;
}

void view_set_orientation(View* view, ViewOrientation orientation) {
    //furi_assert(view);
    view->orientation = orientation;
}

void view_allocate_model(View* view, ViewModelType type, size_t size) {
    //furi_assert(view);
    //furi_assert(size > 0);
    //furi_assert(view->model_type == ViewModelTypeNone);
    //furi_assert(view->model == NULL);
    view->model_type = type;
    Serial.print("[view] model_type: ");
    Serial.println(type);
    if (view->model_type == ViewModelTypeLockFree) {
    view->model = malloc(size);
    } else if (view->model_type == ViewModelTypeLocking) {
        ViewModelLocking* model = (ViewModelLocking*)malloc(sizeof(ViewModelLocking));
        model->mutex = furi_mutex_alloc(FuriMutexTypeRecursive);
        //furi_check(model->mutex);
        model->data = malloc(size);
        view->model = model;
    } else {
        //furi_crash(NULL);
    }
}

void view_free_model(View* view) {
//   view->model = NULL;
    // free(view->model);
    //furi_assert(view);
    if (view->model_type == ViewModelTypeNone) {
        return;
    } else if (view->model_type == ViewModelTypeLockFree) {
        free(view->model);
    } else if (view->model_type == ViewModelTypeLocking) {
        ViewModelLocking* model = (ViewModelLocking*)view->model;
        vSemaphoreDelete(model->mutex);
        free(model->data);
        free(model);
        view->model = NULL;
    } else {
        //furi_crash(NULL);
    }
}

void* view_get_model(View* view) {
    //furi_assert(view);
    if(view->model_type == ViewModelTypeLocking) {
        ViewModelLocking* model = (ViewModelLocking*)(view->model);
        Serial.println("[view] check xSemaphoreGetMutexHolder mutex");
        if (xSemaphoreGetMutexHolder((SemaphoreHandle_t)model->mutex)) {
            Serial.println("[view] is valid xSemaphoreGetMutexHolder mutex");
            if(furi_mutex_acquire(model->mutex, 10) == FuriStatusOk) {
                Serial.println("[view] furi_mutex_acquired");
                return model->data;
            }
        } else {
            Serial.println("El semáforo no es una instancia de xSemaphoreCreateMutex");

        }
        // furi_check(furi_mutex_acquire(model->mutex, FuriWaitForever) == FuriStatusOk);
    }
    return view->model;
}

void view_commit_model(View* view, bool update) {
    //furi_assert(view);
    view_unlock_model(view);
    //  TODO: Implement view update_callback
    if(update && view->update_callback) {
        view->update_callback(view, view->update_callback_context);
    }
}

void view_icon_animation_callback(Icon* instance, void* context) {
    // UNUSED(instance);
    //furi_assert(context);
    View* view = (View*)context;
    if(view->update_callback) {
        view->update_callback(view, view->update_callback_context);
    }
}

void view_unlock_model(View* view) {
    //furi_assert(view);
    if(view->model_type == ViewModelTypeLocking) {
        ViewModelLocking* model = (ViewModelLocking*)(view->model);
        furi_mutex_release(model->mutex);
        // furi_check(furi_mutex_release(model->mutex) == FuriStatusOk);
    }
}

void view_draw(View* view, Canvas* canvas) {
    //furi_assert(view);
    if(view->draw_callback) {
        void* data = view_get_model(view);
        view->draw_callback(canvas, data);
        view_unlock_model(view);
    }
}

bool view_input(View* view, InputEvent* event) {
    //furi_assert(view);
    if(view->input_callback) {
        return view->input_callback(event, view->context);
    } else {
        return false;
    }
}

bool view_custom(View* view, uint32_t event) {
    //furi_assert(view);
    if(view->custom_callback) {
        return view->custom_callback(event, view->context);
    } else {
        return false;
    }
}

uint32_t view_previous(View* view) {
    //furi_assert(view);
    if(view->previous_callback) {
        return view->previous_callback(view->context);
    } else {
        return VIEW_IGNORE;
    }
}

void view_enter(View* view) {
    //furi_assert(view);
    if(view->enter_callback) view->enter_callback(view->context);
}

void view_exit(View* view) {
    //furi_assert(view);
    if(view->exit_callback) view->exit_callback(view->context);
}
