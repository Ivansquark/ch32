extern "C" {

void Reset_Handler();
void Default_Handler();
void _start();
int main();

void NMI_Handler() __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__((weak, alias("Default_Handler")));
void Ecall_M_Mode_Handler() __attribute__((weak, alias("Default_Handler")));
void Ecall_U_Mode_Handler() __attribute__((weak, alias("Default_Handler")));
void Break_Point_Handler() __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__((weak, alias("Default_Handler")));
void SW_Handler() __attribute__((weak, alias("Default_Handler")));
void WWDG_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TAMPER_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler()
    __attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler()
    __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RTCAlarm_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void FSMC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM6_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART6_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART7_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART8_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM9_BRK_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM9_UP_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM9_TRG_COM_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM9_CC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM10_BRK_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM10_UP_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM10_TRG_COM_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM10_CC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel6_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel7_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel8_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel9_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel10_IRQHandler()
    __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel11_IRQHandler()
    __attribute__((weak, alias("Default_Handler")));

// set first instruction: jump to Reset_Handler (naked - no push in stack)
void __attribute__((naked, noreturn)) __attribute__((section(".init")))
_start() {
    asm("j	Reset_Handler");
}

void (*vectors[])() __attribute__((section(".vector"))) = {
    _start,                  /*first instruction*/
    0,                       /* zero */
    NMI_Handler,             /* NMI */
    HardFault_Handler,       /* Hard Fault */
    0,                       /* zero */
    Ecall_M_Mode_Handler,    /* Ecall M Mode */
    0,                       /* zero */
    0, Ecall_U_Mode_Handler, /* Ecall U Mode */
    Break_Point_Handler,     /* Break Point */
    0,                       /* zero */
    0, SysTick_Handler,      /* SysTick */
    0, SW_Handler,           /* SW */
    0,                       /* zero */
    /* External Interrupts */
    WWDG_IRQHandler,            /* Window Watchdog */
    PVD_IRQHandler,             /* PVD through EXTI Line detect */
    TAMPER_IRQHandler,          /* TAMPER */
    RTC_IRQHandler,             /* RTC */
    FLASH_IRQHandler,           /* Flash */
    RCC_IRQHandler,             /* RCC */
    EXTI0_IRQHandler,           /* EXTI Line 0 */
    EXTI1_IRQHandler,           /* EXTI Line 1 */
    EXTI2_IRQHandler,           /* EXTI Line 2 */
    EXTI3_IRQHandler,           /* EXTI Line 3 */
    EXTI4_IRQHandler,           /* EXTI Line 4 */
    DMA1_Channel1_IRQHandler,   /* DMA1 Channel 1 */
    DMA1_Channel2_IRQHandler,   /* DMA1 Channel 2 */
    DMA1_Channel3_IRQHandler,   /* DMA1 Channel 3 */
    DMA1_Channel4_IRQHandler,   /* DMA1 Channel 4 */
    DMA1_Channel5_IRQHandler,   /* DMA1 Channel 5 */
    DMA1_Channel6_IRQHandler,   /* DMA1 Channel 6 */
    DMA1_Channel7_IRQHandler,   /* DMA1 Channel 7 */
    ADC1_2_IRQHandler,          /* ADC1_2 */
    USB_HP_CAN1_TX_IRQHandler,  /* USB HP and CAN1 TX */
    USB_LP_CAN1_RX0_IRQHandler, /* USB LP and CAN1RX0 */
    CAN1_RX1_IRQHandler,        /* CAN1 RX1 */
    CAN1_SCE_IRQHandler,        /* CAN1 SCE */
    EXTI9_5_IRQHandler,         /* EXTI Line 9..5 */
    TIM1_BRK_IRQHandler,        /* TIM1 Break */
    TIM1_UP_IRQHandler,         /* TIM1 Update */
    TIM1_TRG_COM_IRQHandler,    /* TIM1 Trigger and Commutation */
    TIM1_CC_IRQHandler,         /* TIM1 Capture Compare */
    TIM2_IRQHandler,            /* TIM2 */
    TIM3_IRQHandler,            /* TIM3 */
    TIM4_IRQHandler,            /* TIM4 */
    I2C1_EV_IRQHandler,         /* I2C1 Event */
    I2C1_ER_IRQHandler,         /* I2C1 Error */
    I2C2_EV_IRQHandler,         /* I2C2 Event */
    I2C2_ER_IRQHandler,         /* I2C2 Error */
    SPI1_IRQHandler,            /* SPI1 */
    SPI2_IRQHandler,            /* SPI2 */
    USART1_IRQHandler,          /* USART1 */
    USART2_IRQHandler,          /* USART2 */
    USART3_IRQHandler,          /* USART3 */
    EXTI15_10_IRQHandler,       /* EXTI Line 15..10 */
    RTCAlarm_IRQHandler,        /* RTC Alarm through EXTI Line */
    0,                          /* zero */
    TIM8_BRK_IRQHandler,        /* TIM8 Break */
    TIM8_UP_IRQHandler,         /* TIM8 Update */
    TIM8_TRG_COM_IRQHandler,    /* TIM8 Trigger and Commutation */
    TIM8_CC_IRQHandler,         /* TIM8 Capture Compare */
    RNG_IRQHandler,             /* RNG */
    FSMC_IRQHandler,            /* FSMC */
    SDIO_IRQHandler,            /* SDIO */
    TIM5_IRQHandler,            /* TIM5 */
    SPI3_IRQHandler,            /* SPI3 */
    UART4_IRQHandler,           /* UART4 */
    UART5_IRQHandler,           /* UART5 */
    TIM6_IRQHandler,            /* TIM6 */
    TIM7_IRQHandler,            /* TIM7 */
    DMA2_Channel1_IRQHandler,   /* DMA2 Channel 1 */
    DMA2_Channel2_IRQHandler,   /* DMA2 Channel 2 */
    DMA2_Channel3_IRQHandler,   /* DMA2 Channel 3 */
    DMA2_Channel4_IRQHandler,   /* DMA2 Channel 4 */
    DMA2_Channel5_IRQHandler,   /* DMA2 Channel 5 */
    0,                          /* zero */
    0,                          /* zero */
    0,                          /* zero */
    0,                          /* zero */
    0,                          /* zero */
    0,                          /* zero */
    OTG_FS_IRQHandler,          /* OTGFS */
    0,                          /* zero */
    0,                          /* zero */
    0,                          /* zero */
    UART6_IRQHandler,           /* UART6 */
    UART7_IRQHandler,           /* UART7 */
    UART8_IRQHandler,           /* UART8 */
    TIM9_BRK_IRQHandler,        /* TIM9 Break */
    TIM9_UP_IRQHandler,         /* TIM9 Update */
    TIM9_TRG_COM_IRQHandler,    /* TIM9 Trigger and Commutation */
    TIM9_CC_IRQHandler,         /* TIM9 Capture Compare */
    TIM10_BRK_IRQHandler,       /* TIM10 Break */
    TIM10_UP_IRQHandler,        /* TIM10 Update */
    TIM10_TRG_COM_IRQHandler,   /* TIM10 Trigger and Commutation */
    TIM10_CC_IRQHandler,        /* TIM10 Capture Compare */
    DMA2_Channel6_IRQHandler,   /* DMA2 Channel 6 */
    DMA2_Channel7_IRQHandler,   /* DMA2 Channel 7 */
    DMA2_Channel8_IRQHandler,   /* DMA2 Channel 8 */
    DMA2_Channel9_IRQHandler,   /* DMA2 Channel 9 */
    DMA2_Channel10_IRQHandler,  /* DMA2 Channel 10 */
    DMA2_Channel11_IRQHandler   /* DMA2 Channel 11 */
};
// from linker
extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
// from linker constructors
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
// from linker destructors
extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));
// dummy This section holds executable instructions that contribute to the
// process initialization code. When a program starts to run, the system
// arranges to execute the code in this section before calling the main
// program entry point (called main for C programs).
void __attribute__((weak)) _init(void) {}
// dummy заглушка This section holds executable instructions that contribute
// to the process termination code. That is, when a program exits normally,
// the system arranges to execute the code in this section
void __attribute__((weak)) _fini(void) {}

/* Iterate over all the init routines.  */
void __libc_init_array(void) {
    // static initialization constructors function
    int count;
    int i;
    // counts of preinit functions DK what it is
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++) __preinit_array_start[i]();
    _init();
    // counts of init constructors
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++) __init_array_start[i]();
}
/* Run all the cleanup routines.  */
void __libc_fini_array(void) {
    //!< destructors not usefull in microcontrollers
    int count;
    int i;
    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--) { __fini_array_start[i - 1](); }
    _fini();
}

extern void *_data_lma, *_data_vma, *_edata, *_sbss, *_ebss; // from .ld
extern void *__global_pointer, *_eusrstack;                  // from .ld

void __attribute__((naked, noreturn)) Reset_Handler() {
    // set ??? (global pointer to linux)
    //asm("la gp, __global_pointer$");
    // set stack
    asm("la sp, _eusrstack");
    void** pSource;
    void** pDest;
    // copy data section from flash to RAM
    pSource = &_data_lma, pDest = &_data_vma;
    while(pDest < &_edata) {
        *pDest = *pSource;
        pSource++; pDest++;
    }
    //for (pSource = &_data_lma, pDest = &_data_vma; pDest != &_edata;
    //     pSource++, pDest++) {
    //    if(pDest > &_edata) {
    //        break;
    //    }
    //    *pDest = *pSource;
    //}
    // clear bss section
    for (pDest = &_sbss; pDest != &_ebss; pDest++) { *pDest = 0; }

    /* Enable nested and hardware stack */
    asm("li t0, 0x0b");
    asm("csrw 0x804, t0");


    // set start address of vector table
    asm("la t0, vectors");
    asm("ori t0, t0, 3"); // add to low bits
    asm("csrw mtvec, t0");

    __libc_init_array();
    
    /* Enable floating point and interrupt */
    asm("li t0, 0x6088");
    asm("csrw mstatus, t0");

    // asm("jal SystemInit");
    asm("la t0, main");
    asm("csrw mepc, t0"); // set prog counter
    asm("mret");

    // main();
    while (1) {}
}
extern "C" void Default_Handler() {
    while (1) {}
}
}
