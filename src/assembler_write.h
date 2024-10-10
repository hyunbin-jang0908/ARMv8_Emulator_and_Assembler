#include <stdint.h>

#define BITS_32 32

// Default write value
#define DEFAULT_WRITE_VALUE 0x00000000U

void write_bits(uint32_t *value, int start, int end, uint32_t bits);