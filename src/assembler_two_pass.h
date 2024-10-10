#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "assembler_data_processing.h"
#include "assembler_branching.h"
#include "assembler_loads_and_stores.h"

#define LINE_LENGTH 60

// Directive
#define INT_DIRECTIVE ".int"

void first_pass(FILE* file);

instruction handle_directive(char *line);

instruction handle_instruction(char *line, int address);

void second_pass(FILE* file, FILE* output);
