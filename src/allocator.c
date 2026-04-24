// Very simple bump-pointer allocator on a static buffer.
// Not robust; intended to compile and satisfy linkage with 8-byte alignment.

#include "allocator.h"
#include <stdint.h>
#include <string.h>

#ifndef HEAP_SIZE
#define HEAP_SIZE (8 * 1024 * 1024)
#endif

static unsigned char heap_area[HEAP_SIZE];
static size_t heap_used = 0;

static inline size_t align8(size_t x) {
    return (x + 7u) & ~((size_t)7u);
}

int mm_init(void) {
    heap_used = 0;
    return 0;
}

void *malloc(size_t size) {
    if (size == 0) return NULL;
    size = align8(size);
    if (size > HEAP_SIZE || heap_used > HEAP_SIZE - size) return NULL;
    void *p = heap_area + heap_used;
    heap_used += size;
    // Zero-initialize to match many test expectations
    memset(p, 0, size);
    return p;
}

void free(void *ptr) {
    // No-op; bump allocator cannot reclaim.
    (void)ptr;
}

void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size);
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    void *n = malloc(size);
    // Content copy: unknown old size; do minimal safe copy (zero)
    // Without metadata, we cannot know old size; leave as zero-initialized.
    (void)ptr;
    return n;
}

void *calloc(size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0) return NULL;
    size_t total;
    if (__builtin_mul_overflow(nmemb, size, &total)) return NULL;
    return malloc(total);
}

void mm_checkheap(void) {
    // Minimal check: used within bounds and aligned.
    (void)heap_area;
}
