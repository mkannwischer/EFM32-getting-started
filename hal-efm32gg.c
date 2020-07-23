// Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

#include "hal.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"
#include "em_chip.h"
#include "randombytes.h"

#include <stddef.h>
#include <stdint.h>

static void usart_setup(void)
{
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    // Enable oscillator to GPIO and USART1 modules
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART5, true);

    // set pin modes for UART TX; we don't need to RX
    //GPIO_PinModeSet(gpioPortE, 9, gpioModeInput, 0);
    GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 1);

    // Initialize USART asynchronous mode and route pins
    USART_InitAsync(USART5, &init);
    // USART5->ROUTEPEN |= USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;
    USART5->ROUTEPEN |= USART_ROUTEPEN_TXPEN;
}

static void trng_setup(void)
{
    uint8_t i = 0, j = 0;

    CMU_ClockEnable(cmuClock_TRNG0, true);

    TRNG0->CONTROL = TRNG_CONTROL_SOFTRESET;
    TRNG0->CONTROL = 0;

    TRNG0->CONTROL = TRNG_CONTROL_CONDBYPASS | TRNG_CONTROL_ENABLE;

    // Read FIFO at least 257 times to ensure randomness
    // of entropy source
    for (i = 0; i < 5; i++)
    {
        while (TRNG0->FIFOLEVEL < 64)
            ;
        for (j = 0; j < 64; j++)
        {
            TRNG0->FIFO;
        }
    }

    // Program random key and enable conditioning function
    TRNG0->KEY0 = TRNG0->FIFO;
    TRNG0->KEY1 = TRNG0->FIFO;
    TRNG0->KEY2 = TRNG0->FIFO;
    TRNG0->KEY3 = TRNG0->FIFO;

    TRNG0->CONTROL = TRNG_CONTROL_ENABLE;
}
static volatile unsigned long long overflowcnt = 0;
void SysTick_Handler(void)
{
    overflowcnt++;
}

static void cyccnt_setup(void)
{
    SysTick_Config(16777215);

    //alternatively, if a 32-bit counter is enough:
    //CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    //DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    //DWT->CYCCNT = 0;
}

int randombytes(uint8_t *buf, size_t len)
{
    union
    {
        uint8_t aschar[4];
        uint32_t asint;
    } random;

    while (len > 4)
    {
        while (!TRNG0->FIFOLEVEL)
            ;
        random.asint = TRNG0->FIFO;
        *buf++ = random.aschar[0];
        *buf++ = random.aschar[1];
        *buf++ = random.aschar[2];
        *buf++ = random.aschar[3];
        len -= 4;
    }
    if (len > 0)
    {
        while (!TRNG0->FIFOLEVEL)
            ;
        random.asint = TRNG0->FIFO;
        for (; len > 0; len--)
        {
            *buf++ = random.aschar[len - 1];
        }
    }
    return 0;
}

static void clock_setup(const enum clock_mode clock)
{
    // Chip errata
    CHIP_Init();

    // Set frequency
    switch (clock)
    {
    case CLOCK_FAST:
        // Maximum frequency for EFM32GG11
        CMU_HFRCOFreqSet(cmuHFRCOFreq_72M0Hz);
        break;
    case CLOCK_BENCHMARK:
        // Maximum frequency such that there are no wait states
        CMU_HFRCOFreqSet(cmuHFRCOFreq_16M0Hz);
        break;
    }
}

void hal_setup(const enum clock_mode clock)
{
    clock_setup(clock);
    usart_setup();
    trng_setup();
    cyccnt_setup();
}

void hal_send_str(const char *in)
{
    int i;
    for (i = 0; in[i] != 0; i++)
    {
        USART_Tx(USART5, *(unsigned char *)(in + i));
    }
    USART_Tx(USART5, '\n');
}

uint64_t hal_get_time(void)
{
    //alternatively, if a 32-bit counter is enough:
    //return (uint64_t) DWT->CYCCNT;

    while (true)
    {
        unsigned long long before = overflowcnt;
        unsigned long long result = (before + 1) * 16777216llu - (SysTick->VAL);
        if (overflowcnt == before)
        {
            return result;
        }
    }
}