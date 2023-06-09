#include "rcc.h"

uint32_t SystemCoreClock = 0;

Rcc::Rcc(uint8_t quarz) { init(quarz); }

void Rcc::init(uint8_t quarz) {
    // HSI on
    RCC->CTLR |= (uint32_t)0x00000001;
    // clear SW, HPRE, PPRE, ADCPRE, MCO
    RCC->CFGR0 &= (uint32_t)0xF0FF0000;
    // clear HSE_ON clear CSS_ON clear PLL_ON
    RCC->CTLR &= (uint32_t)0xFEF6FFFF;
    // clear HSE_BYP
    RCC->CTLR &= (uint32_t)0xFFFBFFFF;
    // clear PLL_MUL, PLL_XTPRE, PLL_SRC
    RCC->CFGR0 &= (uint32_t)0xFF80FFFF;
    // clear interrupt flags
    RCC->INTR = 0x009F0000;
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    switch (quarz) {
    case 8:
        RCC->CTLR |= ((uint32_t)RCC_HSEON);
        do {
            HSEStatus = RCC->CTLR & RCC_HSERDY;
            StartUpCounter++;
        } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
        if ((RCC->CTLR & RCC_HSERDY) != RESET) {
            HSEStatus = (uint32_t)0x01;
        } else {
            HSEStatus = (uint32_t)0x00;
        }
        if (HSEStatus == (uint32_t)0x01) {
            /* HCLK = SYSCLK */
            RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
            /* PCLK2 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV2;
            /* PCLK1 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;
            RCC->CFGR0 &= (uint32_t)(
                (uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

            /*  PLL configuration: PLLCLK = HSE * 18 = 144 MHz */
            RCC->CFGR0 |=
                (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18);
            /*  PLL configuration: PLLCLK = HSE * 12 = 96 MHz */
            // RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE |
            //                         RCC_PLLMULL12_EXTEN);

            /* Enable PLL */
            RCC->CTLR |= RCC_PLLON;
            /* Wait till PLL is ready */
            while ((RCC->CTLR & RCC_PLLRDY) == 0) {}
            /* Select PLL as system clock source */
            RCC->CFGR0 &= (uint32_t)((uint32_t) ~(RCC_SW));
            RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08) {}
            SystemCoreClock = 144000000;
        } else {
            /* If HSE fails to start-up, the application will have wrong clock
             * configuration. User can add here some code to deal with this
             * error
             */
        }

        break;
    case 24:
        RCC->CTLR |= ((uint32_t)RCC_HSEON);
        do {
            HSEStatus = RCC->CTLR & RCC_HSERDY;
            StartUpCounter++;
        } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
        if ((RCC->CTLR & RCC_HSERDY) != RESET) {
            HSEStatus = (uint32_t)0x01;
        } else {
            HSEStatus = (uint32_t)0x00;
        }
        if (HSEStatus == (uint32_t)0x01) {
            /* HCLK = SYSCLK */
            RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
            /* PCLK2 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
            /* PCLK1 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;
            /*  PLL configuration: PLLCLK = HSE * 6 = 144 MHz */
            RCC->CFGR0 &= (uint32_t)(
                (uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

            RCC->CFGR0 |=
                (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL6);

            /* Enable PLL */
            RCC->CTLR |= RCC_PLLON;
            /* Wait till PLL is ready */
            while ((RCC->CTLR & RCC_PLLRDY) == 0) {}
            /* Select PLL as system clock source */
            RCC->CFGR0 &= ~(RCC_SW);
            RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
            /* Wait till PLL is used as system clock source */
            while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08) {}
            SystemCoreClock = 144000000;
        } else {
            /* If HSE fails to start-up, the application will have wrong clock
             * configuration. User can add here some code to deal with this
             * error
             */
        }
        break;
    default:
        // HSI 8 MHz
        //        SetSysClockTo48_HSI
        EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;
        RCC->CFGR0 &=
            (uint32_t)((uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));
        RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL3);
        /* HCLK = SYSCLK = 24 */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
        /* PCLK2 = HCLK = 24*/
        RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
        /* PCLK1 = HCLK = 24*/
        RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV1;
        /*  PLL configuration: PLLCLK = HSI * 3 = 24 MHz */

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while ((RCC->CTLR & RCC_PLLRDY) == 0) {}
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t) ~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08) {}
        SystemCoreClock = 144000000;
        break;
    }
}
