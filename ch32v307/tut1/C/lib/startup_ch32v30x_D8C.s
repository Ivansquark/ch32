# 0 "../../../SRC/Startup/startup_ch32v30x_D8C.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "../../../SRC/Startup/startup_ch32v30x_D8C.S"
# 11 "../../../SRC/Startup/startup_ch32v30x_D8C.S"
 .section .init,"ax",@progbits
 .global _start
 .align 1
_start:
 j handle_reset
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00000013
 .word 0x00100073
    .section .vector,"ax",@progbits
    .align 1
_vector_base:
    .option norvc;
    .word _start
    .word 0
    .word NMI_Handler
    .word HardFault_Handler
    .word 0
    .word Ecall_M_Mode_Handler
    .word 0
    .word 0
    .word Ecall_U_Mode_Handler
    .word Break_Point_Handler
    .word 0
    .word 0
    .word SysTick_Handler
    .word 0
    .word SW_Handler
    .word 0

    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMPER_IRQHandler
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    .word RCC_IRQHandler
    .word EXTI0_IRQHandler
    .word EXTI1_IRQHandler
    .word EXTI2_IRQHandler
    .word EXTI3_IRQHandler
    .word EXTI4_IRQHandler
    .word DMA1_Channel1_IRQHandler
    .word DMA1_Channel2_IRQHandler
    .word DMA1_Channel3_IRQHandler
    .word DMA1_Channel4_IRQHandler
    .word DMA1_Channel5_IRQHandler
    .word DMA1_Channel6_IRQHandler
    .word DMA1_Channel7_IRQHandler
    .word ADC1_2_IRQHandler
    .word USB_HP_CAN1_TX_IRQHandler
    .word USB_LP_CAN1_RX0_IRQHandler
    .word CAN1_RX1_IRQHandler
    .word CAN1_SCE_IRQHandler
    .word EXTI9_5_IRQHandler
    .word TIM1_BRK_IRQHandler
    .word TIM1_UP_IRQHandler
    .word TIM1_TRG_COM_IRQHandler
    .word TIM1_CC_IRQHandler
    .word TIM2_IRQHandler
    .word TIM3_IRQHandler
    .word TIM4_IRQHandler
    .word I2C1_EV_IRQHandler
    .word I2C1_ER_IRQHandler
    .word I2C2_EV_IRQHandler
    .word I2C2_ER_IRQHandler
    .word SPI1_IRQHandler
    .word SPI2_IRQHandler
    .word USART1_IRQHandler
    .word USART2_IRQHandler
    .word USART3_IRQHandler
    .word EXTI15_10_IRQHandler
    .word RTCAlarm_IRQHandler
    .word USBWakeUp_IRQHandler
    .word TIM8_BRK_IRQHandler
    .word TIM8_UP_IRQHandler
    .word TIM8_TRG_COM_IRQHandler
    .word TIM8_CC_IRQHandler
    .word RNG_IRQHandler
    .word FSMC_IRQHandler
    .word SDIO_IRQHandler
    .word TIM5_IRQHandler
    .word SPI3_IRQHandler
    .word UART4_IRQHandler
    .word UART5_IRQHandler
    .word TIM6_IRQHandler
    .word TIM7_IRQHandler
    .word DMA2_Channel1_IRQHandler
    .word DMA2_Channel2_IRQHandler
    .word DMA2_Channel3_IRQHandler
    .word DMA2_Channel4_IRQHandler
    .word DMA2_Channel5_IRQHandler
    .word ETH_IRQHandler
    .word ETH_WKUP_IRQHandler
    .word CAN2_TX_IRQHandler
    .word CAN2_RX0_IRQHandler
    .word CAN2_RX1_IRQHandler
    .word CAN2_SCE_IRQHandler
    .word OTG_FS_IRQHandler
    .word USBHSWakeup_IRQHandler
    .word USBHS_IRQHandler
    .word DVP_IRQHandler
    .word UART6_IRQHandler
    .word UART7_IRQHandler
    .word UART8_IRQHandler
    .word TIM9_BRK_IRQHandler
    .word TIM9_UP_IRQHandler
    .word TIM9_TRG_COM_IRQHandler
    .word TIM9_CC_IRQHandler
    .word TIM10_BRK_IRQHandler
    .word TIM10_UP_IRQHandler
    .word TIM10_TRG_COM_IRQHandler
    .word TIM10_CC_IRQHandler
    .word DMA2_Channel6_IRQHandler
    .word DMA2_Channel7_IRQHandler
    .word DMA2_Channel8_IRQHandler
    .word DMA2_Channel9_IRQHandler
    .word DMA2_Channel10_IRQHandler
    .word DMA2_Channel11_IRQHandler

    .option rvc;

    .section .text.vector_handler, "ax", @progbits
    .weak NMI_Handler
    .weak HardFault_Handler
    .weak Ecall_M_Mode_Handler
    .weak Ecall_U_Mode_Handler
    .weak Break_Point_Handler
    .weak SysTick_Handler
    .weak SW_Handler
    .weak WWDG_IRQHandler
    .weak PVD_IRQHandler
    .weak TAMPER_IRQHandler
    .weak RTC_IRQHandler
    .weak FLASH_IRQHandler
    .weak RCC_IRQHandler
    .weak EXTI0_IRQHandler
    .weak EXTI1_IRQHandler
    .weak EXTI2_IRQHandler
    .weak EXTI3_IRQHandler
    .weak EXTI4_IRQHandler
    .weak DMA1_Channel1_IRQHandler
    .weak DMA1_Channel2_IRQHandler
    .weak DMA1_Channel3_IRQHandler
    .weak DMA1_Channel4_IRQHandler
    .weak DMA1_Channel5_IRQHandler
    .weak DMA1_Channel6_IRQHandler
    .weak DMA1_Channel7_IRQHandler
    .weak ADC1_2_IRQHandler
    .weak USB_HP_CAN1_TX_IRQHandler
    .weak USB_LP_CAN1_RX0_IRQHandler
    .weak CAN1_RX1_IRQHandler
    .weak CAN1_SCE_IRQHandler
    .weak EXTI9_5_IRQHandler
    .weak TIM1_BRK_IRQHandler
    .weak TIM1_UP_IRQHandler
    .weak TIM1_TRG_COM_IRQHandler
    .weak TIM1_CC_IRQHandler
    .weak TIM2_IRQHandler
    .weak TIM3_IRQHandler
    .weak TIM4_IRQHandler
    .weak I2C1_EV_IRQHandler
    .weak I2C1_ER_IRQHandler
    .weak I2C2_EV_IRQHandler
    .weak I2C2_ER_IRQHandler
    .weak SPI1_IRQHandler
    .weak SPI2_IRQHandler
    .weak USART1_IRQHandler
    .weak USART2_IRQHandler
    .weak USART3_IRQHandler
    .weak EXTI15_10_IRQHandler
    .weak RTCAlarm_IRQHandler
    .weak USBWakeUp_IRQHandler
    .weak TIM8_BRK_IRQHandler
    .weak TIM8_UP_IRQHandler
    .weak TIM8_TRG_COM_IRQHandler
    .weak TIM8_CC_IRQHandler
    .weak RNG_IRQHandler
    .weak FSMC_IRQHandler
    .weak SDIO_IRQHandler
    .weak TIM5_IRQHandler
    .weak SPI3_IRQHandler
    .weak UART4_IRQHandler
    .weak UART5_IRQHandler
    .weak TIM6_IRQHandler
    .weak TIM7_IRQHandler
    .weak DMA2_Channel1_IRQHandler
    .weak DMA2_Channel2_IRQHandler
    .weak DMA2_Channel3_IRQHandler
    .weak DMA2_Channel4_IRQHandler
    .weak DMA2_Channel5_IRQHandler
    .weak ETH_IRQHandler
    .weak ETH_WKUP_IRQHandler
    .weak CAN2_TX_IRQHandler
    .weak CAN2_RX0_IRQHandler
    .weak CAN2_RX1_IRQHandler
    .weak CAN2_SCE_IRQHandler
    .weak OTG_FS_IRQHandler
    .weak USBHSWakeup_IRQHandler
    .weak USBHS_IRQHandler
    .weak DVP_IRQHandler
    .weak UART6_IRQHandler
    .weak UART7_IRQHandler
    .weak UART8_IRQHandler
    .weak TIM9_BRK_IRQHandler
    .weak TIM9_UP_IRQHandler
    .weak TIM9_TRG_COM_IRQHandler
    .weak TIM9_CC_IRQHandler
    .weak TIM10_BRK_IRQHandler
    .weak TIM10_UP_IRQHandler
    .weak TIM10_TRG_COM_IRQHandler
    .weak TIM10_CC_IRQHandler
    .weak DMA2_Channel6_IRQHandler
    .weak DMA2_Channel7_IRQHandler
    .weak DMA2_Channel8_IRQHandler
    .weak DMA2_Channel9_IRQHandler
    .weak DMA2_Channel10_IRQHandler
    .weak DMA2_Channel11_IRQHandler

NMI_Handler: 1: j 1b
HardFault_Handler: 1: j 1b
Ecall_M_Mode_Handler: 1: j 1b
Ecall_U_Mode_Handler: 1: j 1b
Break_Point_Handler: 1: j 1b
SysTick_Handler: 1: j 1b
SW_Handler: 1: j 1b
WWDG_IRQHandler: 1: j 1b
PVD_IRQHandler: 1: j 1b
TAMPER_IRQHandler: 1: j 1b
RTC_IRQHandler: 1: j 1b
FLASH_IRQHandler: 1: j 1b
RCC_IRQHandler: 1: j 1b
EXTI0_IRQHandler: 1: j 1b
EXTI1_IRQHandler: 1: j 1b
EXTI2_IRQHandler: 1: j 1b
EXTI3_IRQHandler: 1: j 1b
EXTI4_IRQHandler: 1: j 1b
DMA1_Channel1_IRQHandler: 1: j 1b
DMA1_Channel2_IRQHandler: 1: j 1b
DMA1_Channel3_IRQHandler: 1: j 1b
DMA1_Channel4_IRQHandler: 1: j 1b
DMA1_Channel5_IRQHandler: 1: j 1b
DMA1_Channel6_IRQHandler: 1: j 1b
DMA1_Channel7_IRQHandler: 1: j 1b
ADC1_2_IRQHandler: 1: j 1b
USB_HP_CAN1_TX_IRQHandler: 1: j 1b
USB_LP_CAN1_RX0_IRQHandler: 1: j 1b
CAN1_RX1_IRQHandler: 1: j 1b
CAN1_SCE_IRQHandler: 1: j 1b
EXTI9_5_IRQHandler: 1: j 1b
TIM1_BRK_IRQHandler: 1: j 1b
TIM1_UP_IRQHandler: 1: j 1b
TIM1_TRG_COM_IRQHandler: 1: j 1b
TIM1_CC_IRQHandler: 1: j 1b
TIM2_IRQHandler: 1: j 1b
TIM3_IRQHandler: 1: j 1b
TIM4_IRQHandler: 1: j 1b
I2C1_EV_IRQHandler: 1: j 1b
I2C1_ER_IRQHandler: 1: j 1b
I2C2_EV_IRQHandler: 1: j 1b
I2C2_ER_IRQHandler: 1: j 1b
SPI1_IRQHandler: 1: j 1b
SPI2_IRQHandler: 1: j 1b
USART1_IRQHandler: 1: j 1b
USART2_IRQHandler: 1: j 1b
USART3_IRQHandler: 1: j 1b
EXTI15_10_IRQHandler: 1: j 1b
RTCAlarm_IRQHandler: 1: j 1b
USBWakeUp_IRQHandler: 1: j 1b
TIM8_BRK_IRQHandler: 1: j 1b
TIM8_UP_IRQHandler: 1: j 1b
TIM8_TRG_COM_IRQHandler: 1: j 1b
TIM8_CC_IRQHandler: 1: j 1b
RNG_IRQHandler: 1: j 1b
FSMC_IRQHandler: 1: j 1b
SDIO_IRQHandler: 1: j 1b
TIM5_IRQHandler: 1: j 1b
SPI3_IRQHandler: 1: j 1b
UART4_IRQHandler: 1: j 1b
UART5_IRQHandler: 1: j 1b
TIM6_IRQHandler: 1: j 1b
TIM7_IRQHandler: 1: j 1b
DMA2_Channel1_IRQHandler: 1: j 1b
DMA2_Channel2_IRQHandler: 1: j 1b
DMA2_Channel3_IRQHandler: 1: j 1b
DMA2_Channel4_IRQHandler: 1: j 1b
DMA2_Channel5_IRQHandler: 1: j 1b
ETH_IRQHandler: 1: j 1b
ETH_WKUP_IRQHandler: 1: j 1b
CAN2_TX_IRQHandler: 1: j 1b
CAN2_RX0_IRQHandler: 1: j 1b
CAN2_RX1_IRQHandler: 1: j 1b
CAN2_SCE_IRQHandler: 1: j 1b
OTG_FS_IRQHandler: 1: j 1b
USBHSWakeup_IRQHandler: 1: j 1b
USBHS_IRQHandler: 1: j 1b
DVP_IRQHandler: 1: j 1b
UART6_IRQHandler: 1: j 1b
UART7_IRQHandler: 1: j 1b
UART8_IRQHandler: 1: j 1b
TIM9_BRK_IRQHandler: 1: j 1b
TIM9_UP_IRQHandler: 1: j 1b
TIM9_TRG_COM_IRQHandler: 1: j 1b
TIM9_CC_IRQHandler: 1: j 1b
TIM10_BRK_IRQHandler: 1: j 1b
TIM10_UP_IRQHandler: 1: j 1b
TIM10_TRG_COM_IRQHandler: 1: j 1b
TIM10_CC_IRQHandler: 1: j 1b
DMA2_Channel6_IRQHandler: 1: j 1b
DMA2_Channel7_IRQHandler: 1: j 1b
DMA2_Channel8_IRQHandler: 1: j 1b
DMA2_Channel9_IRQHandler: 1: j 1b
DMA2_Channel10_IRQHandler: 1: j 1b
DMA2_Channel11_IRQHandler: 1: j 1b


 .section .text.handle_reset,"ax",@progbits
 .weak handle_reset
 .align 1
handle_reset:
.option push
.option norelax
 la gp, __global_pointer$
.option pop
1:
 la sp, _eusrstack
2:

 la a0, _data_lma
 la a1, _data_vma
 la a2, _edata
 bgeu a1, a2, 2f
1:
 lw t0, (a0)
 sw t0, (a1)
 addi a0, a0, 4
 addi a1, a1, 4
 bltu a1, a2, 1b
2:

 la a0, _sbss
 la a1, _ebss
 bgeu a0, a1, 2f
1:
 sw zero, (a0)
 addi a0, a0, 4
 bltu a0, a1, 1b
2:
    li t0, 0x1f
    csrw 0xbc0, t0


 li t0, 0x1f
 csrw 0x804, t0


    li t0, 0x6088
    csrs mstatus, t0

  la t0, _vector_base
    ori t0, t0, 3
 csrw mtvec, t0

 lui a0, 0x1ffff
    li a1, 0x300
    sh a1, 0x1b0(a0)
1: lui s2, 0x40022
    lw a0, 0xc(s2)
    andi a0, a0, 1
    bnez a0, 1b

    jal SystemInit
 la t0, main
 csrw mepc, t0
 mret
