#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulator_registers.h"
#include "emulator_types.h"

#define MEMORY_SIZE 2097152  // 2^21
#define MEM_ALIGN 4

extern uint64_t PC;

void initialise_memory(FILE* file);

uint32_t load_memory(address_t address);

void store_memory(address_t address, int32_t value);

int fetch(void);

char* output_memory(void);
