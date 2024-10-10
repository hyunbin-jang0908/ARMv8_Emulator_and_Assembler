#include "emulator_registers.h"

// General purpose registers
GeneralRegister generalRegisters[NUM_OF_REGISTERS];

// uint64_t ZR;              // Zero register

uint64_t PC;                 // Program counter

// uint64_t SP;              // Stack pointer

ProcessorStateRegister PSR;  // Processor state register

bool PC_branched = false;

// Initialise the registers
void initialise_registers(void) {
    for (register_encoding_t i = 0; i < NUM_OF_REGISTERS; i++) {
        generalRegisters[i].value = 0;
    }

    PSR.N = 0;
    PSR.Z = 1;
    PSR.C = 0;
    PSR.V = 0;

    PC = 0;
}

// Increment the PC if needed
void update_PC(void) {
    if (!PC_branched) {
        PC += 4;
    }
    PC_branched = false;
}

// Sets the PC to a new address: either by offset of immediate:
// if offset is not zero, then user the offset
// Otherwise, use the address given
void branch_PC(int32_t offset, address_t address) {
    PC_branched = true;
    if (offset) {
        PC += offset;
        return;
    }

    PC = address;
}

// Functions to interact with the general registers

// Read register value in 64-bit mode
uint64_t read64(register_encoding_t reg_no, char special_register_name[]) {
    if (reg_no == SR_HEX_VALUE) {
        return read64_SR(special_register_name);
    }
    return generalRegisters[reg_no].value;
}

// Read register value in 32-bit mode
uint32_t read32(register_encoding_t reg_no, char special_register_name[]) {
    if (reg_no == SR_HEX_VALUE) {
        return read32_SR(special_register_name);
    }
    return (uint32_t)generalRegisters[reg_no].value;
}

// Write register value in 64-bit mode
void write64(register_encoding_t reg_no, register_value64_t value) {
    if (reg_no == SR_HEX_VALUE) {
        return; // Zero Register Ignored
    }
    generalRegisters[reg_no].value = value;
}

// Write register value in 32-bit mode
void write32(register_encoding_t reg_no, register_value32_t value) {
    if (reg_no == SR_HEX_VALUE) {
        return; // Zero Register Ignored
    }
    generalRegisters[reg_no].value = value;
}

// Functions to interact with special purpose registers

// Read special purpose register value in 32-bit mode
int read32_SR(char special_register_name[]) {
    if (strcmp(special_register_name, "ZR") == 0) {
        return 0;
    }
    if (strcmp(special_register_name, "PC") == 0) {
        return PC;
    }
    if (strcmp(special_register_name, "SP") == 0) {
        return 0; // Not implemented
    }
    return 0; // Unknown register
}

// Read special purpose register value in 64-bit mode
uint64_t read64_SR(char special_register_name[]) {
    if (strcmp(special_register_name, "ZR") == 0) {
        return 0;
    }
    if (strcmp(special_register_name, "PC") == 0) {
        return PC;
    }
    if (strcmp(special_register_name, "SP") == 0) {
        return 0; // Not implemented
    }
    return 0; // Unknown register
}

// Output the final state of the registers
char* output_registers(void) {
    // Allocate memory for the output string
    char* output = malloc(CHARACTER_PER_LINE * (NUM_OF_REGISTERS + 2));
    char* output_ptr = output;

    // Output the register values
    for (register_encoding_t i = 0; i < NUM_OF_REGISTERS; i++) {
        output_ptr += snprintf(
            output_ptr,
            CHARACTER_PER_LINE,
            "x%02i    = %016" PRIx64 "\n",
            i,
            generalRegisters[i].value
        );
    }

    // Output the special purpose registers
    output_ptr += snprintf(
        output_ptr,
        CHARACTER_PER_LINE,
        "PC     = %016" PRIx64 "\n",
        PC
    );

    output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "PSTATE : ");
    if (PSR.N) {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "N");
    } else {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "-");
    }
    if (PSR.Z) {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "Z");
    } else {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "-");
    }
    if (PSR.C) {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "C");
    } else {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "-");
    }
    if (PSR.V) {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "V\n");
    } else {
        output_ptr += snprintf(output_ptr, CHARACTER_PER_LINE, "-\n");
    }

    return output;
}