#ifndef TUART_H
#define TUART_H

#include "global_constants.h"
#include "gpios.h"
#include "tdma.h"
#include <string.h>

namespace TUart {
enum BaudRate {
    B9600 = 9600,
    B115200 = 115200,
};

extern volatile bool IsReceived;

extern char data[32];

void sendStr(const char* arr, int len);

template <Uarts dmaMode>
void sendByte(char byte);
template <Uarts dmaMode>
void sendStr(const char* arr, int len);

consteval bool getVal() { return true; }

//------------------ Uart pins concepts ---------------------------------------
template <int baud>
concept IsBaud = (baud == B9600 || baud == B115200);

template <int num_tx, int num_rx>
concept IsProperNumber =
    num_tx == 0 && num_rx == 1 || num_tx == 5 && num_rx == 6 ||
    num_tx == 6 && num_rx == 5;

template <Gpios::PortName port_tx, Gpios::PortName port_rx>
concept IsProperPort = (port_tx == Gpios::PD && port_rx == Gpios::PD) ||
                       (port_tx == Gpios::PC && port_rx == Gpios::PC);

template <Gpios::PortName port_tx, int num_tx, Gpios::PortName port_rx,
          int num_rx, int baud>
concept FullPinCheck = IsBaud<baud> && IsProperPort<port_tx, port_rx> &&
                       IsProperNumber<num_tx, num_rx>;

// concept FullPinCheck = requires() {
//     requires IsBaud<baud>;
//     requires IsProperPort<port_tx, port_rx>;
//     requires IsProperNumber<num_tx, num_rx>;
// };
//-----------------------------------------------------------------------------

template <Gpios::PortName port_tx, int num_tx, Gpios::PortName port_rx,
          int num_rx, int baud>
// requires requires() {
//     requires IsBaud<baud>;
//     requires IsProperPort<port_tx, port_rx>;
//     requires IsProperNumber<num_tx, num_rx>;
// }
    requires FullPinCheck<port_tx, num_tx, port_rx, num_rx, baud>
class Uart {
  public:
    using Type = Gpios::PortName;
    Uart() {
        RCC->APB2PCENR |= RCC_USART1EN;
        USART1->CTLR1 |= USART_CTLR1_TE | USART_CTLR1_RE | USART_CTLR1_RXNEIE |
                         USART_CTLR1_UE;

        if constexpr (Uart1Dma == UART1_DMA) {
            TDma::Dma<TDma::Dma1, 4> dma;
        } else {
        }
        // check pins to remap
        if constexpr (port_tx == Gpios::PD) {
            if constexpr (num_tx == 0) {
                RCC->APB2PCENR |= RCC_AFIOEN;
                AFIO->PCFR1 |= (1 << 2);
                AFIO->PCFR1 &= ~(1 << 21);
            } else if constexpr (num_tx == 5) {
                Gpios::Out<Gpios::PD, 5,
                           Gpios::InitModeOut::ALTERNATE_PUSH_PULL>
                    tx;
                Gpios::In<Gpios::PD, 6, Gpios::InitModeIn::PULL_UP> rx;
                AFIO->PCFR1 &= ~(1 << 2);
                AFIO->PCFR1 &= ~(1 << 21);
            } else if constexpr (num_tx == 6) {
                RCC->APB2PCENR |= RCC_AFIOEN;
                Gpios::Out<Gpios::PD, 6,
                           Gpios::InitModeOut::ALTERNATE_PUSH_PULL>
                    tx;
                Gpios::In<Gpios::PD, 5, Gpios::InitModeIn::PULL_UP> rx;
                AFIO->PCFR1 &= ~(1 << 2);
                AFIO->PCFR1 |= (1 << 21);
            }
        } else if constexpr (port_tx == Gpios::PC) {
            RCC->APB2PCENR |= RCC_AFIOEN;
            AFIO->PCFR1 |= (1 << 21) | (1 << 2);
        }
        // check clock
        if constexpr (Glob::SysClk == 24) {
            if constexpr (baud == 9600) {
                // 9600 = 24000000/(16*USARTDIV) =>
                // USARTDIV = 24000000/(16*9600) = 156,25
                // USARTDIV = DIV_M+(DIV_F/16) = 156 + (16*0.25) = 156_M + 4_F
                USART1->BRR |= (156 << 4) | (4); // 9600 on 24MHz
            } else if constexpr (baud == 115200) {
                // 115200 = 24000000/(16*USARTDIV) =>
                // USARTDIV = 24000000/(16*115200) = 13.02
                // USARTDIV = DIV_M+(DIV_F/16) = 13 + (16*0.02) = 13_M + 0_F
                USART1->BRR |= (13 << 4) | (0); // 115200 on 24MHz
            }
        } else if constexpr (Glob::SysClk == 48) {
            if constexpr (baud == 9600) {
                // 9600 = 48000000/(16*USARTDIV) =>
                // USARTDIV = 48000000/(16*9600) = 312,5
                // USARTDIV = DIV_M+(DIV_F/16) = 312 + (16*0.5) = 312_M + 8_F
                USART1->BRR |= (312 << 4) | (8); // 9600 on 48MHz
            } else if constexpr (baud == 115200) {
                // 115200 = 48000000/(16*USARTDIV) =>
                // USARTDIV = 48000000/(16*115200) = 26.04
                // USARTDIV = DIV_M+(DIV_F/16) = 26 + (16*0.04) = 26_M + 1_F
                USART1->BRR |= (26 << 4) | (1); // 115200 on 48MHz
            }
        }
        NVIC_EnableIRQ(USART1_IRQn);
    }

  private:
};

} // namespace TUart
#endif // TUART_H
