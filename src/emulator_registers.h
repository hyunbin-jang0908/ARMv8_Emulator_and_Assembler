#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator_types.h"

#define NUM_OF_REGISTERS 31
#define CHARACTER_PER_LINE 27

#define SR_HEX_VALUE 0x1fU // Special Register Hex Value

// Creating a structure to represent the general registers
typedef struct GeneralRegister {
  uint64_t value;
} GeneralRegister;

typedef struct ProcessorStateRegister {
  bool N;  // Negative flag
  bool Z;  // Zero flag
  bool C;  // Carry flag
  bool V;  // Overflow flag
} ProcessorStateRegister;

extern uint64_t PC;

void initialise_registers(void);

void update_PC(void);

void branch_PC(int32_t offset, address_t address);

uint64_t read64(register_encoding_t reg_no, char special_register_name[]);

uint32_t read32(register_encoding_t reg_no, char special_register_name[]);

void write64(register_encoding_t reg_no, register_value64_t value);

void write32(register_encoding_t reg_no, register_value32_t value);

int read32_SR(char special_register_name[]);

uint64_t read64_SR(char special_register_name[]);

char* output_registers(void);
