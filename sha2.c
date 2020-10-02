// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

#include "randombytes.h"
#include "hal.h"
#include "hal-sha2.h"
#include "sendfn.h"

#include <string.h>
#include <stdio.h>

const unsigned char msg[113] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
const unsigned char expected_256[32] = {
    0xcf, 0x5b, 0x16, 0xa7, 0x78, 0xaf, 0x83, 0x80, 0x03, 0x6c, 0xe5, 0x9e,
    0x7b, 0x04, 0x92, 0x37, 0x0b, 0x24, 0x9b, 0x11, 0xe8, 0xf0, 0x7a, 0x51,
    0xaf, 0xac, 0x45, 0x03, 0x7a, 0xfe, 0xe9, 0xd1
};


int main(void)
{
  volatile uint64_t t0, t1;
  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

  uint8_t buf[(1<<12)*16];
  uint8_t hash[32];
  char str[100];

  hal_sha256(hash, msg, 112);
  if(memcmp(hash, expected_256, sizeof expected_256)) {
    hal_send_str("ERROR hal_sha256 output does not match test vector.\n");
  } else {
    hal_send_str("OK hal_sha256.\n");
  }


  hal_send_str("benchmarking hal_sha256.");
  for(size_t i=1;i<=(1<<12);i<<=1){


    t0 = hal_get_time();
    hal_sha256(hash, buf, i*16);
    t1 = hal_get_time();

    sprintf(str, "%u bytes: %llu cycles --> %f cycles/byte", i*16, t1-t0, (double) (t1-t0) / (i*16));
    hal_send_str(str);
  }

  hal_send_str("#");
  while (1)
    ;
  return 0;
}
