#include <Arduino.h>
#include <string.h>
#include "furi_string.h"

extern "C" {

FuriString* furi_string_alloc() {
    FuriString* string = (FuriString*)malloc(sizeof(FuriString));
    string->string = nullptr;
    string->length = 0;
    return string;
}

bool furi_string_empty(const FuriString* string) {
    return string->string == nullptr || string->string[0] == '\0';
}

void furi_string_reset(FuriString* string) {
    free(string->string);
    string->string = nullptr;
    string->length = 0;
}

void furi_string_set_str(FuriString* string, const char source[]) {
    free(string->string);
    string->string = strdup(source);
    string->length = strlen(source);
}

FuriString* furi_string_alloc_set(const FuriString* source) {
    FuriString* string = (FuriString*)malloc(sizeof(FuriString));
    if (source->string != nullptr) {
        string->string = strdup(source->string);
        string->length = source->length;
    } else {
        string->string = nullptr;
        string->length = 0;
    }
    return string;
}

FuriString* furi_string_alloc_set_str(const char* source) {
    FuriString* string = (FuriString*)malloc(sizeof(FuriString));
    if (source != nullptr) {
        string->string = strdup(source);
        string->length = strlen(source);
    } else {
        string->string = nullptr;
        string->length = 0;
    }
    return string;
}

void furi_string_set(FuriString* string, FuriString* source) {
    free(string->string);
    if (source->string != nullptr) {
        string->string = strdup(source->string);
        string->length = source->length;
    } else {
        string->string = nullptr;
        string->length = 0;
    }
}

void furi_string_free(FuriString* string) {
    free(string->string);
    free(string);
}

const char* furi_string_get_cstr(const FuriString* string) {
    return string->string;
}

FuriString* furi_string_alloc_printf(const char format[], ...) {
    FuriString* string = furi_string_alloc();

    va_list args;
    va_start(args, format);

    int size = vsnprintf(NULL, 0, format, args);

    if (size < 0) {
        furi_string_reset(string);
        return string;
    }

    size += 1;

    char* buffer = (char*)malloc(size * sizeof(char));

    vsnprintf(buffer, size, format, args);

    furi_string_set_str(string, buffer);

    free(buffer);

    va_end(args);

    return string;
}

void furi_string_left(FuriString* string, size_t index) {
    if (string == NULL || string->string == NULL || index >= string->length) {
        return;
    }

    string->length = index;
    string->string[index] = '\0';
}

size_t furi_string_size(const FuriString* string) {
    return string->length;
}

void furi_string_cat(FuriString* string_1, const FuriString* string_2) {
    if (string_1 == NULL || string_2 == NULL || string_2->string == NULL) {
        return;
    }

    size_t len_1 = string_1->length;
    size_t len_2 = string_2->length;
    size_t total_len = len_1 + len_2;

    char* new_string = (char*)malloc((total_len + 1) * sizeof(char));

    if (len_1 > 0) {
        memcpy(new_string, string_1->string, len_1);
    }

    if (len_2 > 0) {
        memcpy(new_string + len_1, string_2->string, len_2);
    }

    new_string[total_len] = '\0';

    free(string_1->string);
    string_1->string = new_string;
    string_1->length = total_len;
}

void furi_string_cat_str(FuriString* string_1, const char* string_2) {
    if (string_1 == NULL || string_2 == NULL) {
        return;
    }

    size_t len_1 = string_1->length;
    size_t len_2 = strlen(string_2);
    size_t total_len = len_1 + len_2;

    char* new_string = (char*)malloc((total_len + 1) * sizeof(char));

    if (len_1 > 0) {
        memcpy(new_string, string_1->string, len_1);
    }

    if (len_2 > 0) {
        memcpy(new_string + len_1, string_2, len_2);
    }

    new_string[total_len] = '\0';

    free(string_1->string);
    string_1->string = new_string;
    string_1->length = total_len;
}

} // extern "C"
