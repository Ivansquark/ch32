#include "dac.h"

void dac_init() {
    /*!< U-PA4 I-PA5 dac-output >*/
    RCC->APB2PCENR |= RCC_IOPAEN;
    // 0:0:0:0 AIN
    //GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF4 | GPIO_CFGLR_CNF5);
    GPIOA->CFGLR |= GPIO_CFGLR_CNF4_1 | GPIO_CFGLR_CNF5_1;
    GPIOA->CFGLR &= ~(GPIO_CFGLR_MODE4 | GPIO_CFGLR_MODE5);
    RCC->APB1PCENR |= RCC_DACEN;
    DAC->CTLR |= DAC_EN1; // DAC channel 1 enabled
    DAC->CTLR |= DAC_EN2;
    DAC->DOR1 = 0; // DAC channel1 12-bit right-aligned data holding register
    DAC->DOR2 = 0; // DAC channel1 12-bit right-aligned data holding register
}

void dac1_set(uint16_t val) { DAC->R12BDHR1 = val; }
void dac2_set(uint16_t val) { DAC->R12BDHR2 = val; }
