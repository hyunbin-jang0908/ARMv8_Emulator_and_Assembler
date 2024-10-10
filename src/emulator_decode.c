#include "emulator_decode.h"

// These functions obtains the values of the fields in the instruction

void decode_immediate(
    instruction_t instruction,
    int* sf,
    int* opc,
    int* opi,
    int* operand,
    register_encoding_t* rd
) {
    *sf = uextract_bits(
        instruction,
        SF_IMMEDIATE_OR_REGISTER_VALUE_1,
        SF_IMMEDIATE_OR_REGISTER_VALUE_2
    );
    *opc = uextract_bits(instruction, OPC_VALUE_1, OPC_VALUE_2);
    *opi = uextract_bits(instruction, OPI_VALUE_1, OPI_VALUE_2);
    *operand = uextract_bits(
        instruction,
        OPERAND_IMMEDIATE_VALUE_1,
        OPERAND_IMMEDIATE_VALUE_2
    );
    *rd = uextract_bits(instruction, RD_VALUE_1, RD_VALUE_2);
}

void decode_register(
    instruction_t instruction,
    int* sf,
    int* opc,
    int* M,
    int* opr,
    register_encoding_t* rm,
    int* operand,
    register_encoding_t* rn,
    register_encoding_t* rd
) {
    *sf = uextract_bits(
        instruction,
        SF_IMMEDIATE_OR_REGISTER_VALUE_1,
        SF_IMMEDIATE_OR_REGISTER_VALUE_2
    );
    *opc = uextract_bits(instruction, OPC_VALUE_1, OPC_VALUE_2);
    *M = uextract_bits(instruction, M_VALUE_1, M_VALUE_2);
    *opr = uextract_bits(instruction, OPR_VALUE_1, OPR_VALUE_2);
    *rm = uextract_bits(instruction, RM_VALUE_1, RM_VALUE_2);
    *operand = uextract_bits(
        instruction,
        OPERAND_REGISTER_VALUE_1,
        OPERAND_REGISTER_VALUE_2
    );
    *rn = uextract_bits(instruction, REG_RN_VALUE_1, REG_RN_VALUE_2);
    *rd = uextract_bits(instruction, RD_VALUE_1, RD_VALUE_2);
}

void decode_single_data_transfer(
    instruction_t instruction,
    int* sf,
    int* U,
    int* L,
    int* offset,
    register_encoding_t* xn,
    register_encoding_t* rt
) {
    *sf = uextract_bits(
        instruction,
        SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_1,
        SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_2
    );
    *U = uextract_bits(instruction, U_VALUE_1, U_VALUE_2);
    *L = uextract_bits(instruction, L_VALUE_1, L_VALUE_2);
    *offset = uextract_bits(instruction, OFFSET_VALUE_1, OFFSET_VALUE_2);
    *xn = uextract_bits(instruction, XN_VALUE_1, XN_VALUE_2);
    *rt = uextract_bits(instruction, RT_VALUE_1, RT_VALUE_2);
}

void decode_load_literal(
    instruction_t instruction,
    int* sf,
    int* simm19,
    register_encoding_t* rt
) {
    *sf = uextract_bits(
        instruction,
        SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_1,
        SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_2
    );
    *simm19 = extract_bits(instruction, SIMM19_VALUE_1, SIMM19_VALUE_2);
    *rt = uextract_bits(instruction, RT_VALUE_1, RT_VALUE_2);
}

void decode_branch(
    instruction_t instruction,
    int* operand
) {
    *operand = uextract_bits(
        instruction,
        OPERAND_BRANCH_VALUE_1,
        OPERAND_BRANCH_VALUE_2
    );
}

// Get bits from instruction starting from start to end (exclusive)
// with sign extension
int32_t extract_bits(int32_t bit_array, int start, int end) {
    return bit_array << (BITS_32 - end) >> (BITS_32 - end) >> start;
}

// Get bits from instruction starting from start to end (exclusive)
// without sign extension
uint32_t uextract_bits(uint32_t bit_array, int start, int end) {
    return bit_array << (BITS_32 - end) >> (BITS_32 - end) >> start;
}