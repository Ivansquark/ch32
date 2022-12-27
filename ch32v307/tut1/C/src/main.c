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

int main(void) {
    uint8_t i = 0;
    int y = 0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGrou:wqp_2);
    // GPIO_Toggle_INIT();
    RCC->APB2PCENR |= RCC_IOPAEN;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF15;


    while (1) {
        // GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i =
        // Bit_RESET));
        i++;
        y = i;
        GPIOA->BSHR |= GPIO_BSHR_BS15;
        for(volatile int i = 0; i < 100000; i++) {}
        GPIOA->BSHR |= GPIO_BSHR_BR15;
        for(volatile int i = 0; i < 100000; i++) {}
    }
}
