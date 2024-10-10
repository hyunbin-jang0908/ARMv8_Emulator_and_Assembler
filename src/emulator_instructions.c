#include "emulator_instructions.h"

uint64_t lsl(uint64_t operand, int shift_amount, bool is_32_bit) {
    if (is_32_bit) {
        return (uint32_t)(operand << shift_amount);
    }
    return operand << shift_amount;
}

uint64_t lsr(uint64_t operand, int shift_amount, bool is_32_bit) {
    if (is_32_bit) {
        return (uint32_t)(operand >> shift_amount);
    }
    return operand >> shift_amount;
}

uint64_t asr(uint64_t operand, int shift_amount, bool is_32_bit) {
    if (is_32_bit) {
        int32_t signed_operand = (int32_t)operand;
        return (uint32_t)(signed_operand >> shift_amount);
    }
    int64_t signed_operand = (int64_t)operand;
    return signed_operand >> shift_amount;
}

uint64_t ror(uint64_t operand, int shift_amount, bool is_32_bit) {
    if (is_32_bit) {
        uint32_t value = operand;
        shift_amount %= BITS_32;
        return (value >> shift_amount) | (value << (BITS_32 - shift_amount));
    }
    shift_amount %= BITS_64;
    return (operand >> shift_amount) | (operand << (BITS_64 - shift_amount));
}

int add_32(register_value32_t rn_value, int op2) {
    return rn_value + op2;
}

int64_t add_64(register_value64_t rn_value, int64_t op2) {
    return rn_value + op2;
}

int adds_32(register_value32_t rn_value, int op2) {
    int result = rn_value + op2;

    PSR.N = (result < 0);
    PSR.Z = (result == 0);
    PSR.C = ((uint32_t)result < rn_value);
    PSR.V = ((int32_t)rn_value > 0 && op2 > 0 && result < 0);
    PSR.V = PSR.V || ((int32_t)rn_value < 0 && op2 < 0 && result > 0);

    return result;
}

int64_t adds_64(register_value64_t rn_value, int64_t op2) {
    int64_t result = rn_value + op2;

    PSR.N = (result < 0);
    PSR.Z = (result == 0);
    PSR.C = ((uint64_t)result < rn_value);
    PSR.V = ((int64_t)rn_value > 0 && op2 > 0 && result < 0);
    PSR.V = PSR.V || ((int64_t)rn_value < 0 && op2 < 0 && result > 0);

    return result;
}

int sub_32(register_value32_t rn_value, int op2) {
    return rn_value - op2;
}

int64_t sub_64(register_value64_t rn_value, int64_t op2) {
    return rn_value - op2;
}

int subs_32(register_value32_t rn_value, int op2) {
    int result = rn_value - op2;

    PSR.N = (result < 0);
    PSR.Z = (result == 0);
    PSR.C = !((uint32_t)result > rn_value);
    PSR.V = ((int32_t)rn_value < 0 && op2 > 0 && result > 0);
    PSR.V = PSR.V || ((int32_t)rn_value > 0 && op2 < 0 && result < 0);

    return result;
}

int64_t subs_64(register_value64_t rn_value, int64_t op2) {
    int64_t result = rn_value - op2;

    PSR.N = (result < 0);
    PSR.Z = (result == 0);
    PSR.C = !((uint64_t)result > rn_value);
    PSR.V = ((int64_t)rn_value < 0 && op2 > 0 && result > 0);
    PSR.V = PSR.V || ((int64_t)rn_value > 0 && op2 < 0 && result < 0);

    return result;
}

int movn_32(int op) {
    return ~op;
}

int64_t movn_64(int64_t op) {
    return ~op;
}

int movz_32(int op) {
    return op;
}

int64_t movz_64(int64_t op) {
    return op;
}

int movk_32(register_value32_t rd, unsigned int imm16, int hw) {
    int value = rd;
    
    int shift_amount = hw * OPERAND_SHIFT_BITS_16;

    uint32_t mask = ~(CLEAR_16_BITS << shift_amount);

    value = (value & mask) | ((uint32_t)imm16 << shift_amount);

    value &= CLEAR_TOP_32_BITS;

    return value;
}

int64_t movk_64(register_value64_t rd, unsigned int imm16, int hw) {
    int64_t value = rd;
    
    int shift_amount = hw * OPERAND_SHIFT_BITS_16;

    uint64_t mask = ~((uint64_t)CLEAR_16_BITS << shift_amount);

    value = (value & mask) | ((uint64_t)imm16 << shift_amount);

    return value;
}

int and_32(register_value32_t rn_value, int op2) {
    return rn_value & op2;
}

int64_t and_64(register_value64_t rn_value, int64_t op2) {
    return rn_value & op2;
}

int bic_32(register_value32_t rn_value, int op2) {
    return rn_value & ~op2;
}

int64_t bic_64(register_value64_t rn_value, int64_t op2) {
    return rn_value & ~op2;
}

int orr_32(register_value32_t rn_value, int op2) {
    return rn_value | op2;
}

int64_t orr_64(register_value64_t rn_value, int64_t op2) {
    return rn_value | op2;
}

int orn_32(register_value32_t rn_value, int op2) {
    return rn_value | ~op2;
}

int64_t orn_64(register_value64_t rn_value, int64_t op2) {
    return rn_value | ~op2;
}

int eor_32(register_value32_t rn_value, int op2) {
    return rn_value ^ op2;
}

int64_t eor_64(register_value64_t rn_value, int64_t op2) {
    return rn_value ^ op2;
}

int eon_32(register_value32_t rn_value, int op2) {
    return rn_value ^ ~op2;
}

int64_t eon_64(register_value64_t rn_value, int64_t op2) {
    return rn_value ^ ~op2;
}

int ands_32(register_value32_t rn_value, int op2) {
    int result = rn_value & op2;

    PSR.N = (result & (1 << (BITS_32 - 1))) != 0;
    PSR.Z = (result == 0);
    PSR.C = 0;
    PSR.V = 0;

    return result;
}

int64_t ands_64(register_value64_t rn_value, int64_t op2) {
    int64_t result = rn_value & op2;

    PSR.N = (result & ((int64_t)1 << (BITS_64 - 1))) != 0;
    PSR.Z = (result == 0);
    PSR.C = 0;
    PSR.V = 0;

    return result;
}

int bics_32(register_value32_t rn_value, int op2) {
    int result = rn_value & ~op2;

    PSR.N = (result & (1 << (BITS_32 - 1))) != 0;
    PSR.Z = (result == 0);
    PSR.C = 0;
    PSR.V = 0;

    return result;
}

int64_t bics_64(register_value64_t rn_value, int64_t op2) {
    int64_t result = rn_value & ~op2;

    PSR.N = (result & ((int64_t)1 << (BITS_64 - 1))) != 0;
    PSR.Z = (result == 0);
    PSR.C = 0;
    PSR.V = 0;

    return result;
}

int msub_32(
    register_value32_t ra_value,
    register_value32_t rn_value,
    register_value32_t rm_value
) {
    return ra_value - rn_value * rm_value;
}

int64_t msub_64(
    register_value64_t ra_value,
    register_value64_t rn_value,
    register_value64_t rm_value
) {
    return ra_value - rn_value * rm_value;
}

int madd_32(
    register_value32_t ra_value,
    register_value32_t rn_value,
    register_value32_t rm_value
) {
    return ra_value + rn_value * rm_value;
}

int64_t madd_64(
    register_value64_t ra_value,
    register_value64_t rn_value,
    register_value64_t rm_value
) {
    return ra_value + rn_value * rm_value;
}

bool EQ(void) {
    return (PSR.Z == 1);
}

bool NE(void) {
    return (PSR.Z == 0);
}

bool GE(void) {
    return (PSR.N == PSR.V);
}

bool LT(void) {
    return !GE();
}

bool GT(void) {
    return (NE() && GE());
}

bool LE(void) {
    return !GT();
}

bool AL(void) {
    return true;
}

void b(int64_t offset) {
    branch_PC(offset, 0);
}

void br(register_value32_t xn) {
    branch_PC(0, xn);
}

void b_cond(int cond, int64_t offset) {
    if (cond == EQ_HEX_VALUE && EQ()) {
        branch_PC(offset, 0);
    } else if (cond == NE_HEX_VALUE && NE()) {
        branch_PC(offset, 0);
    } else if (cond == GE_HEX_VALUE && GE()) {
        branch_PC(offset, 0);
    } else if (cond == LT_HEX_VALUE && LT()) {
        branch_PC(offset, 0);
    } else if (cond == GT_HEX_VALUE && GT()) {
        branch_PC(offset, 0);
    } else if (cond == LE_HEX_VALUE && LE()) {
        branch_PC(offset, 0);
    } else if (cond == AL_HEX_VALUE && AL()) {
        branch_PC(offset, 0);
    }
}