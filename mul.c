// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

#include "randombytes.h"
#include "hal.h"
#include "hal-mul.h"

#include <string.h>
#include <stdio.h>


static void  ref_mul128(uint32_t r[8], const uint32_t a[4], const uint32_t b[4]) {
    int i,j,k;
    uint64_t tmp;

    for(i=0;i<8;i++){
        r[i] = 0;
    }

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){

            tmp = (uint64_t)a[i] * b[j];

            // carry
            for(k=i+j;k<8;k++){
                tmp += r[k];
                r[k] = (uint32_t) tmp;
                tmp >>= 32;
            }
        }
    }
}
static void ref_mul256(uint32_t r[16], const uint32_t a[8], const uint32_t b[8]) {
    int i,j,k;
    uint64_t tmp;

    for(i=0;i<16;i++){
        r[i] = 0;
    }

    for(i=0;i<8;i++){
        for(j=0;j<8;j++){

            tmp = (uint64_t)a[i] * b[j];

            // carry
            for(k=i+j;k<16;k++){
                tmp += r[k];
                r[k] = (uint32_t) tmp;
                tmp >>= 32;
            }
        }
    }
}


static void testmul128(void) {
  volatile uint64_t t0, t1;
  int i;
  char str[100];
  uint32_t a[4];
  uint32_t b[4];
  uint32_t r0[8];
  uint32_t r1[8];

  randombytes((uint8_t *)a, sizeof a);
  randombytes((uint8_t *)b, sizeof b);
  randombytes((uint8_t *)r0, sizeof r0);
  randombytes((uint8_t *)r1, sizeof r1);

  ref_mul128(r0, a, b);

  t0 = hal_get_time();
  for(i = 0;i<10000;i++){
    hal_mul128(r1, a, b);
  }
  t1 = hal_get_time();


  sprintf(str, "10000 x hal_mul128 %llu cycles --> %f cycles/mul", t1-t0, (double) (t1-t0) / (10000.0));
  hal_send_str(str);
  if(memcmp(r0, r1, sizeof r0)) {
    hal_send_str("ERROR hal_mul128 output does not match reference.\n");
  } else {
    hal_send_str("OK hal_mul128.\n");
  }
}

static void testmul256(void){
  volatile uint64_t t0, t1;
  int i;
  char str[100];
  uint32_t a[8];
  uint32_t b[8];
  uint32_t r0[16];
  uint32_t r1[16];

  randombytes((uint8_t *)a, sizeof a);
  randombytes((uint8_t *)b, sizeof b);
  randombytes((uint8_t *)r0, sizeof r0);
  randombytes((uint8_t *)r1, sizeof r1);

  ref_mul256(r0, a, b);

  t0 = hal_get_time();
  for(i = 0;i<10000;i++){
    hal_mul256(r1, a, b);
  }
  t1 = hal_get_time();


  sprintf(str, "10000 x hal_mul256 %llu cycles --> %f cycles/mul", t1-t0, (double) (t1-t0) / (10000.0));
  hal_send_str(str);
  if(memcmp(r0, r1, sizeof r0)) {
    hal_send_str("ERROR hal_mul256 output does not match reference.\n");
  } else {
    hal_send_str("OK hal_mul256.\n");
  }
}

int main(void) {

  hal_setup(CLOCK_BENCHMARK);

  hal_send_str("==========================");

  testmul128();
  testmul256();

  hal_send_str("#");
  while (1)
    ;
  return 0;
}