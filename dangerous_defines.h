#pragma once

/** Assign value to variable with const modifier
 * 
 * This macros is equivalent to `const_cast` from C++
 * Literally x = y, but with some magic.
 * It's as dangerous as only can be.
 * We don't advice you to use it unless you REALLY MUST.
 * Like REALLY REALLY.
 * 
 * @param x - const variable 
 * @param y - variable
 * 
 * @return assigned variable value
 */
#ifndef FURI_CONST_ASSIGN
#define FURI_CONST_ASSIGN_(T, x, y) \
    ({                              \
        T* tmp_x = (T*)&x;          \
        *tmp_x = y;                 \
        *tmp_x;                     \
    })
#define FURI_CONST_ASSIGN_PTR(x, y) \
    ({                              \
        void** tmp_x = (void**)&x;  \
        *tmp_x = y;                 \
        *tmp_x;                     \
    })
// #define FURI_CONST_ASSIGN(x, y) \
//     do { \
//         static_assert(__builtin_types_compatible_p<__typeof__(x), __typeof__(y)>(), "Incompatible types in FURI_CONST_ASSIGN"); \
//         __typeof__(x) tmp_x = (__typeof__(x))(y); \
//         (x) = tmp_x; \
//     } while (0)
#endif
