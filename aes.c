// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

#include "randombytes.h"
#include "hal.h"
#include "hal-aes.h"
#include "sendfn.h"

#include <string.h>
#include <stdio.h>


const unsigned char msg[48] = "The quick brown fox jumps over the lazy dog!!!!";

const unsigned char ct128[48] = {
0x10, 0xdc, 0x43, 0x2b, 0x15, 0x11, 0x81, 0x36, 0x3f, 0x00, 0x51, 0x74, 0x81, 0x7c, 0x22, 0x87,
0x3a, 0x3b, 0xfe, 0xd7, 0xb9, 0xa6, 0xf2, 0x3c, 0x81, 0x00, 0x63, 0xef, 0xe5, 0xb8, 0xbd, 0x36,
0x11, 0xcc, 0xc9, 0xdf, 0x2b, 0xea, 0xbc, 0xe6, 0x11, 0x1c, 0x34, 0x79, 0xf9, 0x6b, 0x47, 0x7b};

const unsigned char ct256[48] = {
0xdb, 0xb5, 0x44, 0x70, 0x68, 0xe6, 0xad, 0x6a, 0x09, 0xdf, 0xa6, 0xef, 0x85, 0x73, 0xff, 0xc0,
0xc2, 0x91, 0x38, 0xbd, 0xd7, 0xd0, 0x22, 0x7e, 0x79, 0x71, 0xa1, 0x98, 0x6f, 0xd5, 0x80, 0xa8,
0x1e, 0x97, 0xd7, 0x6d, 0xd2, 0x6b, 0x0e, 0x7b, 0x79, 0x76, 0x75, 0x86, 0xa5, 0x2f, 0x76, 0x0b};

const unsigned char key[64] = {
  0x66, 0xd9, 0xb7, 0x60, 0x0e, 0xda, 0xaa, 0x81, 0x42, 0xa2, 0xd6, 0x3d, 0x8f, 0x51, 0x6c, 0x6f,
  0xb6, 0xdf, 0x5b, 0x97, 0xf3, 0xf1, 0xf7, 0x0e, 0xeb, 0xe0, 0x40, 0x4d, 0xc5, 0x24, 0xa1, 0xfa};

int main(void)
{
  volatile uint64_t t0, t1;
  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

  uint8_t buf[(1<<12)*16];
  char str[100];

  hal_aes128_ecb(buf, msg, 3, key);
  if(memcmp(buf, ct128, 48)) {
    hal_send_str("ERROR hal_aes128_ecb output does not match test vector.\n");
  } else {
    hal_send_str("OK hal_aes128_ecb.\n");
    send_unsignedll("Count:", t1 - t0);
  }


  hal_aes256_ecb(buf, msg, 3, key);

  if(memcmp(buf, ct256, 48)) {
    hal_send_str("ERROR hal_aes256_ecb output does not match test vector.\n");
  } else {
    hal_send_str("OK hal_aes256_ecb.\n");
  }



  hal_send_str("benchmarking hal_aes128_ecb.");

  for(size_t i=1;i<=(1<<12);i<<=1){
    t0 = hal_get_time();
    hal_aes128_ecb(buf, msg, i, key);
    t1 = hal_get_time();

    sprintf(str, "%u bytes: %llu cycles --> %f cycles/byte", i*16, t1-t0, (double) (t1-t0) / (i*16));
    hal_send_str(str);
  }



  hal_send_str("benchmarking hal_aes256_ecb.");

  for(size_t i=1;i<=(1<<12);i<<=1){
    t0 = hal_get_time();
    hal_aes256_ecb(buf, msg, i, key);
    t1 = hal_get_time();

    sprintf(str, "%u bytes: %llu cycles --> %f cycles/byte", i*16, t1-t0, (double) (t1-t0) / (i*16));
    hal_send_str(str);
  }


  hal_send_str("#");
  while (1)
    ;
  return 0;
}