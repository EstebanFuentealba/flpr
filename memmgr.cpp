#include "memmgr.h"
#include "common_defines.h"

#include <cstring>
#include <stddef.h>
#include <stdlib.h>
#include "esp_heap_caps.h"

extern void* pvPortMalloc(size_t xSize);
extern void vPortFree(void* pv);
extern size_t xPortGetFreeHeapSize(void);
extern size_t xPortGetTotalHeapSize(void);
extern size_t xPortGetMinimumEverFreeHeapSize(void);


void* malloc(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_DEFAULT);
}

void free(void* ptr) {
    if(ptr != NULL) {
        //  TODO:
        // heap_caps_free(ptr);
    }
    
}

void* realloc(void* ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    void* p = heap_caps_realloc(ptr, size, MALLOC_CAP_DEFAULT);
    return p;
}

void* calloc(size_t count, size_t size) {
    size_t totalSize = count * size;
    void* p = heap_caps_malloc(totalSize, MALLOC_CAP_DEFAULT);
    if (p != NULL) {
        memset(p, 0, totalSize);
    }
    return p;
}

char* strdup(const char* s) {
    size_t siz = strlen(s) + 1;
    char* y = (char*)heap_caps_malloc(siz, MALLOC_CAP_DEFAULT);
    if (y != NULL) {
        memcpy(y, s, siz);
    }
    return y;
    // size_t siz = strlen(s) + 1;
    // char* y = pvPortMalloc(siz);
    // memcpy(y, s, siz);

    // return y;
}

size_t memmgr_get_free_heap(void) {
    return heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
}

size_t memmgr_get_total_heap(void) {
    return heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
}

size_t memmgr_get_minimum_free_heap(void) {
    return heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
}
void* furi_hal_memory_alloc(size_t size) {
    return malloc(size);
}
void* memmgr_alloc_from_pool(size_t size) {
    void* p = furi_hal_memory_alloc(size);
    if (p == NULL) p = malloc(size);
    return p;
}

size_t memmgr_pool_get_free(void) {
    return heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    // return furi_hal_memory_get_free();
}

// size_t memmgr_pool_get_max_block(void) {
//     return furi_hal_memory_max_pool_block();
// }
size_t memmgr_pool_get_max_block(void) {
    return heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
}

void* aligned_malloc(size_t size, size_t alignment) {
    void* p1;  // original block
    void** p2; // aligned block
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(size + offset)) == NULL) {
        return NULL;
    }
    p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void* p) {
    free(((void**)p)[-1]);
}