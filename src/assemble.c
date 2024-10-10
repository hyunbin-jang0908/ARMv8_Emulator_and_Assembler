#include "assemble.h"

int main(int argc, char **argv) {
    // Check if the number of arguments is correct
    if (argc != 3) {
        printf("Invalid argument.");
        return EXIT_FAILURE;
    }

    // Obtain the input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fclose(file);
        printf("File does not exist.");
        return EXIT_FAILURE;
    }

    // Obtain the output file
    FILE* output = fopen(argv[2], "wb");
    if (output == NULL) {
        fclose(output);
        printf("File does not exist.");
        return EXIT_FAILURE;
    }

    // First pass : Create symbol table
    first_pass(file);

    // Second pass : reads each instruction and .int directive and generates the corresponding binary encoding
    second_pass(file, output);

    free_symbol_table();

    fclose(file);
    fclose(output);

    return EXIT_SUCCESS;
}
