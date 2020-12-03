// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>
#include "hal-mul.h"
#include "em_crypto.h"

void hal_mul128(uint32_t r[8], const uint32_t a[4], const uint32_t b[4]){
    CRYPTO0->WAC = CRYPTO_WAC_MULWIDTH_MUL128 | CRYPTO_WAC_RESULTWIDTH_256BIT;

    // select DDATA3 as second operand (V1) of multiplication
    CRYPTO0->CMD = CRYPTO_CMD_INSTR_SELDDATA1DDATA3;

    CRYPTO_SEQ_LOAD_2(CRYPTO0,
                     CRYPTO_CMD_INSTR_DDATA2TODDATA1,
                     CRYPTO_CMD_INSTR_MUL);

    // Load 128-bit a into DATA2 (lower half of DDATA3)
    CRYPTO_DataWrite(&CRYPTO0->DATA2, a);

    // LOAD 128-bit b into DATA0 (lower half of DDATA2)
    CRYPTO_DataWrite(&CRYPTO0->DATA0, b);

    // Execute
    CRYPTO_InstructionSequenceExecute(CRYPTO0);

    // Retrieve 256-bit result
    CRYPTO_DDataRead(&CRYPTO0->DDATA0, r);
}
void hal_mul256(uint32_t r[16], const uint32_t a[8], const uint32_t b[8]){
    // select DDATA3 as second operand (V1) of multiplication
    CRYPTO0->CMD = CRYPTO_CMD_INSTR_SELDDATA1DDATA3;

    // Load 256-bit a into DDATA1
    CRYPTO_DDataWrite(&CRYPTO0->DDATA1, a);

    // LOAD 256-bit b into DDATA3
    CRYPTO_DDataWrite(&CRYPTO0->DDATA3, b);

    // Execute
    CRYPTO0->CMD = CRYPTO_CMD_INSTR_LMUL;

    // Retrieve 512-bit result
    CRYPTO_QDataRead(&CRYPTO0->QDATA0, r);
}
