#include "assembler_data_processing.h"

// Multiply
instruction handle_multiply(char *opcode, char *line, int address) {
    // Multiply sf = depends on register, M = 1, opc = 00, opr = 1000, rm = rm register binary, operand = x + ra (x is 0 for madd x is 1 for msub), rn = rn register binary, rd = rd register binary

    char *saveptr;

    char *rd_str = strtok_r(line, ", ", &saveptr);
    reg rd = get_register(rd_str);

    char *rn_str = strtok_r(NULL, ", ", &saveptr);
    reg rn = get_register(rn_str);

    char *rm_str = strtok_r(NULL, ", ", &saveptr);
    reg rm = get_register(rm_str);

    char *ra_str = strtok_r(NULL, ", ", &saveptr);
    reg ra = get_register(ra_str);

    int x;
    if (strcmp(opcode, MADD) == 0) {
        x = 0;
    } else {
        x = 1;
    }
    
    // assembling the instruction
    instruction value_to_write = DEFAULT_WRITE_VALUE;
    instruction *ptr_value = &value_to_write;
    write_bits(ptr_value, RD_MUL_LOW_BITS, RD_MUL_HIGH_BITS, rd.no);
    write_bits(ptr_value, RN_MUL_LOW_BITS, RN_MUL_HIGH_BITS, rn.no);
    write_bits(ptr_value, RA_MUL_LOW_BITS, RA_MUL_HIGH_BITS, ra.no);
    write_bits(ptr_value, X_MUL_LOW_BITS, X_MUL_HIGH_BITS, x);
    write_bits(ptr_value, RM_MUL_LOW_BITS, RM_MUL_HIGH_BITS, rm.no);
    write_bits(
        ptr_value, 
        CONSTANT_MUL_LOW_BITS, 
        CONSTANT_MUL_HIGH_BITS, 
        CV_MUL_BITS
    );
    write_bits(
        ptr_value, 
        SF_MUL_LOW_BITS, 
        SF_MUL_HIGH_BITS, 
        rd.bit_width == 64
    );
    
    return value_to_write;
}

// Two Operand with Destination Structure
// Applies to: add(s), sub(s), and(s), bic(s), orr, orn, eor, eon
instruction handle_two_operands(char *opcode, char *line, int address) {
    char *saveptr;

    char *rd_str = strtok_r(line, ", ", &saveptr);
    reg rd = get_register(rd_str);

    char *rn_str = strtok_r(NULL, ", ", &saveptr);
    reg rn = get_register(rn_str);

    char *operand_str = strtok_r(NULL, ", ", &saveptr);

    instruction value_to_write = DEFAULT_WRITE_VALUE;
    instruction *ptr_value = &value_to_write;
    write_bits(ptr_value, RD_2OP_LOW_BITS, RD_2OP_HIGH_BITS, rd.no);
    write_bits(ptr_value, RN_2OP_LOW_BITS, RN_2OP_HIGH_BITS, rn.no);

    if (operand_str[0] == '#') {
        // Immediate
        char *imm_str = operand_str;
        unsigned int imm12 = strtol(imm_str + 1, NULL, 0);

        write_bits(ptr_value, IMM12_2OP_LOW_BITS, IMM12_2OP_HIGH_BITS, imm12);

        char *shift_str = strtok_r(NULL, " #", &saveptr);
        if (shift_str != NULL) {
            // Shifted
            char *amount_str = strtok_r(NULL, " #", &saveptr);  // the remaining string after ' #' in operand_str
            int amount = strtol(amount_str, NULL, 0);

            if (strcmp(shift_str, LSL) == 0 && amount == SHIFT_AMOUNT_2OP) {
                write_bits(
                    ptr_value, 
                    SHIFT_2OP_LOW_BITS, 
                    SHIFT_2OP_HIGH_BITS, 
                    SHIFTED_2OP_CV
                );
            } else {
                // Unsupported
            }
        } else {
            // Not shifted
            write_bits(
                ptr_value, 
                SHIFT_2OP_LOW_BITS, 
                SHIFT_2OP_HIGH_BITS, 
                NOT_SHIFTED_2OP_CV
            );
        }

        write_bits(
            ptr_value, 
            ARITHMETIC_2OP_CV_LOW_BITS_1, 
            ARITHMETIC_2OP_CV_HIGH_BITS_1, 
            ARITHMETIC_2OP_CV_1
        );    // Arithmetic
        write_bits(
            ptr_value, 
            ARITHMETIC_2OP_CV_LOW_BITS_2, 
            ARITHMETIC_2OP_CV_HIGH_BITS_2, 
            ARITHMETIC_2OP_CV_2
        );
    } else {
        // Register

        char *rm_str = operand_str;
        reg rm = get_register(rm_str);

        char *shift_str = strtok_r(NULL, " #", &saveptr);
        if (shift_str != NULL) {
            // Shifted
            char *amount_str = strtok_r(NULL, " #", &saveptr);  // remaining part after ' #' in operand_str
            int amount = strtol(amount_str, NULL, 0);
            
            write_bits(
                ptr_value, 
                AMOUNT_SHIFTED_2OP_LOW_BITS, 
                AMOUNT_SHIFTED_2OP_HIGH_BITS, 
                amount
            );

            if (strcmp(shift_str, LSL) == 0) {
                write_bits(
                    ptr_value, 
                    SHIFT_INSTRUCTION_2OP_LOW_BITS, 
                    SHIFT_INSTRUCTION_2OP_HIGH_BITS, 
                    LSL_2OP_CV
                );
            } else if (strcmp(shift_str, LSR) == 0) {
                write_bits(
                    ptr_value, 
                    SHIFT_INSTRUCTION_2OP_LOW_BITS, 
                    SHIFT_INSTRUCTION_2OP_HIGH_BITS, 
                    LSR_2OP_CV
                );
            } else if (strcmp(shift_str, ASR) == 0) {
                write_bits(
                    ptr_value, 
                    SHIFT_INSTRUCTION_2OP_LOW_BITS, 
                    SHIFT_INSTRUCTION_2OP_HIGH_BITS, 
                    ASR_2OP_CV
                );
            }
        }

        // rm
        write_bits(ptr_value, RM_2OP_LOW_BITS, RM_2OP_HIGH_BITS, rm.no);

        if (strncmp(opcode, ADD, 3) == 0 || strncmp(opcode, SUB, 3) == 0) {
            // Arithmetic
            write_bits(
                ptr_value, 
                ARITHMETIC_REG_2OP_LOW_BITS, 
                ARITHMETIC_REG_2OP_HIGH_BITS, 
                ARITHMETIC_REG_2OP_CV
            );
            // Shift bit written above
            write_bits(
                ptr_value, 
                ARITHMETIC_REG_SHIFT_BIT_2OP_LOW_BITS, 
                ARITHMETIC_REG_SHIFT_BIT_2OP_HIGH_BITS, 
                ARITHMETIC_REG_SHIFT_BIT_2OP_CV
            );

        } else {
            // Logical
            // N bit will be written below
            // Shift bit written above except ror
            if (shift_str != NULL && strcmp(shift_str, ROR) == 0) {
                write_bits(
                    ptr_value, 
                    SHIFT_INSTRUCTION_2OP_LOW_BITS, 
                    SHIFT_INSTRUCTION_2OP_HIGH_BITS, 
                    ROR_2OP_CV
                );
            }
            write_bits(
                ptr_value, 
                LOGIC_2OP_CV_LOW_BITS_1, 
                LOGIC_2OP_CV_HIGH_BITS_1, 
                LOGIC_2OP_CV_1
            );
        }
        write_bits(
            ptr_value, 
            LOGIC_2OP_CV_LOW_BITS_2, 
            LOGIC_2OP_CV_HIGH_BITS_2, 
            LOGIC_2OP_CV_2
        );
    }

    // opc
    if (strcmp(opcode, ADD) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            ADD_2OP_VALUE
        );
    } else if (strcmp(opcode, ADDS) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            ADDS_2OP_VALUE
        );
    } else if (strcmp(opcode, SUB) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            SUB_2OP_VALUE
        );
    } else if (strcmp(opcode, SUBS) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            SUBS_2OP_VALUE
        );
    } else if (strcmp(opcode, AND) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            AND_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_FALSE_VALUE
        );
    } else if (strcmp(opcode, BIC) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            BIC_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_TRUE_VALUE
        );
    } else if (strcmp(opcode, ORR) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            ORR_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_FALSE_VALUE
        );
    } else if (strcmp(opcode, ORN) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            ORN_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_TRUE_VALUE
        );
    } else if (strcmp(opcode, EOR) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            EOR_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_FALSE_VALUE
        );
    } else if (strcmp(opcode, EON) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            EON_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_TRUE_VALUE
        );
    } else if (strcmp(opcode, ANDS) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            ANDS_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_FALSE_VALUE
        );
    } else if (strcmp(opcode, BICS) == 0) {
        write_bits(
            ptr_value, 
            OPCODE_2OP_LOW_BITS, 
            OPCODE_2OP_HIGH_BITS, 
            BICS_2OP_VALUE
        );
        write_bits(
            ptr_value, 
            N_2OP_LOW_BITS, 
            N_2OP_HIGH_BITS, 
            N_2OP_TRUE_VALUE
        );
    } else {
        exit(EXIT_FAILURE);  // Unknown opcode
    }

    write_bits(
        ptr_value, 
        SF_2OP_LOW_BITS, 
        SF_2OP_HIGH_BITS, 
        rd.bit_width == 64
    );

    return value_to_write;
}

// Single Operand with Destination Structure
// Applies to: mov, mul, mneg, neg(s), mvn, and wide move: movz, movn, movk
instruction handle_single_operand_with_destination(
    char *opcode, 
    char *line, 
    int address
) {
    char *saveptr;

    char *rd_str = strtok_r(line, ", ", &saveptr);
    reg rd = get_register(rd_str);


    if (strncmp(opcode, MOV, 3) == 0 && strlen(opcode) > 3) {
        // Wide Move
        instruction value_to_write = DEFAULT_WRITE_VALUE;
        instruction *ptr_value = &value_to_write;
        write_bits(ptr_value, RD_1OP_LOW_BITS, RD_1OP_HIGH_BITS, rd.no);

        char *operand_str = strtok_r(NULL, ", ", &saveptr);
        char *imm_str = operand_str;
        unsigned int imm16 = strtol(imm_str + 1, NULL, 0);

        write_bits(ptr_value, IMM16_1OP_LOW_BITS, IMM16_1OP_HIGH_BITS, imm16);

        char *test_str = strtok_r(NULL, ", ", &saveptr);
        if (test_str != NULL) {
            // Shifted
            char *shift_str = test_str;
            char *amount_str = strtok_r(NULL, " #", &saveptr);
            int amount = strtol(amount_str, NULL, 0);

            if (strcmp(shift_str, LSL) == 0 && (amount % 16 == 0)) {
                write_bits(
                    ptr_value, 
                    SHIFT_INSTRUCTION_1OP_LOW_BITS, 
                    SHIFT_INSTRUCTION_1OP_HIGH_BITS, 
                    amount / 16
                );
            } else {
                // Unsupported
            }
        }
        write_bits(ptr_value, CV_1OP_LOW_BITS, CV_1OP_HIGH_BITS, CV_1OP);
        if (strcmp(opcode, MOVN) == 0) {
            write_bits(
                ptr_value, 
                MOVE_INSTRUCTION_1OP_LOW_BITS, 
                MOVE_INSTRUCTION_1OP_HIGH_BITS, 
                MOVN_1OP_VALUE
            );
        } else if (strcmp(opcode, MOVZ) == 0) {
            write_bits(
                ptr_value, 
                MOVE_INSTRUCTION_1OP_LOW_BITS, 
                MOVE_INSTRUCTION_1OP_HIGH_BITS, 
                MOVZ_1OP_VALUE
            );
        } else if (strcmp(opcode, MOVK) == 0) {
            write_bits(
                ptr_value, 
                MOVE_INSTRUCTION_1OP_LOW_BITS, 
                MOVE_INSTRUCTION_1OP_HIGH_BITS, 
                MOVK_1OP_VALUE
            );
        }

        write_bits(
            ptr_value, 
            SF_1OP_LOW_BITS, 
            SF_1OP_HIGH_BITS, 
            rd.bit_width == 64
        );

        return value_to_write;

    } else {
        // Aliases
        char *operand_str = strtok_r(NULL, "", &saveptr);
        char* new_line = malloc((strlen(line) + 20) * sizeof(char));
        char* zr_str;
        if (rd.bit_width == 64) {
            zr_str = XZR;
        } else {
            zr_str = WZR;
        }

        instruction value;
        if (strcmp(opcode, NEG) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, zr_str, operand_str);
            value = handle_two_operands(SUB, new_line, address);
        } else if (strcmp(opcode, NEGS) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, zr_str, operand_str);
            value = handle_two_operands(SUBS, new_line, address);
        } else if (strcmp(opcode, MVN) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, zr_str, operand_str);
            value = handle_two_operands(ORN, new_line, address);
        } else if (strcmp(opcode, MOV) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, zr_str, operand_str);
            value = handle_two_operands(ORR, new_line, address);
        } else if (strcmp(opcode, MUL) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, operand_str, zr_str);
            value = handle_multiply(MADD, new_line, address);
        } else if (strcmp(opcode, MNEG) == 0) {
            sprintf(new_line, "%s, %s, %s", rd_str, operand_str, zr_str);
            value = handle_multiply(MSUB, new_line, address);
        }

        free(new_line);
        return value;
    }
}

// Two Operands, no Desitnation Structure
// Applies to: cmp, cmn, tst
instruction handle_two_operands_with_no_destination(
    char *opcode, 
    char *line, 
    int address
) {
    char *saveptr;

    char *new_line = malloc((strlen(line) + 10) * sizeof(char));

    char *rn_str = strtok_r(line, ", ", &saveptr);
    reg rn = get_register(rn_str);

    char *operand_str = strtok_r(NULL, "", &saveptr);

    char* zr_str;
    if (rn.bit_width == 64) {
        zr_str = XZR;
    } else {
        zr_str = WZR;
    }

    instruction value;
    if (strcmp(opcode, CMP) == 0) {
        sprintf(new_line, "%s, %s, %s", zr_str, rn_str, operand_str);
        value = handle_two_operands(SUBS, new_line, address);
    } else if (strcmp(opcode, CMN) == 0) {
        sprintf(new_line, "%s, %s, %s", zr_str, rn_str, operand_str);
        value = handle_two_operands(ADDS, new_line, address);
    } else if (strcmp(opcode, TST) == 0) {
        sprintf(new_line, "%s, %s, %s", zr_str, rn_str, operand_str);
        value = handle_two_operands(ANDS, new_line, address);
    }

    free(new_line);

    return value;
}

// Handle Data Processing of all types, then pass to corresponding function
instruction handle_data_processing(char *opcode, char *line, int address) {

    // Multiply
    if (strcmp(opcode, MADD) == 0 || strcmp(opcode, MSUB) == 0) {
        return handle_multiply(opcode, line, address);
    }

    // Two Operand with Destination
    if (strncmp(opcode, ADD, 3) == 0 || strncmp(opcode, SUB, 3) == 0) {
        return handle_two_operands(opcode, line, address);
    }
    if (strncmp(opcode, AND, 3) == 0 || strncmp(opcode, BIC, 3) == 0) {
        return handle_two_operands(opcode, line, address);
    }
    if (
        strcmp(opcode, EOR) == 0 || 
        strcmp(opcode, EON) == 0 || 
        strcmp(opcode, ORR) == 0 || 
        strcmp(opcode, ORN) == 0
    ) {
        return handle_two_operands(opcode, line, address);
    }

    // Single Operand with Destination

    if (strcmp(opcode, MOV) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MUL) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MNEG) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strncmp(opcode, NEG, 3) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MVN) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MOVZ) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MOVN) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }
    if (strcmp(opcode, MOVK) == 0) {
        return handle_single_operand_with_destination(opcode, line, address);
    }


    // Two Operands with no destination
    if (
        strcmp(opcode, CMP) == 0 || 
        strcmp(opcode, CMN) == 0 || 
        strcmp(opcode, TST) == 0
    ) {
        return handle_two_operands_with_no_destination(opcode, line, address);
    }
    exit(EXIT_FAILURE);  // Unknown instruction
}
