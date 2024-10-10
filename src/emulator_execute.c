#include "emulator_execute.h"

// These functions execute the instructions accordingly using
// the values and call respective instruction functions

void execute_immediate_arithmetic_32(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
) {
    int sh = uextract_bits(operand, SH_VALUE_1, SH_VALUE_2);
    int imm12 = uextract_bits(operand, IMM12_VALUE_1, IMM12_VALUE_2);
    register_encoding_t rn = uextract_bits(operand, IMM_RN_VALUE_1, IMM_RN_VALUE_2);

    register_value32_t rn_value;
    rn_value = read32(rn, "SP");
    
    if (sh == SH_NEED_TO_SHIFT) {
        imm12 = lsl(imm12, SH_SHIFT_BITS_12, true);
    }

    int result;
    switch (opc) {
        case ADD_DECIMAL_VALUE:
            result = add_32(rn_value, imm12);
            break;
        case ADDS_DECIMAL_VALUE:
            result = adds_32(rn_value, imm12);
            break;
        case SUB_DECIMAL_VALUE:
            result = sub_32(rn_value, imm12);
            break;
        case SUBS_DECIMAL_VALUE:
            result = subs_32(rn_value, imm12);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write32(rd, result);
}

void execute_immediate_arithmetic_64(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
) {
    int sh = uextract_bits(operand, SH_VALUE_1, SH_VALUE_2);
    int imm12 = uextract_bits(operand, IMM12_VALUE_1, IMM12_VALUE_2);
    register_encoding_t rn = uextract_bits(operand, IMM_RN_VALUE_1, IMM_RN_VALUE_2);

    register_value64_t rn_value;
    rn_value = read64(rn, "SP");

    if (sh == SH_NEED_TO_SHIFT) {
        imm12 = lsl(imm12, SH_SHIFT_BITS_12, false);
    }

    int64_t result;
    switch (opc) {
        case ADD_DECIMAL_VALUE:
            result = add_64(rn_value, imm12);
            break;
        case ADDS_DECIMAL_VALUE:
            result = adds_64(rn_value, imm12);
            break;
        case SUB_DECIMAL_VALUE:
            result = sub_64(rn_value, imm12);
            break;
        case SUBS_DECIMAL_VALUE:
            result = subs_64(rn_value, imm12);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write64(rd, result);
}

void execute_immediate_wide_move_32(
    int sf,
    int opc,
    int opi,
    int operand,
    register_encoding_t rd
) {
    int hw = uextract_bits(operand, HW_VALUE_1, HW_VALUE_2);
    unsigned int imm16 = uextract_bits(operand, IMM16_VALUE_1, IMM16_VALUE_2);
    operand = lsl(imm16, (hw * OPERAND_SHIFT_BITS_16), true);
    
    register_value32_t rd_value;
    rd_value = read32(rd, "SP");

    int result;
    switch (opc) {
        case MOVN_DECIMAL_VALUE:
            result = movn_32(operand);
            break;
        case MOVZ_DECIMAL_VALUE:
            result = movz_32(operand);
            break;
        case MOVK_DECIMAL_VALUE:
            result = movk_32(rd_value, imm16, hw);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write32(rd, result);
}

void execute_immediate_wide_move_64(
    int sf,
    int opc,
    int opi,
    int64_t operand,
    register_encoding_t rd
) {
    int hw = uextract_bits(operand, HW_VALUE_1, HW_VALUE_2);
    unsigned int imm16 = uextract_bits(operand, IMM16_VALUE_1, IMM16_VALUE_2);
    operand = lsl(imm16, (hw * OPERAND_SHIFT_BITS_16), false);

    register_value64_t rd_value;
    rd_value = read64(rd, "SP");

    int64_t result;
    switch (opc) {
        case MOVN_DECIMAL_VALUE:
            result = movn_64(operand);
            break;
        case MOVZ_DECIMAL_VALUE:
            result = movz_64(operand);
            break;
        case MOVK_DECIMAL_VALUE:
            result = movk_64(rd_value, imm16, hw);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write64(rd, result);
}

void execute_register_arithmetic_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int shift = uextract_bits(
        opr,
        SHIFT_REGISTER_VALUE_1,
        SHIFT_REGISTER_VALUE_2
    );

    register_value32_t rn_value;
    rn_value = read32(rn, "SP");

    register_value32_t rm_value;
    rm_value = read32(rm, "ZR");

    switch (shift) {
        case SHIFT_REGISTER_LSL:
            rm_value = lsl(rm_value, operand, true);
            break;
        case SHIFT_REGISTER_LSR:
            rm_value = lsr(rm_value, operand, true);
            break;
        case SHIFT_REGISTER_ASR:
            rm_value = asr(rm_value, operand, true);
            break;
    }

    int result;
    switch (opc) {
        case ADD_DECIMAL_VALUE:
            result = add_32(rn_value, rm_value);
            break;
        case ADDS_DECIMAL_VALUE:
            result = adds_32(rn_value, rm_value);
            break;
        case SUB_DECIMAL_VALUE:
            result = sub_32(rn_value, rm_value);
            break;
        case SUBS_DECIMAL_VALUE:
            result = subs_32(rn_value, rm_value);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }
    write32(rd, result);
}

void execute_register_arithmetic_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int shift = uextract_bits(opr, SHIFT_REGISTER_VALUE_1, SHIFT_REGISTER_VALUE_2);

    register_value64_t rn_value;
    rn_value = read64(rn, "SP");

    register_value64_t rm_value;
    rm_value = read64(rm, "ZR");

    switch (shift) {
        case SHIFT_REGISTER_LSL:
            rm_value = lsl(rm_value, operand, false);
            break;
        case SHIFT_REGISTER_LSR:
            rm_value = lsr(rm_value, operand, false);
            break;
        case SHIFT_REGISTER_ASR:
            rm_value = asr(rm_value, operand, false);
            break;
    }

    int64_t result;
    switch (opc) {
        case ADD_DECIMAL_VALUE:
            result = add_64(rn_value, rm_value);
            break;
        case ADDS_DECIMAL_VALUE:
            result = adds_64(rn_value, rm_value);
            break;
        case SUB_DECIMAL_VALUE:
            result = sub_64(rn_value, rm_value);
            break;
        case SUBS_DECIMAL_VALUE:
            result = subs_64(rn_value, rm_value);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write64(rd, result);
}

void execute_register_logical_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int shift = uextract_bits(opr, SHIFT_REGISTER_VALUE_1, SHIFT_REGISTER_VALUE_2);
    int N = uextract_bits(opr, N_REGISTER_VALUE_1, N_REGISTER_VALUE_2);

    register_value32_t rn_value;
    rn_value = read32(rn, "SP");

    register_value32_t rm_value;
    rm_value = read32(rm, "ZR");

    switch (shift) {
        case SHIFT_REGISTER_LSL:
            rm_value = lsl(rm_value, operand, true);
            break;
        case SHIFT_REGISTER_LSR:
            rm_value = lsr(rm_value, operand, true);
            break;
        case SHIFT_REGISTER_ASR:
            rm_value = asr(rm_value, operand, true);
            break;
        case SHIFT_REGISTER_ROR:
            rm_value = ror(rm_value, operand, true);
            break;
    }

    int result;
    switch ((opc << 1) + N) {
        case (AND_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = and_32(rn_value, rm_value);
            break;
        case (BIC_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = bic_32(rn_value, rm_value);
            break;
        case (ORR_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = orr_32(rn_value, rm_value);
            break;
        case (ORN_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = orn_32(rn_value, rm_value);
            break;
        case (EOR_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = eor_32(rn_value, rm_value);
            break;
        case (EON_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = eon_32(rn_value, rm_value);
            break;
        case (ANDS_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = ands_32(rn_value, rm_value);
            break;
        case (BICS_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = bics_32(rn_value, rm_value);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write32(rd, result);
}

void execute_register_logical_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int shift = uextract_bits(opr, SHIFT_REGISTER_VALUE_1, SHIFT_REGISTER_VALUE_2);
    int N = uextract_bits(opr, N_REGISTER_VALUE_1, N_REGISTER_VALUE_2);

    register_value64_t rn_value;
    rn_value = read64(rn, "SP");

    register_value64_t rm_value;
    rm_value = read64(rm, "ZR");

    switch (shift) {
        case SHIFT_REGISTER_LSL:
            rm_value = lsl(rm_value, operand, false);
            break;
        case SHIFT_REGISTER_LSR:
            rm_value = lsr(rm_value, operand, false);
            break;
        case SHIFT_REGISTER_ASR:
            rm_value = asr(rm_value, operand, false);
            break;
        case SHIFT_REGISTER_ROR:
            rm_value = ror(rm_value, operand, false);
            break;
    }

    int64_t result;
    switch ((opc << 1) + N) {
        case (AND_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = and_64(rn_value, rm_value);
            break;
        case (BIC_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = bic_64(rn_value, rm_value);
            break;
        case (ORR_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = orr_64(rn_value, rm_value);
            break;
        case (ORN_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = orn_64(rn_value, rm_value);
            break;
        case (EOR_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = eor_64(rn_value, rm_value);
            break;
        case (EON_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = eon_64(rn_value, rm_value);
            break;
        case (ANDS_DECIMAL_VALUE << 1) + N_REGISTER_FALSE_VALUE:
            result = ands_64(rn_value, rm_value);
            break;
        case (BICS_DECIMAL_VALUE << 1) + N_REGISTER_TRUE_VALUE:
            result = bics_64(rn_value, rm_value);
            break;
        default:
            exit(EXIT_FAILURE);  // Impossible to reach
    }

    write64(rd, result);
}

void execute_register_multiply_32(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int x = operand >> X_SHIFT;
    int ra = uextract_bits(operand, RA_VALUE_1, RA_VALUE_2);
    
    register_value32_t ra_value;
    ra_value = read32(ra, "SP");
    
    register_value32_t rn_value;
    rn_value = read32(rn, "SP");
    
    register_value32_t rm_value;
    rm_value = read32(rm, "ZR");

    if (x) {
        write32(rd, msub_32(ra_value, rn_value, rm_value));
    } else {
        write32(rd, madd_32(ra_value, rn_value, rm_value));
    }
}

void execute_register_multiply_64(
    int sf,
    int opc,
    int M,
    int opr,
    register_encoding_t rm,
    int operand,
    register_encoding_t rn,
    register_encoding_t rd
) {
    int x = operand >> X_SHIFT;
    int ra = uextract_bits(operand, RA_VALUE_1, RA_VALUE_2);
    
    register_value64_t ra_value;
    ra_value = read64(ra, "SP");
    
    register_value64_t rn_value;
    rn_value = read64(rn, "SP");
    
    register_value64_t rm_value;
    rm_value = read64(rm, "ZR");

    if (x) {
        write64(rd, msub_64(ra_value, rn_value, rm_value));
    } else {
        write64(rd, madd_64(ra_value, rn_value, rm_value));
    }
}

void execute_load_literal_32(int sf, int simm19, register_encoding_t rt) {
    address_t address = PC + simm19 * 4;
    write32(rt, load_memory(address));
}

void execute_load_literal_64(int sf, int64_t simm19, register_encoding_t rt) {
    address_t address = PC + simm19 * 4;
    uint64_t value = load_memory(address);
    value += (uint64_t)load_memory(address + 4) << BITS_32;
    write64(rt, value);
}

void execute_load_32(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t xn,
    register_encoding_t rt
) {
    // Calculate target address
    address_t address;
    if (U) {
        // Unsigned Immediate Offset
        int uoffset = offset * 4;
        address = read64(xn, "SP") + uoffset;
    } else if (offset & ADDRESSING_MODE_BIT){
        // Register Offset
        register_encoding_t xm = uextract_bits(offset, XM_VALUE_1, XM_VALUE_2);
        address = read64(xn, "SP") + read64(xm, NULL);
    } else {
        int I = uextract_bits(offset, I_VALUE_1, I_VALUE_2);
        int simm9 = extract_bits(offset, SIMM9_VALUE_1, SIMM9_VALUE_2);
        if (I) {
            // Pre-Indexed
            address = read64(xn, "SP") + simm9;
            write64(xn, address);
        } else {
            // Post-Indexed
            address = read64(xn, "SP");
            write64(xn, address + simm9);
        }
    }
    write32(rt, load_memory(address));
};

void execute_load_64(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t xn,
    register_encoding_t rt
) {
    // Calculate target address
    address_t address;
    if (U) {
        // Unsigned Immediate Offset
        int uoffset = offset * 8;
        address = read64(xn, "SP") + uoffset;
    } else if (offset & ADDRESSING_MODE_BIT){
        // Register Offset
        register_encoding_t xm = uextract_bits(offset, XM_VALUE_1, XM_VALUE_2);
        address = read64(xn, "SP") + read64(xm, NULL);
    } else {
        int I = uextract_bits(offset, I_VALUE_1, I_VALUE_2);
        int simm9 = extract_bits(offset, SIMM9_VALUE_1, SIMM9_VALUE_2);
        if (I) {
            // Pre-Indexed
            address = read64(xn, "SP") + simm9;
            write64(xn, address);
        } else {
            // Post-Indexed
            address = read64(xn, "SP");
            write64(xn, address + simm9);
        }
    }
    uint64_t value = load_memory(address);
    value += (uint64_t)load_memory(address + 4) << BITS_32;
    write64(rt, value);
};

void execute_store_32(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t xn,
    register_encoding_t rt
) {
    // Calculate target address
    address_t address;
    if (U) {
        // Unsigned Immediate Offset
        int uoffset = offset * 4;
        address = read64(xn, "SP") + uoffset;
    } else if (offset & ADDRESSING_MODE_BIT){
        // Register Offset
        register_encoding_t xm = uextract_bits(offset, XM_VALUE_1, XM_VALUE_2);
        address = read64(xn, "SP") + read64(xm, NULL);
    } else {
        int I = uextract_bits(offset, I_VALUE_1, I_VALUE_2);
        int simm9 = extract_bits(offset, SIMM9_VALUE_1, SIMM9_VALUE_2);
        if (I) {
            // Pre-Indexed
            address = read64(xn, "SP") + simm9;
            write32(xn, address);
        } else {
            // Post-Indexed
            address = read64(xn, "SP");
            write32(xn, address + simm9);
        }
    }
    
    store_memory(address, read64(rt, NULL));
};

void execute_store_64(
    int sf,
    int U,
    int L,
    int offset,
    register_encoding_t xn,
    register_encoding_t rt
) {
    // Calculate target address
    address_t address;
    if (U) {
        // Unsigned Immediate Offset
        int uoffset = offset * 8;
        address = read64(xn, "SP") + uoffset;
    } else if (offset & ADDRESSING_MODE_BIT){
        // Register Offset
        register_encoding_t xm = uextract_bits(offset, XM_VALUE_1, XM_VALUE_2);
        address = read64(xn, "SP") + read64(xm, NULL);
    } else {
        int I = uextract_bits(offset, I_VALUE_1, I_VALUE_2);
        int simm9 = extract_bits(offset, SIMM9_VALUE_1, SIMM9_VALUE_2);
        if (I) {
            // Pre-Indexed
            address = read64(xn, "SP") + simm9;
            write32(xn, address);
        } else {
            // Post-Indexed
            address = read64(xn, "SP");
            write32(xn, address + simm9);
        }
    }

    store_memory(address, read64(rt, NULL));
    store_memory(address + 4, read64(rt, NULL) >> BITS_32);
};

void execute_branch_unconditional(int operand) {
    int32_t simm26 = extract_bits(operand, SIMM26_VALUE_1, SIMM26_VALUE_2);

    b(simm26 * 4);
}

void execute_branch_register(int operand) {
    register_encoding_t xn = uextract_bits(operand, XN_BRANCH_VALUE_1, XN_BRANCH_VALUE_2);

    if (xn == SR_HEX_VALUE) {
        // Zero Register Ignored
    } else {
        br(read64(xn, NULL));
    }
}

void execute_branch_conditional(int operand) {
    int32_t simm19 = extract_bits(operand, SIMM19_BRANCH_VALUE_1, SIMM19_BRANCH_VALUE_2);
    int cond = uextract_bits(operand, COND_BRANCH_VALUE_1, COND_BRANCH_VALUE_2);

    b_cond(cond, simm19 * 4);
}
