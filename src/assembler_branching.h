#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "assembler_registers.h"
#include "assembler_symbol_table.h"
#include "assembler_write.h"

// Branching B
#define B "b"

#define B_CHAR_AT_1 '\0'

#define SIMM26_B_LOW_BITS 0
#define SIMM26_B_HIGH_BITS 26

#define B_CV 0x5U // CV Stands for Constant Value
#define B_CV_LOW_BITS 26
#define B_CV_HIGH_BITS 32

// Branching BR
#define BR "br"

#define BR_CHAR_AT_1 'r'

#define XN_BR_LOW_BITS 5
#define XN_BR_HIGH_BITS 10

#define BR_CV 0x3587c0U
#define BR_CV_LOW_BITS 10
#define BR_CV_HIGH_BITS 32

// Branching B.COND
#define B_COND "b.cond"

#define B_COND_CHAR_AT_1 '.'

#define B_COND_LOW_BITS 0
#define B_COND_HIGH_BITS 4

#define SIMM19_B_COND_LOW_BITS 5
#define SIMM19_B_COND_HIGH_BITS 24

#define B_COND_CV_LOW_BITS 24
#define B_COND_CV_HIGH_BITS 32
#define B_COND_CV 0x54U

// b.eq
#define B_EQ "b.eq"
#define B_EQ_CV 0

// b.ne
#define B_NE "b.ne"
#define B_NE_CV 1

// b.ge
#define B_GE "b.ge"
#define B_GE_CV 10

// b.lt
#define B_LT "b.lt"
#define B_LT_CV 11

// b.gt
#define B_GT "b.gt"
#define B_GT_CV 12

// b.le
#define B_LE "b.le"
#define B_LE_CV 13

// b.al
#define B_AL "b.al"
#define B_AL_CV 14

instruction handle_branching(char *opcode, char *line, int address);
