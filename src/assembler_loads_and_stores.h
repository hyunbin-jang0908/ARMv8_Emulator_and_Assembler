#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "assembler_registers.h"
#include "assembler_symbol_table.h"
#include "assembler_write.h"

// Loads and Stores Opcodes List
#define LOADS_AND_STORES_OPCODES {"str", "ldr"}

// Offset
#define INDEX_OFFSET_LOW_BITS 0
#define INDEX_OFFSET_HIGH_BITS 2

#define SIMM9_OFFSET_LOW_BITS 2
#define SIMM9_OFFSET_HIGH_BITS 11

#define PRE_INDEX_OFFSET_CV 0x3U // CV Stands for Constant Value

#define POST_INDEX_OFFSET_CV 0x1U

#define REG_OFFSET_CV_LOW_BITS_1 0
#define REG_OFFSET_CV_HIGH_BITS_1 6
#define REG_OFFSET_CV_1 0x1aU

#define REG_OFFSET_RM_LOW_BITS 6
#define REG_OFFSET_RM_HIGH_BITS 11

#define REG_OFFSET_CV_LOW_BITS_2 11
#define REG_OFFSET_CV_HIGH_BITS_2 12
#define REG_OFFSET_CV_2 1

// Load
#define LDR "ldr"

#define RT_LOAD_LOW_BITS 0
#define RT_LOAD_HIGH_BITS 5

#define SIMM19_LOAD_LOW_BITS 5
#define SIMM19_LOAD_HIGH_BITS 24

#define LOAD_CV_LOW_BITS 24
#define LOAD_CV_HIGH_BITS 32
#define LOAD_CV 0x18U

#define XN_LOAD_OFFSET_LOW_BITS 5
#define XN_LOAD_OFFSET_HIGH_BITS 10

#define OFFSET_LOAD_OFFSET_LOW_BITS 10
#define OFFSET_LOAD_OFFSET_HIGH_BITS 22

#define LOAD_OFFSET_CV_LOW_BITS_1 22
#define LOAD_OFFSET_CV_HIGH_BITS_1 23
#define LOAD_OFFSET_CV_1 1

#define LOAD_OFFSET_CV_LOW_BITS_2 23
#define LOAD_OFFSET_CV_HIGH_BITS_2 24
#define LOAD_OFFSET_CV_2 0

#define U_LOAD_OFFSET_LOW_BITS 24
#define U_LOAD_OFFSET_HIGH_BITS 25

// Store
#define RT_STORE_LOW_BITS 0
#define RT_STORE_HIGH_BITS 4

#define XN_STORE_LOW_BITS 5
#define XN_STORE_HIGH_BITS 10

#define OFFSET_STORE_LOW_BITS 10
#define OFFSET_STORE_HIGH_BITS 22

#define STORE_CV_LOW_BITS 22
#define STORE_CV_HIGH_BITS 24
#define STORE_CV 0

#define U_STORE_LOW_BITS 24
#define U_STORE_HIGH_BITS 25

// Load and Store
#define LAS_CV_LOW_BITS 25 // LAS Stands for Load And Store
#define LAS_CV_HIGH_BITS 32
#define LAS_CV 0x5cU

#define SF_LAS_LOW_BITS 30
#define SF_LAS_HIGH_BITS 31

instruction handle_loads_and_stores(char *opcode, char *line, int address);