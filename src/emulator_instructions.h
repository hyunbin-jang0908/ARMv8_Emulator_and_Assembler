#include <stdbool.h>
#include <stdint.h>

#include "emulator_decode.h"
#include "emulator_memory.h"

extern struct ProcessorStateRegister PSR;

#define OPERAND_SHIFT_BITS_16 16

#define CLEAR_16_BITS 0xffffU
#define CLEAR_TOP_32_BITS 0xffffffffU

#define EQ_HEX_VALUE 0x0U
#define NE_HEX_VALUE 0x1U
#define GE_HEX_VALUE 0xaU
#define LT_HEX_VALUE 0xbU
#define GT_HEX_VALUE 0xcU
#define LE_HEX_VALUE 0xdU
#define AL_HEX_VALUE 0xeU

uint64_t lsl(uint64_t operand, int shift_amount, bool is_32_bit);

uint64_t lsr(uint64_t operand, int shift_amount, bool is_32_bit);

uint64_t asr(uint64_t operand, int shift_amount, bool is_32_bit);

uint64_t ror(uint64_t operand, int shift_amount, bool is_32_bit);

int add_32(register_value32_t rn_value, int op2);

int64_t add_64(register_value64_t rn_value, int64_t op2);

int adds_32(register_value32_t rn_value, int op2);

int64_t adds_64(register_value64_t rn_value, int64_t op2);

int sub_32(register_value32_t rn_value, int op2);

int64_t sub_64(register_value64_t rn_value, int64_t op2);

int subs_32(register_value32_t rn_value, int op2);

int64_t subs_64(register_value64_t rn_value, int64_t op2);

int movn_32(int op);

int64_t movn_64(int64_t op);

int movz_32(int op);

int64_t movz_64(int64_t op);

int movk_32(register_value32_t rd, unsigned int imm16, int hw);

int64_t movk_64(register_value64_t rd, unsigned int imm16, int hw);

int and_32(register_value32_t rn_value, int op2);

int64_t and_64(register_value64_t rn_value, int64_t op2);

int bic_32(register_value32_t rn_value, int op2);

int64_t bic_64(register_value64_t rn_value, int64_t op2);

int orr_32(register_value32_t rn_value, int op2);

int64_t orr_64(register_value64_t rn_value, int64_t op2);

int orn_32(register_value32_t rn_value, int op2);

int64_t orn_64(register_value64_t rn_value, int64_t op2);

int eor_32(register_value32_t rn_value, int op2);

int64_t eor_64(register_value64_t rn_value, int64_t op2);

int eon_32(register_value32_t rn_value, int op2);

int64_t eon_64(register_value64_t rn_value, int64_t op2);

int ands_32(register_value32_t rn_value, int op2);

int64_t ands_64(register_value64_t rn_value, int64_t op2);

int bics_32(register_value32_t rn_value, int op2);

int64_t bics_64(register_value64_t rn_value, int64_t op2);

int msub_32(
    register_value32_t ra_value,
    register_value32_t rn_value,
    register_value32_t rm_value
);

int64_t msub_64(
    register_value64_t ra_value,
    register_value64_t rn_value,
    register_value64_t rm_value
);

int madd_32(
    register_value32_t ra_value,
    register_value32_t rn_value,
    register_value32_t rm_value
);

int64_t madd_64(
    register_value64_t ra_value,
    register_value64_t rn_value,
    register_value64_t rm_value
);

bool EQ(void);

bool NE(void);

bool GE(void);

bool LT(void);

bool GT(void);

bool LE(void);

bool AL(void);

void b(int64_t offset);

void br(register_value32_t xn);

void b_cond(int cond, int64_t offset);
