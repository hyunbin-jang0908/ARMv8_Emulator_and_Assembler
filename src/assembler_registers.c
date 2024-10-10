#include "assembler_registers.h"

// Function to get the register number and bit width from the register string
reg get_register(char *reg_str) {
    reg reg;

    if (reg_str[0] == X) {
        reg.bit_width = 64;
    } else {
        reg.bit_width = 32;
    }
    if (strcmp(reg_str + 1, ZR) == 0) {
        reg.no = 31;
        return reg;
    }
    reg.no = strtol(reg_str + 1, NULL, 0);
    return reg;
}