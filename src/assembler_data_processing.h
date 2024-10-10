#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "assembler_registers.h"
#include "assembler_write.h"
#include "assembler_types.h"

// Data Processing Opcodes List
#define DATA_PROCESSING_OPCODES {"add", "adds", "sub", "subs", "cmp", "cmn", "neg", "negs", "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn", "tst", "movk", "movn", "movz", "mov", "mvn", "madd", "msub", "mul", "mneg"}

// Multiply
#define MADD "madd"

#define MSUB "msub"

#define CV_MUL_BITS 0xd8U // CV Stands for Constant Value

#define RD_MUL_LOW_BITS 0 // MUL Stands for Multiply
#define RD_MUL_HIGH_BITS 5

#define RN_MUL_LOW_BITS 5
#define RN_MUL_HIGH_BITS 10

#define RA_MUL_LOW_BITS 10
#define RA_MUL_HIGH_BITS 15

#define X_MUL_LOW_BITS 15
#define X_MUL_HIGH_BITS 16

#define RM_MUL_LOW_BITS 16
#define RM_MUL_HIGH_BITS 21

#define CONSTANT_MUL_LOW_BITS 21
#define CONSTANT_MUL_HIGH_BITS 31

#define SF_MUL_LOW_BITS 31
#define SF_MUL_HIGH_BITS 32

// Two Operands
#define RD_2OP_LOW_BITS 0 // 2OP Stands for Two Operands
#define RD_2OP_HIGH_BITS 5

#define RN_2OP_LOW_BITS 5
#define RN_2OP_HIGH_BITS 10

// Immediate

#define IMM12_2OP_LOW_BITS 10
#define IMM12_2OP_HIGH_BITS 22

#define SHIFT_AMOUNT_2OP 12

#define SHIFT_2OP_LOW_BITS 22
#define SHIFT_2OP_HIGH_BITS 23
#define SHIFTED_2OP_CV 1
#define NOT_SHIFTED_2OP_CV 0

#define ARITHMETIC_2OP_CV_1 0x2U
#define ARITHMETIC_2OP_CV_LOW_BITS_1 23
#define ARITHMETIC_2OP_CV_HIGH_BITS_1 26

#define ARITHMETIC_2OP_CV_2 0x4U
#define ARITHMETIC_2OP_CV_LOW_BITS_2 26
#define ARITHMETIC_2OP_CV_HIGH_BITS_2 29

// Register
#define AMOUNT_SHIFTED_2OP_LOW_BITS 10
#define AMOUNT_SHIFTED_2OP_HIGH_BITS 16

#define SHIFT_INSTRUCTION_2OP_LOW_BITS 22
#define SHIFT_INSTRUCTION_2OP_HIGH_BITS 24

#define LSL "lsl"
#define LSL_2OP_CV 0
#define LSR "lsr"
#define LSR_2OP_CV 1
#define ASR "asr"
#define ASR_2OP_CV 2

#define RM_2OP_LOW_BITS 16
#define RM_2OP_HIGH_BITS 21

#define ADD "add"

#define SUB "sub"

#define ARITHMETIC_REG_2OP_LOW_BITS 21
#define ARITHMETIC_REG_2OP_HIGH_BITS 22
#define ARITHMETIC_REG_2OP_CV 0

#define ARITHMETIC_REG_SHIFT_BIT_2OP_LOW_BITS 24
#define ARITHMETIC_REG_SHIFT_BIT_2OP_HIGH_BITS 25
#define ARITHMETIC_REG_SHIFT_BIT_2OP_CV 1

#define ROR "ror"
#define ROR_2OP_CV 3

#define LOGIC_2OP_CV_LOW_BITS_1 24
#define LOGIC_2OP_CV_HIGH_BITS_1 25
#define LOGIC_2OP_CV_1 0

#define LOGIC_2OP_CV_LOW_BITS_2 25
#define LOGIC_2OP_CV_HIGH_BITS_2 29
#define LOGIC_2OP_CV_2 5

#define ADDS "adds"

#define SUBS "subs"

#define AND "and"

#define BIC "bic"

#define ORR "orr"

#define ORN "orn"

#define EOR "eor"

#define EON "eon"

#define ANDS "ands"

#define BICS "bics"

#define OPCODE_2OP_LOW_BITS 29
#define OPCODE_2OP_HIGH_BITS 31

#define N_2OP_LOW_BITS 21
#define N_2OP_HIGH_BITS 22

#define N_2OP_FALSE_VALUE 0
#define N_2OP_TRUE_VALUE 1

#define ADD_2OP_VALUE 0

#define ADDS_2OP_VALUE 1

#define SUB_2OP_VALUE 2

#define SUBS_2OP_VALUE 3

#define AND_2OP_VALUE 0

#define BIC_2OP_VALUE 0

#define ORR_2OP_VALUE 1

#define ORN_2OP_VALUE 1

#define EOR_2OP_VALUE 2

#define EON_2OP_VALUE 2

#define ANDS_2OP_VALUE 3

#define BICS_2OP_VALUE 3

#define SF_2OP_LOW_BITS 31
#define SF_2OP_HIGH_BITS 32

// Single Operand with Destination
#define MOV "mov"

#define RD_1OP_LOW_BITS 0 // 1OP Stands for Single Operand with Destination
#define RD_1OP_HIGH_BITS 5

#define IMM16_1OP_LOW_BITS 5
#define IMM16_1OP_HIGH_BITS 21

#define SHIFT_INSTRUCTION_1OP_LOW_BITS 21
#define SHIFT_INSTRUCTION_1OP_HIGH_BITS 23

#define CV_1OP_LOW_BITS 23
#define CV_1OP_HIGH_BITS 29
#define CV_1OP 0x25U

#define MOVE_INSTRUCTION_1OP_LOW_BITS 29
#define MOVE_INSTRUCTION_1OP_HIGH_BITS 31

#define MOVN "movn"
#define MOVN_1OP_VALUE 0

#define MOVZ "movz"
#define MOVZ_1OP_VALUE 2

#define MOVK "movk"
#define MOVK_1OP_VALUE 3

#define SF_1OP_LOW_BITS 31
#define SF_1OP_HIGH_BITS 32

#define XZR "xzr"

#define WZR "wzr"

#define NEG "neg"

#define NEGS "negs"

#define MVN "mvn"

#define MUL "mul"

#define MNEG "mneg"

#define CMP "cmp"

#define CMN "cmn"

#define TST "tst"

instruction handle_multiply(char *opcode, char *line, int address);

instruction handle_two_operands(char *opcode, char *line, int address);

instruction handle_single_operands_with_destination(char *opcode, char *line, int address);

instruction handle_two_operands_with_no_destination(char *opcode, char *line, int address);

instruction handle_data_processing(char *opcode, char *line, int address);
