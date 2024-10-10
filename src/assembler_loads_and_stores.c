#include "assembler_loads_and_stores.h"

// Generic function to get the offset to be written
static unsigned int get_offset(char *address_str, reg *xn, bool *U) {
    char *saveptr;
    char *fst_str = strtok_r(address_str, ", ", &saveptr);
    char *snd_str = strtok_r(NULL, ", ", &saveptr);
    if (snd_str == NULL) {
        // Unsigned Offset
        *U = true;

        fst_str[strlen(fst_str) - 1] = '\0';
        *xn = get_register(fst_str);

        return 0;  // which is the offset
    }
    if (snd_str[0] == '#') {
        if (snd_str[strlen(snd_str) - 1] == ']') {
            // Unsigned Immediate Offset
            *U = true;

            *xn = get_register(fst_str);

            snd_str[strlen(snd_str) - 1] = '\0';
            int imm12 = strtol(snd_str + 1, NULL, 0) / (xn->bit_width / 8);

            return imm12;  // which is the whole offset
        }
        if (snd_str[strlen(snd_str) - 1] == '!') {
            // Pre-indexed Offset
            *U = false;

            *xn = get_register(fst_str);

            snd_str[strlen(snd_str) - 2] = '\0';
            int simm9 = strtol(snd_str + 1, NULL, 0);

            unsigned int offset = 0;
            write_bits(
                &offset, 
                INDEX_OFFSET_LOW_BITS, 
                INDEX_OFFSET_HIGH_BITS, 
                PRE_INDEX_OFFSET_CV
            );
            write_bits(
                &offset, 
                SIMM9_OFFSET_LOW_BITS, 
                SIMM9_OFFSET_HIGH_BITS, 
                simm9
            );
            return offset;
        }
        // Post-indexed Offset
        *U = false;

        fst_str[strlen(fst_str) - 1] = '\0';
        *xn = get_register(fst_str);

        int simm9 = strtol(snd_str + 1, NULL, 0);

        unsigned int offset = 0;
        write_bits(
            &offset, 
            INDEX_OFFSET_LOW_BITS, 
            INDEX_OFFSET_LOW_BITS, 
            POST_INDEX_OFFSET_CV
        );
        write_bits(
            &offset, 
            SIMM9_OFFSET_LOW_BITS, 
            SIMM9_OFFSET_HIGH_BITS, 
            simm9
        );
        return offset;
    }
    // Register Offset
    *U = false;

    *xn = get_register(fst_str);

    snd_str[strlen(snd_str) - 1] = '\0';
    reg rm = get_register(snd_str);

    unsigned int offset = 0;
    write_bits(
        &offset, 
        REG_OFFSET_CV_LOW_BITS_1, 
        REG_OFFSET_CV_HIGH_BITS_1, 
        REG_OFFSET_CV_1
    );
    write_bits(
        &offset, 
        REG_OFFSET_RM_LOW_BITS, 
        REG_OFFSET_RM_HIGH_BITS, 
        rm.no
    );
    write_bits(
        &offset, 
        REG_OFFSET_CV_LOW_BITS_2, 
        REG_OFFSET_CV_HIGH_BITS_2, 
        REG_OFFSET_CV_2
    );
    return offset;
}

// Function to handle loads and stores
instruction handle_loads_and_stores(char *opcode, char *line, int address) {
    char *saveptr;

    // Load
    if (strcmp(opcode, LDR) == 0) {
        char *rt_str = strtok_r(line, ", ", &saveptr);
        reg rt = get_register(rt_str);

        // Test for load literal
        int index = 0;
        if (saveptr[0] == ' ') {
            index = 1;
        }

        int32_t simm19;
        if (saveptr[index] == '#') {
            // Load literal using immediate value
            simm19 =
                (strtol(saveptr + 1, NULL, 0) - address) / (rt.bit_width / 8);

            instruction value_to_write = DEFAULT_WRITE_VALUE;
            instruction *ptr_value = &value_to_write;
            write_bits(ptr_value, RT_LOAD_LOW_BITS, RT_LOAD_HIGH_BITS, rt.no);
            write_bits(
                ptr_value, 
                SIMM19_LOAD_LOW_BITS, 
                SIMM19_LOAD_HIGH_BITS, 
                simm19
            );
            write_bits(ptr_value, LOAD_CV_LOW_BITS, LOAD_CV_HIGH_BITS, LOAD_CV);
            write_bits(
                ptr_value, 
                SF_LAS_LOW_BITS, 
                SF_LAS_HIGH_BITS, 
                rt.bit_width == 64
            );
            return value_to_write;
        }
        // Try load literal using label
        int target = find_address(saveptr + index);
        if (target >= 0) {
            simm19 = (target - address) / 4;

            instruction value_to_write = DEFAULT_WRITE_VALUE;
            instruction *ptr_value = &value_to_write;
            write_bits(ptr_value, RT_LOAD_LOW_BITS, RT_LOAD_HIGH_BITS, rt.no);
            write_bits(
                ptr_value, 
                SIMM19_LOAD_LOW_BITS, 
                SIMM19_LOAD_HIGH_BITS, 
                simm19
            );
            write_bits(ptr_value, LOAD_CV_LOW_BITS, LOAD_CV_HIGH_BITS, LOAD_CV);
            write_bits(
                ptr_value, 
                SF_LAS_LOW_BITS, 
                SF_LAS_HIGH_BITS, 
                rt.bit_width == 64
            );
            return value_to_write;
        }
        // Call get_offset to get the offset
        reg *xn = malloc(sizeof(reg));
        bool *U = malloc(sizeof(bool));
        saveptr += 1;
        char *address_str = strtok_r(NULL, "[", &saveptr);
        unsigned int offset = get_offset(address_str, xn, U);

        instruction value_to_write = DEFAULT_WRITE_VALUE;
        instruction *ptr_value = &value_to_write;
        write_bits(ptr_value, RT_LOAD_LOW_BITS, RT_LOAD_HIGH_BITS, rt.no);
        write_bits(
            ptr_value, 
            XN_LOAD_OFFSET_LOW_BITS, 
            XN_LOAD_OFFSET_HIGH_BITS, 
            xn->no
        );
        write_bits(
            ptr_value, 
            OFFSET_LOAD_OFFSET_LOW_BITS, 
            OFFSET_LOAD_OFFSET_HIGH_BITS, 
            offset
        );
        write_bits(
            ptr_value, 
            LOAD_OFFSET_CV_LOW_BITS_1, 
            LOAD_OFFSET_CV_HIGH_BITS_1, 
            LOAD_OFFSET_CV_1
        );
        write_bits(
            ptr_value, 
            LOAD_OFFSET_CV_LOW_BITS_2, 
            LOAD_OFFSET_CV_HIGH_BITS_2, 
            LOAD_OFFSET_CV_2
        );
        write_bits(
            ptr_value, 
            U_LOAD_OFFSET_LOW_BITS, 
            U_LOAD_OFFSET_HIGH_BITS, 
            *U
        );
        write_bits(ptr_value, LAS_CV_LOW_BITS, LAS_CV_HIGH_BITS, LAS_CV);
        write_bits(
            ptr_value, 
            SF_LAS_LOW_BITS, 
            SF_LAS_HIGH_BITS, 
            rt.bit_width == 64
        );

        free(xn); free(U);

        return value_to_write;
    }
    // Store
    char *rt_str = strtok_r(line, ", ", &saveptr);
    reg rt = get_register(rt_str);

    // Call get_offset to get the offset
    reg *xn = malloc(sizeof(reg));
    bool *U = malloc(sizeof(bool));
    saveptr += 1;
    char *address_str = strtok_r(NULL, "[", &saveptr);
    unsigned int offset = get_offset(address_str, xn, U);

    instruction value_to_write = DEFAULT_WRITE_VALUE;
    instruction *ptr_value = &value_to_write;
    write_bits(ptr_value, RT_STORE_LOW_BITS, RT_LOAD_HIGH_BITS, rt.no);
    write_bits(ptr_value, XN_STORE_LOW_BITS, XN_STORE_HIGH_BITS, xn->no);
    write_bits(
        ptr_value, 
        OFFSET_STORE_LOW_BITS, 
        OFFSET_STORE_HIGH_BITS, 
        offset
    );
    write_bits(ptr_value, STORE_CV_LOW_BITS, STORE_CV_HIGH_BITS, STORE_CV);  // Store
    write_bits(ptr_value, U_STORE_LOW_BITS, U_STORE_HIGH_BITS, *U);
    write_bits(ptr_value, LAS_CV_LOW_BITS, LAS_CV_HIGH_BITS, LAS_CV);
    write_bits(
        ptr_value, 
        SF_LAS_LOW_BITS, 
        SF_LAS_HIGH_BITS, 
        rt.bit_width == 64
    );

    free(xn); free(U);
    return value_to_write;
}
