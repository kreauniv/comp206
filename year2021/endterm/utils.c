
#include "utils.h"

uint32_t hash_int(uint32_t x) {
    uint32_t a = 1664525;
    uint32_t b = 1013904223;
    const uint8_t *xs = (const uint8_t *)&x;
    uint32_t hash = 0;
    hash = a * (hash ^ xs[0]) + b;
    hash = a * (hash ^ xs[1]) + b;
    hash = a * (hash ^ xs[2]) + b;
    hash = a * (hash ^ xs[3]) + b;
    return hash;
}

uint32_t hash_str(const char *str) {
    uint32_t a = 1664525;
    uint32_t b = 1013904223;
    uint32_t hash = b;
    while (*str) {
        hash = a * (hash ^ (*str++)) + b;
    }
    return hash;
}

