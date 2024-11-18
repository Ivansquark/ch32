#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

namespace Glob {
constexpr unsigned SysClk = 48;
} // namespace Glob

namespace TUart {
enum Uarts {
    UART1_NO_DMA,
    UART1_DMA
};

//constexpr Uarts Uart1Dma = Uarts::UART1_DMA;
constexpr Uarts Uart1Dma = Uarts::UART1_NO_DMA;

constexpr Uarts uarts[2] = {UART1_NO_DMA, UART1_DMA};

} // namespace TUart

#endif // GLOBAL_CONSTANTS_H
