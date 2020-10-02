#include "hal-sha2.h"
#include "em_crypto.h"

void hal_sha256(uint8_t *out, const uint8_t *in, size_t inlen)
{
    CRYPTO_SHA_256(CRYPTO0, in, inlen, out);
}