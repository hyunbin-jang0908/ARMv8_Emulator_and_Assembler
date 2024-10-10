#include "emulate.h"

int main(int argc, char **argv) {
    // Check if the number of arguments is correct
    if (argc > 3 || argc < 2) {
        printf("Invalid argument.");
        return EXIT_FAILURE;
    }

    // Check if input file exists
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        fclose(file);
        printf("File does not exist.");
        return EXIT_FAILURE;
    }

    initialise_registers();
    initialise_memory(file);

    fclose(file);
    
    // Start of the while loop and execute until an halt instruction is reached
    instruction_t next_instruction;
    while (1) {
        next_instruction = fetch();
        // Halt instruction that is not in A64, but for emulator to halt
        if (next_instruction == HALT_INSTRUCTION) {
            break;
        }
        process_instruction(next_instruction);
        update_PC();
    }

    // Print the final state of the registers and memory
    if (argc == 2) {
        // Print the final state of the registers and memory to the console
        printf("Registers:\n");

        char* register_output = output_registers();
        printf("%s", register_output);
        free(register_output);

        printf("Non-zero memory:\n");

        char* memory_output = output_memory();
        printf("%s", memory_output);
        free(memory_output);
    } else {
        // Write the final state of the registers and memory to a file
        FILE *output = fopen(argv[2], "w");

        fputs("Registers:\n", output);

        char* register_output = output_registers();
        fputs(register_output, output);
        free(register_output);

        fputs("Non-zero memory:\n", output);

        char* memory_output = output_memory();
        fputs(memory_output, output);
        free(memory_output);
        
        fclose(output);
    }

    return EXIT_SUCCESS;
}
