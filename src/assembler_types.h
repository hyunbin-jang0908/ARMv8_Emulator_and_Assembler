#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

typedef struct {
    char* label;
    int address;
} symbol;

typedef struct {
    int bit_width;
    int no;
} reg;

typedef uint32_t instruction;

#endif // SYMBOLS_H