/**
 * @file furi_string.h
 * Furi string primitive
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Furi string primitive.
 */
// typedef struct FuriString FuriString;
typedef struct FuriString {
    char* string;
    size_t length; // Nuevo miembro para almacenar la longitud
};


/**
 * @brief Allocate new FuriString.
 * @return FuriString* 
 */
FuriString* furi_string_alloc();


/**
 * @brief Check that string is empty or not
 * @param string 
 * @return bool
 */
bool furi_string_empty(const FuriString* string);


/**
 * @brief Reset string.
 * Make the string empty.
 * @param s 
 */
void furi_string_reset(FuriString* string);

/**
 * @brief Set the string to the other C string.
 * Set the string to the source C string.
 * @param string 
 * @param source 
 */
void furi_string_set_str(FuriString* string, const char source[]);


/**
 * @brief Allocate new FuriString and set it to string.
 * Allocate & Set the string a to the string.
 * @param source 
 * @return FuriString* 
 */
FuriString* furi_string_alloc_set(const FuriString* source);

/**
 * @brief Allocate new FuriString and set it to C string.
 * Allocate & Set the string a to the C string.
 * @param cstr_source 
 * @return FuriString* 
 */
FuriString* furi_string_alloc_set_str(const char cstr_source[]);

/**
 * @brief Set the string to the other string.
 * Set the string to the source string.
 * @param string 
 * @param source 
 */
void furi_string_set(FuriString* string, FuriString* source);


/**
 * @brief Free FuriString.
 * @param string 
 */
void furi_string_free(FuriString* string);


/**
 * @brief Return the string view a classic C string.
 * @param string 
 * @return const char* 
 */
const char* furi_string_get_cstr(const FuriString* string);


/**
 * @brief Allocate new FuriString and printf to it.
 * Initialize and set a string to the given formatted value.
 * @param format 
 * @param ... 
 * @return FuriString* 
 */
FuriString* furi_string_alloc_printf(const char format[], ...);


/**
 * @brief Trim the string left to the first 'index' bytes.
 * @param string 
 * @param index 
 */
void furi_string_left(FuriString* string, size_t index);

/**
 * @brief Get string size (usually length, but not for UTF-8)
 * @param string 
 * @return size_t 
 */
size_t furi_string_size(const FuriString* string);

/**
 * @brief Append a string to the string.
 * Concatenate the string with the other string.
 * @param string_1 
 * @param string_2 
 */
void furi_string_cat(FuriString* string_1, const FuriString* string_2);

/**
 * @brief Append a C string to the string.
 * Concatenate the string with the C string.
 * @param string_1 
 * @param cstring_2 
 */
void furi_string_cat_str(FuriString* string_1, const char cstring_2[]);

#ifdef __cplusplus
}
#endif