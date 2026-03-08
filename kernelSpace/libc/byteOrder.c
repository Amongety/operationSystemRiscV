#include "../../include/kernelSpace/libc/byteOrder.h"

uint32_t bswap32(uint32_t x) {
    return ((x & 0x000000FFU) << 24) | ((x & 0x0000FF00U) << 8)  | ((x & 0x00FF0000U) >> 8)  | ((x & 0xFF000000U) >> 24);
}

uint64_t bswap64(uint64_t x) {
    return ((x & 0x00000000000000FFULL) << 56) | ((x & 0x000000000000FF00ULL) << 40) | ((x & 0x0000000000FF0000ULL) << 24) | ((x & 0x00000000FF000000ULL) << 8)  | ((x & 0x000000FF00000000ULL) >> 8)  | ((x & 0x0000FF0000000000ULL) >> 24) | ((x & 0x00FF000000000000ULL) >> 40) | ((x & 0xFF00000000000000ULL) >> 56);
}
