#include "emulator_memory.h"

// Memory array of size 2MB
uint8_t memory[MEMORY_SIZE];

void initialise_memory(FILE* file) {
    // Initialising Memory to set all registers and values to 0
    for (int i = 0; i < (MEMORY_SIZE); i++) {
        memory[i] = (uint8_t)0;
    }
    // Load the binary file into memory
    fread(memory, sizeof(uint8_t), MEMORY_SIZE, file);
    // Does not handle when file is smaller than memory size
}

// Load memory from address
uint32_t load_memory(address_t address) {
    return *(uint32_t*)(memory + address);
}

// Store to memory at address
void store_memory(address_t address, int32_t value) {
    *(uint32_t*)(memory + address) = value;
}

// Function to fetch the next instruction and increment the PC
int fetch(void) {
    instruction_t instruction = *(uint32_t*)(memory + PC);
    return instruction;
}

// Output non-zero memory locations
char* output_memory(void) {
    // Allocate memory for the output string
    char* output = malloc(25*(MEMORY_SIZE/MEM_ALIGN));
    char* output_ptr = output;
    int count = 0;

    // Output the non-zero memory locations
    for (address_t i = 0; i < MEMORY_SIZE; i += MEM_ALIGN) {
        if (load_memory(i)) {
            output_ptr += snprintf(
                output_ptr,
                (25*(MEMORY_SIZE/MEM_ALIGN - count)),
                "0x%08x: %08x\n",
                i,
                load_memory(i)
            );
            count++;
        }
    }
    return output;
}
