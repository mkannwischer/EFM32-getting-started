#include "hal-aes.h"
#include "em_crypto.h"

#define AESCTR_NONCEBYTES 12
#define AES_BLOCKBYTES 16

void hal_aes128_ecb(uint8_t *out, const uint8_t *in, size_t nblocks, const uint8_t *key)
{
    CRYPTO_AES_ECB128(CRYPTO0, out, in, AES_BLOCKBYTES*nblocks, key, 1);
}


void hal_aes256_ecb(uint8_t *out, const uint8_t *in, size_t nblocks, const uint8_t *key)
{
    CRYPTO_AES_ECB256(CRYPTO0, out, in, AES_BLOCKBYTES*nblocks, key, 1);
}
