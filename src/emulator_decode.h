#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "emulator_types.h"

// Instruction structure
#define BITS_32 32
#define BITS_64 64

// Immediate
#define SF_IMMEDIATE_OR_REGISTER_VALUE_1 31
#define SF_IMMEDIATE_OR_REGISTER_VALUE_2 32

#define OPC_VALUE_1 29
#define OPC_VALUE_2 31

#define OPI_VALUE_1 23
#define OPI_VALUE_2 26

#define OPERAND_IMMEDIATE_VALUE_1 5
#define OPERAND_IMMEDIATE_VALUE_2 23

#define RD_VALUE_1 0
#define RD_VALUE_2 5

// Register
#define M_VALUE_1 28
#define M_VALUE_2 29

#define OPR_VALUE_1 21
#define OPR_VALUE_2 25

#define RM_VALUE_1 16
#define RM_VALUE_2 21

#define OPERAND_REGISTER_VALUE_1 10
#define OPERAND_REGISTER_VALUE_2 16

#define REG_RN_VALUE_1 5
#define REG_RN_VALUE_2 10

// Single Data Transfer
#define SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_1 30
#define SF_SINGLE_DATA_TRANSFER_OR_LOAD_LITERAL_VALUE_2 31

#define U_VALUE_1 24
#define U_VALUE_2 25

#define L_VALUE_1 22
#define L_VALUE_2 23

#define OFFSET_VALUE_1 10
#define OFFSET_VALUE_2 22

#define XN_VALUE_1 5
#define XN_VALUE_2 10

#define RT_VALUE_1 0
#define RT_VALUE_2 5

// Load Literal
#define SIMM19_VALUE_1 5
#define SIMM19_VALUE_2 24

// Branch
#define OPERAND_BRANCH_VALUE_1 0
#define OPERAND_BRANCH_VALUE_2 26

void decode_immediate(instruction_t instruction,
    int* sf,
    int* opc,
    int* opi,
    int* operand,
    register_encoding_t* rd
);

void decode_register(instruction_t instruction,
    int* sf,
    int* opc,
    int* M,
    int* opr,
    register_encoding_t* rm,
    int* operand,
    register_encoding_t* rn,
    register_encoding_t* rd
);

void decode_single_data_transfer(instruction_t instruction,
    int* sf,
    int* U,
    int* L,
    int* offset,
    register_encoding_t* xn,
    register_encoding_t* rt
);

void decode_load_literal(instruction_t instruction,
    int* sf,
    int* simm19,
    register_encoding_t* rt
);

void decode_branch(
    instruction_t instruction,
    int* operand
);

int32_t extract_bits(int32_t bit_array, int start, int end);

uint32_t uextract_bits(uint32_t bit_array, int start, int end);
