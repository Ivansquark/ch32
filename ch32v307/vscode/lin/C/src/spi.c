#include "spi.h"


uint8_t spi2_readWriteByte(uint8_t byte) {
    uint8_t i = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_STATR_TXE) == RESET) {
        i++;
        if (i > 200) return 0;
    }
    SPI_I2S_SendData(SPI2, byte);
    i = 0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_STATR_RXNE) == RESET) {
        i++;
        if (i > 200) return 0;
    }
    return SPI_I2S_ReceiveData(SPI2);
}

void spi2_init() {
    // B13-CLK, B15-MOSI
    GPIOB->CFGHR |= GPIO_CFGHR_MODE13 | GPIO_CFGHR_MODE15;
    GPIOB->CFGHR &= ~(GPIO_CFGHR_CNF13 | GPIO_CFGHR_CNF15);
    GPIOB->CFGHR |= (GPIO_CFGHR_CNF13_1 | GPIO_CFGHR_CNF15_1);
    // B14-MISO CNF_0:1-input floating
    GPIOB->CFGHR &= ~GPIO_CFGHR_MODE14;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF14_0;
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF14_1;

    // TODO: SPI init
    SPI_InitTypeDef SPI_InitStructure = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    RCC->APB1PCENR |= RCC_APB1Periph_SPI2;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    // SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    // SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPI_Cmd(SPI2, ENABLE);
}

