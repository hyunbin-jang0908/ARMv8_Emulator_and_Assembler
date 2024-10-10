#include <stdlib.h>
#include <string.h>

#include "assembler_types.h"

#define INITIAL_SYMBOL_TABLE_SIZE 32 * sizeof(symbol)

void initialise_symbol_table(void);

void free_symbol_table(void);

void add_item(char *label, int address);

int find_address(char *label);
