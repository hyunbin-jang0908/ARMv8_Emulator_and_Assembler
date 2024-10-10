#include "emulator_process_instruction.h"

// Calls the appropriate function to execute the instruction by categorizing it
// and calls the respective decoding function to obtain the values of the fields

void process_instruction(instruction_t instruction) {
    if (uextract_bits(
            instruction,
            OP0_IMMEDIATE_VALUE_1,
            OP0_IMMEDIATE_VALUE_2
        ) == OP0_IMMEDIATE_BIT_VALUE) {

        int *sf = malloc(sizeof(int)), *opc = malloc(sizeof(int));
        int *opi = malloc(sizeof(int)), *operand = malloc(sizeof(int));
        register_encoding_t *rd = malloc(sizeof(register_encoding_t));
        decode_immediate(instruction, sf, opc, opi, operand, rd);

        if (*opi == OPI_010) {
            if (*sf == SF_64) {
                execute_immediate_arithmetic_64(*sf, *opc, *opi, *operand, *rd);
            } else {
                execute_immediate_arithmetic_32(*sf, *opc, *opi, *operand, *rd);
            }
        } else if (*opi == OPI_101) {
            if (*sf == SF_64) {
                execute_immediate_wide_move_64(*sf, *opc, *opi, *operand, *rd);
            } else {
                execute_immediate_wide_move_32(*sf, *opc, *opi, *operand, *rd);
            }
        }

        free(sf); free(opc); free(opi); free(operand); free(rd);
    } else if (uextract_bits(
            instruction,
            OP0_REGISTER_VALUE_1,
            OP0_REGISTER_VALUE_2
        ) == OP0_REGISTER_BIT_VALUE) {

        int *sf = malloc(sizeof(int)), *opc = malloc(sizeof(int));
        int *M = malloc(sizeof(int)), *opr = malloc(sizeof(int));
        int *operand = malloc(sizeof(int));
        register_encoding_t *rm = malloc(sizeof(register_encoding_t));
        register_encoding_t *rn = malloc(sizeof(register_encoding_t));
        register_encoding_t *rd = malloc(sizeof(register_encoding_t));
        decode_register(instruction, sf, opc, M, opr, rm, operand, rn, rd);

        if (
            *M == M_FALSE_VALUE &&
            (*opr & OPR_REGISTER_ARITHMETIC_MASK) ==
                OPR_REGISTER_ARITHMETIC_VALUE
        ) {
            // Register Arithmetic
            if (*sf == SF_64) {
                execute_register_arithmetic_64(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            } else {
                execute_register_arithmetic_32(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            }
        } else if (*M == M_FALSE_VALUE && ~*opr & OPR_REGISTER_BIT_VALUE) {
            // Register Bit-Logic
            if (*sf == SF_64) {
                execute_register_logical_64(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            } else {
                execute_register_logical_32(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            }
        } else if (*M == M_TRUE_VALUE && *opr == OPR_REGISTER_MULTIPLY_VALUE) {
            // Register Multiply
            if (*sf == SF_64) {
                execute_register_multiply_64(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            } else {
                execute_register_multiply_32(
                    *sf,
                    *opc,
                    *M,
                    *opr,
                    *rm,
                    *operand,
                    *rn,
                    *rd
                );
            }
        }

        free(sf); free(opc); free(M); free(opr);
        free(rm); free(operand); free(rn); free(rd);
    } else if (
        (instruction & OP0_LOAD_OR_STORE_MASK) == OP0_LOAD_OR_STORE_BIT_VALUE
    ) {
        // Assuming load literal determined by bit 31
        if (instruction & OP0_SINGLE_DATA_TRANSFER_VALUE) {

            int *sf = malloc(sizeof(int)), *U = malloc(sizeof(int));
            int *L = malloc(sizeof(int)), *offset = malloc(sizeof(int));
            register_encoding_t *xn = malloc(sizeof(register_encoding_t));
            register_encoding_t *rt = malloc(sizeof(register_encoding_t));
            decode_single_data_transfer(instruction, sf, U, L, offset, xn, rt);
            
            if (*L == L_DECIMAL_VALUE) {
                if (*sf == SF_64) {
                    execute_load_64(*sf, *U, *L, *offset, *xn, *rt);
                } else {
                    execute_load_32(*sf, *U, *L, *offset, *xn, *rt);
                }
            } else {
                if (*sf == SF_64) {
                    execute_store_64(*sf, *U, *L, *offset, *xn, *rt);
                } else {
                    execute_store_32(*sf, *U, *L, *offset, *xn, *rt);
                }
            }

            free(sf); free(U); free(L); free(offset); free(xn); free(rt);
        } else {
            int *sf = malloc(sizeof(int)), *simm19 = malloc(sizeof(int));
            register_encoding_t *rt = malloc(sizeof(register_encoding_t));
            decode_load_literal(instruction, sf, simm19, rt);

            if (*sf == SF_64) {
                execute_load_literal_64(*sf, *simm19, *rt);
            } else {
                execute_load_literal_32(*sf, *simm19, *rt);
            }

            free(sf); free(simm19); free(rt);
        }
    } else if ((instruction & OP0_BRANCH_MASK) == OP0_BRANCH_BIT_VALUE) {

        int *operand = malloc(sizeof(int));
        decode_branch(instruction, operand);

        int opc = uextract_bits(
            instruction,
            OPC_BRANCH_VALUE_1,
            OPC_BRANCH_VALUE_2
        );
        if (opc == OPC_BRANCH_UNCONDITIONAL_DECIMAL_VALUE) {
            execute_branch_unconditional(*operand);
        } else if (opc == OPC_BRANCH_REGISTER_DECIMAL_VALUE) {
            execute_branch_register(*operand);
        } else if (opc == OPC_BRANCH_CONDITIONAL_DECIMAL_VALUE) {
            execute_branch_conditional(*operand);
        }

        free(operand);
    } else {
        return; // Unknown instruction
    }
}