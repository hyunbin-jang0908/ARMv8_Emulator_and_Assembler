#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "assembler_types.h"

#define X 'x'

#define ZR "zr"

reg get_register(char *reg_str);