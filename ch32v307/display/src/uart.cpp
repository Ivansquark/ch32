#include "uart.h"

Uart5* Uart5::pThis = nullptr;

Uart5::Uart5() {
    init();
    pThis = this;
}

void Uart5::runTask([[maybe_unused]] void* pvParameter) {
    uint16_t adcBat = (Uart1::pThis->rxBuf[0] << 2);
    uint16_t adc5V = (Uart1::pThis->rxBuf[1] << 2);
    float adcBat_f = (float)adcBat * 5 / 1024;
    float adc5V_f = (float)adc5V * 5 * 2 / 1024;
    uint8_t adcBat_arr[5] = {0};
    uint8_t adc5V_arr[5] = {0};
    sprintf((char*)adcBat_arr, "%.2f", adcBat_f);
    sprintf((char*)adc5V_arr, "%.2f", adc5V_f);
    TxBuff[0] = 'B';
    TxBuff[1] = '+';
    TxBuff[2] = '=';
    TxBuff[3] = adcBat_arr[0];
    TxBuff[4] = adcBat_arr[1];
    TxBuff[5] = adcBat_arr[2];
    TxBuff[6] = adcBat_arr[3];
    TxBuff[7] = '\t';

    TxBuff[8] = '+';
    TxBuff[9] = '5';
    TxBuff[10] = '=';
    TxBuff[11] = adc5V_arr[0];
    TxBuff[12] = adc5V_arr[1];
    TxBuff[13] = adc5V_arr[2];
    TxBuff[14] = adc5V_arr[3];
    TxBuff[15] = '\t';
    uint16_t adcI = (Uart3::pThis->rxBuf[0] << 2);
    uint16_t adcBuck = (Uart3::pThis->rxBuf[1] << 2);
    uint16_t adcBoost = (Uart3::pThis->rxBuf[2] << 2);
    float adcI_f = (float)adcI * 5 / 1024;
    float adcBuck_f = (float)adcBuck * 2 / 1024;
    float adcBoost_f = (float)adcBoost * 5 / 1024;
    uint8_t adcI_arr[5] = {0};
    uint8_t adcBuck_arr[5] = {0};
    uint8_t adcBoost_arr[5] = {0};
    sprintf((char*)adcI_arr, "%.2f", adcI_f);
    sprintf((char*)adcBuck_arr, "%.2f", adcBuck_f);
    sprintf((char*)adcBoost_arr, "%.2f", adcBoost_f);

    TxBuff[16] = 'I';
    TxBuff[17] = '=';
    TxBuff[18] = adcI_arr[0];
    TxBuff[19] = adcI_arr[1];
    TxBuff[20] = adcI_arr[2];
    TxBuff[21] = adcI_arr[3];
    TxBuff[22] = '\t';

    TxBuff[23] = 'B';
    TxBuff[24] = 'U';
    TxBuff[25] = '=';
    TxBuff[26] = adcBuck_arr[0];
    TxBuff[27] = adcBuck_arr[1];
    TxBuff[28] = adcBuck_arr[2];
    TxBuff[29] = adcBuck_arr[3];
    TxBuff[30] = '\t';

    TxBuff[31] = 'B';
    TxBuff[32] = 'O';
    TxBuff[33] = '=';
    TxBuff[34] = adcBoost_arr[0];
    TxBuff[35] = adcBoost_arr[1];
    TxBuff[36] = adcBoost_arr[2];
    TxBuff[37] = adcBoost_arr[3];
    TxBuff[38] = '\r';
    TxBuff[39] = '\n';

    while (1) {
        adcBat = (Uart1::pThis->rxBuf[0] << 2);
        adc5V = (Uart1::pThis->rxBuf[1] << 2);
        adcBat_f = (float)adcBat * 5 / 1024;
        adc5V_f = (float)adc5V * 5 * 2 / 1024;
        sprintf((char*)adcBat_arr, "%.2f", adcBat_f);
        sprintf((char*)adc5V_arr, "%.2f", adc5V_f);
        TxBuff[3] = adcBat_arr[0];
        TxBuff[4] = adcBat_arr[1];
        TxBuff[5] = adcBat_arr[2];
        TxBuff[6] = adcBat_arr[3];

        TxBuff[11] = adc5V_arr[0];
        TxBuff[12] = adc5V_arr[1];
        TxBuff[13] = adc5V_arr[2];
        TxBuff[14] = adc5V_arr[3];

        adcI = (Uart3::pThis->rxBuf[0] << 2);
        adcBuck = (Uart3::pThis->rxBuf[1] << 2);
        adcBoost = (Uart3::pThis->rxBuf[2] << 2);
        adcI_f = (float)adcI * 5 / 1024;
        adcBuck_f = (float)adcBuck * 2 / 1024;
        adcBoost_f = (float)adcBoost * 5 / 1024;

        TxBuff[18] = adcI_arr[0];
        TxBuff[19] = adcI_arr[1];
        TxBuff[20] = adcI_arr[2];
        TxBuff[21] = adcI_arr[3];

        TxBuff[26] = adcBuck_arr[0];
        TxBuff[27] = adcBuck_arr[1];
        TxBuff[28] = adcBuck_arr[2];
        TxBuff[29] = adcBuck_arr[3];

        TxBuff[34] = adcBoost_arr[0];
        TxBuff[35] = adcBoost_arr[1];
        TxBuff[36] = adcBoost_arr[2];
        TxBuff[37] = adcBoost_arr[3];

        sendBuf(TxBuff, 40);

        Uart1::pThis->rxCounter = 0;
        Uart3::pThis->rxCounter = 0;

        Uart1::pThis->sendByte('>');
        Uart3::pThis->sendByte('>');
        vTaskDelay(200);
    }
}

void Uart5::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(UART5->STATR & USART_STATR_TXE)) && timeout--) {}
    UART5->DATAR = byte;
}
void Uart5::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}

void Uart5::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart5::init() {
    //! _______USART 3 init  pin D8-Tx D9-Rx AF7 remap 1:1  ___________________
    RCC->APB2PCENR |= RCC_IOPBEN;
    // Tx AF-push/pull
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF4_0;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF4_1;
    GPIOB->CFGLR |= GPIO_CFGLR_MODE4;
    // Rx in_floating
    GPIOB->CFGLR &= ~GPIO_CFGLR_CNF5_1;
    GPIOB->CFGLR |= GPIO_CFGLR_CNF5_0;
    GPIOB->CFGLR &= ~GPIO_CFGLR_MODE5;

    RCC->APB2PCENR |= RCC_AFIOEN;
    // 0:1 remap uart5 on b4,b5
    AFIO->PCFR2 |= AFIO_PCFR2_UART5_REMAP_01;

    //------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_UART5EN;
    UART5->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    // UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    UART5->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(UART5_IRQn);
}

extern "C" __attribute__((interrupt)) void UART5_IRQHandler(void) {
    if (UART5->STATR & USART_STATR_RXNE) { UART5->STATR &= ~USART_STATR_RXNE; }
}

Uart1* Uart1::pThis = nullptr;

Uart1::Uart1() {
    init();
    pThis = this;
}
void Uart1::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(USART1->STATR & USART_STATR_TXE)) && timeout--) {}
    USART1->DATAR = byte;
}
void Uart1::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}

void Uart1::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart1::init() {
    //! _______USART 1 init  pin A9-Tx A10-Rx _________________________________
    RCC->APB2PCENR |= RCC_IOPAEN;
    // Tx AF-push/pull
    GPIOA->CFGHR &= ~GPIO_CFGHR_CNF9_0;
    GPIOA->CFGHR |= GPIO_CFGHR_CNF9_1;
    GPIOA->CFGHR |= GPIO_CFGHR_MODE9;
    // Rx in_floating
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF10_1;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10_0;
    GPIOB->CFGHR &= ~GPIO_CFGHR_MODE10;

    //------------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_USART1EN;
    USART1->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    // UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    USART1->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" __attribute__((interrupt)) void USART1_IRQHandler(void) {
    if (USART1->STATR & USART_STATR_RXNE) {
        USART1->STATR &= ~USART_STATR_RXNE;
        Uart1::pThis->rxBuf[Uart1::pThis->rxCounter] = USART1->DATAR;
    }
}
//-----------------------------------------------------------------------------
Uart3* Uart3::pThis = nullptr;

Uart3::Uart3() {
    init();
    pThis = this;
}
void Uart3::sendByte(uint8_t byte) {
    uint32_t timeout = 0xFFFF;
    while ((!(USART3->STATR & USART_STATR_TXE)) && timeout--) {}
    USART3->DATAR = byte;
}
void Uart3::sendStr(const char* str) {
    uint8_t i = 0;
    while (str[i] != '\0') { sendByte(str[i++]); }
}
void Uart3::sendBuf(uint8_t* data, uint8_t len) {
    for (int i = 0; i < len; i++) { sendByte(data[i]); }
}
void Uart3::init() {
    //! _______USART 3 init  pin B10-Tx B11-Rx ________________________________
    RCC->APB2PCENR |= RCC_IOPBEN;
    // Tx AF-push/pull
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF10_0;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF10_1;
    GPIOB->CFGHR |= GPIO_CFGHR_MODE10;
    // Rx in_floating
    GPIOB->CFGHR &= ~GPIO_CFGHR_CNF11_1;
    GPIOB->CFGHR |= GPIO_CFGHR_CNF11_0;
    GPIOB->CFGHR &= ~GPIO_CFGHR_MODE11;

    //------------------- USART -------------------------------------
    RCC->APB1PCENR |= RCC_UART5EN;
    USART3->CTLR1 |=
        USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE | USART_CTLR1_UE;
    // 72 MHz 9600 KB/s
    // 9600 = 72000000/(16*USARTDIV) =>
    // USARTDIV = 72000000/(16*9600) = 468,75
    // USARTDIV = DIV_M+(DIV_F/16) = 468 + (16*0.75) = 468_M + 12_F
    // UART5->BRR |= (468 << 4) | (12); // 9600
    // 144 MHz 9600 KB/s
    // 9600 = 144000000/(16*USARTDIV) =>
    // USARTDIV = 144000000/(16*9600) = 937,5
    // USARTDIV = DIV_M+(DIV_F/16) = 937 + (16*0.5) = 937_M + 8_F
    USART3->BRR |= (937 << 4) | (8); // 9600
    NVIC_EnableIRQ(USART3_IRQn);
}

extern "C" __attribute__((interrupt)) void USART3_IRQHandler(void) {
    if (USART3->STATR & USART_STATR_RXNE) {
        USART3->STATR &= ~USART_STATR_RXNE;
        Uart3::pThis->rxBuf[Uart3::pThis->rxCounter] = USART3->DATAR;
    }
}
