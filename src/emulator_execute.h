#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "emulator_instructions.h"
#include "emulator_decode.h"

// Immediate

// Immediate Arithmetic
#define SH_VALUE_1 17
#define SH_VALUE_2 18

#define SH_NEED_TO_SHIFT 1
#define SH_SHIFT_BITS_12 12

#define IMM12_VALUE_1 5
#define IMM12_VALUE_2 17

#define IMM_RN_VALUE_1 0
#define IMM_RN_VALUE_2 5

#define ADD_DECIMAL_VALUE 0
#define ADDS_DECIMAL_VALUE 1
#define SUB_DECIMAL_VALUE 2
#define SUBS_DECIMAL_VALUE 3

// Immediate Wide Move
#define HW_VALUE_1 16
#define HW_VALUE_2 18

#define IMM16_VALUE_1 0
#define IMM16_VALUE_2 16

#define MOVN_DECIMAL_VALUE 0
#define MOVZ_DECIMAL_VALUE 2
#define MOVK_DECIMAL_VALUE 3

// Register Arithmetic & Logical
#define SHIFT_REGISTER_VALUE_1 1
#define SHIFT_REGISTER_VALUE_2 3

#define SHIFT_REGISTER_LSL 0
#define SHIFT_REGISTER_LSR 1
#define SHIFT_REGISTER_ASR 2
#define SHIFT_REGISTER_ROR 3

#define N_REGISTER_VALUE_1 0
#define N_REGISTER_VALUE_2 1
#define N_REGISTER_TRUE_VALUE 1
#define N_REGISTER_FALSE_VALUE 0

#define AND_DECIMAL_VALUE 0
#define BIC_DECIMAL_VALUE 0
#define ORR_DECIMAL_VALUE 1
#define ORN_DECIMAL_VALUE 1
#define EOR_DECIMAL_VALUE 2
#define EON_DECIMAL_VALUE 2
#define ANDS_DECIMAL_VALUE 3
#define BICS_DECIMAL_VALUE 3

// Register Multiply
#define X_SHIFT 5

#define RA_VALUE_1 0
#define RA_VALUE_2 5

// Single Data Transfer
#define ADDRESSING_MODE_BIT 0x800U

#define XM_VALUE_1 6
#define XM_VALUE_2 11

#define I_VALUE_1 1
#define I_VALUE_2 2

#define SIMM9_VALUE_1 2
#define SIMM9_VALUE_2 11

// Branch Instruction
#define SIMM26_VALUE_1 0
#define SIMM26_VALUE_2 26

#define XN_BRANCH_VALUE_1 5
#define XN_BRANCH_VALUE_2 10

#define SIMM19_BRANCH_VALUE_1 5
#define SIMM19_BRANCH_VALUE_2 24

#define COND_BRANCH_VALUE_1 0
#define COND_BRANCH_VALUE_2 4

// Get extern to registers
extern uint64_t PC;
extern struct ProcessorStateRegister PSR;

// Immediate execution

void execute_immediate_arithmetic_32(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
);

void execute_immediate_arithmetic_64(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
);

void execute_immediate_wide_move_32(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
);

void execute_immediate_wide_move_64(
    int sf,
    int opc,
    int opi,
    int64_t operand,
    register_encoding_t rd
);

// Register execution

void execute_register_arithmetic_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

void execute_register_arithmetic_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

void execute_register_logical_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

void execute_register_logical_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

void execute_register_multiply_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

void execute_register_multiply_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
);

// Single Data Transfer Instruction

void execute_load_literal_32(int sf, int simm19, register_encoding_t rt);

void execute_load_literal_64(int sf, int64_t simm19, register_encoding_t rt);

void execute_load_32(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t rn,
    register_encoding_t rt
);

void execute_load_64(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t rn,
    register_encoding_t rt
);

void execute_store_32(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t rn,
    register_encoding_t rt
);

void execute_store_64(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t rn,
    register_encoding_t rt
);

// Branch Instruction

void execute_branch_unconditional(int operand);

void execute_branch_register(int operand);

void execute_branch_conditional(int operand);