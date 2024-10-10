#include "assembler_branching.h"

// Handling branching instructions differently, according their type
instruction handle_branching(char *opcode, char *line, int address) {
    char *saveptr;

    instruction value_to_write = DEFAULT_WRITE_VALUE;
    instruction *ptr_value = &value_to_write;
    switch(opcode[1]) {
        case B_CHAR_AT_1:
            // Branch to literal
            {
                char *literal_str = strtok_r(line, " ", &saveptr);
                int32_t simm26 = (find_address(literal_str) - address) / 4;

                write_bits(
                    ptr_value, 
                    SIMM26_B_LOW_BITS, 
                    SIMM26_B_HIGH_BITS, 
                    simm26
                );
                write_bits(
                    ptr_value, 
                    B_CV_LOW_BITS, 
                    B_CV_HIGH_BITS, 
                    B_CV
                );
            }
            return value_to_write;

        case BR_CHAR_AT_1:
            // Branch to register
            {
                char *xn = strtok_r(line, " ", &saveptr);
                reg xn_reg = get_register(xn);

                write_bits(ptr_value, XN_BR_LOW_BITS, XN_BR_HIGH_BITS, xn_reg.no);
                write_bits(ptr_value, BR_CV_LOW_BITS, BR_CV_HIGH_BITS, BR_CV);
            }
            return value_to_write;
        
        case B_COND_CHAR_AT_1:
            // Conditional branch
            {
                char *literal_str_cond = strtok_r(line, " ", &saveptr);

                if (strcmp(opcode, B_EQ) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_EQ_CV
                    );
                } else if (strcmp(opcode, B_NE) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_NE_CV
                    );
                } else if (strcmp(opcode, B_GE) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_GE_CV
                    );
                } else if (strcmp(opcode, B_LT) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_LT_CV
                    );
                } else if (strcmp(opcode, B_GT) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_GT_CV
                    );
                } else if (strcmp(opcode, B_LE) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_LE_CV
                    );
                } else if (strcmp(opcode, B_AL) == 0) {
                    write_bits(
                        ptr_value, 
                        B_COND_LOW_BITS, 
                        B_COND_HIGH_BITS, 
                        B_AL_CV
                    );
                } else {
                    exit(EXIT_FAILURE);  // Undefined conditional branch
                }

                int32_t simm19 = (find_address(literal_str_cond) - address) / 4;

                write_bits(
                    ptr_value, 
                    SIMM19_B_COND_LOW_BITS, 
                    SIMM19_B_COND_HIGH_BITS, 
                    simm19
                );
                write_bits(
                    ptr_value, 
                    B_COND_CV_LOW_BITS, 
                    B_COND_CV_HIGH_BITS, 
                    B_COND_CV
                );
            }
            return value_to_write;

        default:
            exit(EXIT_FAILURE);  // Unknown instruction
    }
}