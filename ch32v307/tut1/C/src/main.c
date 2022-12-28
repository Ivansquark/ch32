#include "main.h"
// void GPIO_Toggle_INIT(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure = {0};
//
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
void delay(volatile uint32_t val);
int main(void) {
    uint8_t i = 0;
    int y = 0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGrou:wqp_2);
    // GPIO_Toggle_INIT();
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF15;
    RCC->APB2PCENR |= RCC_IOPBEN;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4;

    while (1) {
        // GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i =
        // Bit_RESET));
        i++;
        y = i;
        GPIOA->BSHR |= GPIO_BSHR_BS15;
        GPIOB->BSHR |= GPIO_BSHR_BR4;
        delay(1000000);
        GPIOA->BSHR |= GPIO_BSHR_BR15;
        GPIOB->BSHR |= GPIO_BSHR_BS4;
        delay(1000000);
    }
}

void delay(volatile uint32_t val) {
    while (val--) {}
}
