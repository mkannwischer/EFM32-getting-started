// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

#include "randombytes.h"
#include "hal.h"
#include "sendfn.h"

#include <string.h>
#include <stdio.h>

int main(void)
{
    volatile uint64_t t0, t1;
    uint32_t num;
    hal_setup(CLOCK_BENCHMARK);

    hal_send_str("==========================");
    hal_send_str("Hello. I'm a Giant Gecko and I would like to play!\n\n");

    // cycle counting
    hal_send_str("I can count cycles up to 2^64, look:");
    t0 = hal_get_time();
    for(int i=0;i< (1<<16);i++){
      asm("nop");
    }
    t1 = hal_get_time();
    send_unsignedll("Count:", t1-t0);

    // TRNG
    hal_send_str("I can also produce true random numbers:");
    randombytes((uint8_t *)&num, 4);
    send_unsigned("Today's lucky number is:", num);


    hal_send_str("#");
    while(1);
    return 0;
}
