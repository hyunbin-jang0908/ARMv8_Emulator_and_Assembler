#include "assembler_write.h"

// An helper function to write value bit by bit
void write_bits(uint32_t *value, int start, int end, uint32_t bits) {
    // Shift bits to starting position
    bits <<= start;
    
    // Clear bits that is out of the range
    bits = bits << (BITS_32 - end) >> (BITS_32 - end);

    // Clear bits in the range
    uint32_t mask = 0;
    for (int i = 0; i < start; i++) {
        mask |= 1 << i;
    }
    for (int i = end; i < BITS_32; i++) {
        mask |= 1 << i;
    }
    *value &= mask;

    // Set bits in the range and return
    *value = *value | bits;
}