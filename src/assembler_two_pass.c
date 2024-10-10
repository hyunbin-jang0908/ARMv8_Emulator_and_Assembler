#include "assembler_two_pass.h"

// First Pass, to populate the symbol table with the labels
void first_pass(FILE* file) {
    // Declare the symbol table
    initialise_symbol_table();
    char line[LINE_LENGTH];
    int lines = 0;

    // Reset file pointer
    rewind(file);
    // Fill up the symbol table
    while (fgets(line, sizeof(line), file) != NULL) {
        char *save_ptr;
        char* new_line = strtok_r(line, " ", &save_ptr);  // tokenise for space

        size_t len = strlen(new_line);

        // Remove new line character
        if (len > 0 && new_line[len - 1] == '\n') {
            new_line[len - 1] = '\0';
            len--;
        }

        // Populate the symbol table
        if (len > 0 && isalpha(new_line[0]) && new_line[len - 1] == ':') {
            new_line[len - 1] = '\0';  // Remove the colon

            // Check if the symbol table needs to be resized
            add_item(new_line, lines * 4);  // Add the symbol to the symbol table
        } else if (len > 0) {
            lines++;
        }
    }
}

// Handle the .int directive, other directives are not supported
instruction handle_directive(char *line) {
        char *saveptr;
        char *token = strtok_r(line, " ", &saveptr);
        if (strcmp(token, INT_DIRECTIVE) == 0) {
                token = strtok_r(NULL, " ", &saveptr);
                return strtol(token, NULL, 0);
        } else {
            exit(EXIT_FAILURE);  // Undefined Directive
        }
}

// General function to handle instructions
instruction handle_instruction(char *line, int address) {
    // Getting Opcode
    char *saveptr;
    char *opcode = strtok_r(line, " ", &saveptr); // the opcode
    char *rest_of_instruction = strtok_r(NULL, "", &saveptr); // rest of instruction after the opcode

    // Check if Opcode belongs to Data Processing
    char *data_processing_opcodes[] = DATA_PROCESSING_OPCODES;
    int data_processing_len =
        sizeof(data_processing_opcodes) / sizeof(data_processing_opcodes[0]);
    for (int index = 0; index < data_processing_len; index++) {
        if (strcmp(opcode, data_processing_opcodes[index]) == 0) {
            return handle_data_processing(opcode, rest_of_instruction, address);
        }
    }

    if (
        strcmp(opcode, B) == 0 || 
        strcmp(opcode, BR) == 0 || 
        strncmp(opcode, B_COND, 2) == 0
    ) {
        return handle_branching(opcode, rest_of_instruction, address);
    }

    // Check if Opcode belongs to loads and stores
    char *loads_and_stores_opcodes[] = LOADS_AND_STORES_OPCODES;
    int loads_and_stores_len =
        sizeof(loads_and_stores_opcodes) / sizeof(loads_and_stores_opcodes[0]);
    for (int index = 0; index < loads_and_stores_len; index++) {
        if (strcmp(opcode, loads_and_stores_opcodes[index]) == 0) {
            return handle_loads_and_stores(opcode, rest_of_instruction, address);
        }
    }

    // Special Instruction (Directive) already handled
    exit(EXIT_FAILURE);  // Unknown Instruction

}

// Second Pass, to write the instructions to the output file
void second_pass(FILE* file, FILE* output) {
    char line[LINE_LENGTH];
    int address = 0;

    // Reset file pointer
    rewind(file);
    rewind(output);
    // Read each line and process instructions
    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);

        // Ignore empty lines
        if (len == 1) {
            continue;
        }
        
        // Remove new line character
        while (line[len - 1] == '\n' || line[len - 1] == ' ') {
            line[len - 1] = '\0';
            len--;
        }

        // Skip Label
        if (line[len - 1] == ':') {
            continue;
        }

        int spaces = 0;
        while (isspace(*(line + spaces))) {    // use square brackets to dereference
            spaces++;
        }

        if (*(line + spaces) == '.') {
                // .int Directive
                instruction value = handle_directive(line + spaces);
                fwrite(&value, sizeof(instruction), 1, output);
        } else {
                // Instruction
                instruction value = handle_instruction(line + spaces, address);
                fwrite(&value, sizeof(instruction), 1, output);
        }

        address += 4;
    }
}
