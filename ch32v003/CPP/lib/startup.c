void Reset_Handler();
void Default_Handler();
void Temp_Handler();
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
void FLASH_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void EXTI7_0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void AWU_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ADC1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler() __attribute__((weak, alias("Temp_Handler")));
void USART1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler() __attribute__((weak, alias("Temp_Handler")));
void TIM1_BRK_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler() __attribute__((weak, alias("Default_Handler")));

// set first instruction: jump to Reset_Handler (naked - no push in stack)
//__attribute__((aligned(4)))
// void __attribute__((naked, noreturn)) __attribute__((section(".init")))
// _start()
void __attribute__((naked)) __attribute__((section(".init"))) _start()
{
    // asm(".option   norvc;");
    asm("j	Reset_Handler");
}
//__attribute__((aligned(8)))
void (*vectors[])() __attribute__((section(".vector"))) = {
    //_start,            /*first instruction*/
    // 0,                 /* zero */
    NMI_Handler,       /* NMI */
    HardFault_Handler, /* Hard Fault */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    0,                 /* zero */
    SysTick_Handler,   /* SysTick */
    0,                 /* zero */
    SW_Handler,        /* SW */
    0,                 /* zero */
    /* External Interrupts */
    WWDG_IRQHandler,          /* Window Watchdog */
    PVD_IRQHandler,           /* PVD through EXTI Line detect */
    FLASH_IRQHandler,         /* Flash */
    RCC_IRQHandler,           /* RCC */
    EXTI7_0_IRQHandler,       /* EXTI Line 7..0 */
    AWU_IRQHandler,           /* AWU */
    DMA1_Channel1_IRQHandler, /* DMA1 Channel 1 */
    DMA1_Channel2_IRQHandler, /* DMA1 Channel 2 */
    DMA1_Channel3_IRQHandler, /* DMA1 Channel 3 */
    DMA1_Channel4_IRQHandler, /* DMA1 Channel 4 */
    DMA1_Channel5_IRQHandler, /* DMA1 Channel 5 */
    DMA1_Channel6_IRQHandler, /* DMA1 Channel 6 */
    DMA1_Channel7_IRQHandler, /* DMA1 Channel 7 */
    ADC1_IRQHandler,          /* ADC1 */
    I2C1_EV_IRQHandler,       /* I2C1 Event */
    I2C1_ER_IRQHandler,       /* I2C1 Error */
    USART1_IRQHandler,        /* USART1 */
    SPI1_IRQHandler,          /* SPI1 */
    TIM1_BRK_IRQHandler,      /* TIM1 Break */
    TIM1_UP_IRQHandler,       /* TIM1 Update */
    TIM1_TRG_COM_IRQHandler,  /* TIM1 Trigger and Commutation */
    TIM1_CC_IRQHandler,       /* TIM1 Capture Compare */
    TIM2_IRQHandler,          /* TIM2 */
};

// from .ld
extern void *_data_start_flash, *_data_start_ram, *_edata, *_sbss, *_ebss;
extern void *__global_pointer, *_eusrstack;

// naked cause we have not proper stack pointer yet
void __attribute__((naked, noreturn)) Reset_Handler()
{
    // duumy instr to compilere to create vect table
    __asm volatile("la t0, vectors");
    // set ??? (global pointer to linux) [reduces firmware size]
    __asm volatile("la gp, __global_pointer");
    // set stack
    __asm volatile("la sp, _eusrstack");
    void** pSource;
    void** pDest;
    // copy data section from flash to RAM
    for (pSource = &_data_start_flash, pDest = &_data_start_ram;
         pDest != &_edata; pSource++, pDest++) {
        if (pDest > &_edata) {
            while (1) {}
        }
        *pDest = *pSource;
    }
    // clear bss section
    for (pDest = &_sbss; pDest != &_ebss; pDest++) {
        *pDest = 0;
    }

    /* Enable nested and hardware stack */
    // asm("li t0, 0x1f");
    // asm("csrw 0x804, t0");
    __asm volatile("csrwi 0x804, 0x3");

    /* Enable floating point and machine mode*/
    /* 0x800 mapped on mstatus*/
    /* gdb: i r mstatus => MPP=3 - machine mode*/
    __asm volatile("li t0, 0x88");
    __asm volatile("csrw 0x800, t0");

    // enable interrupt
    //__asm volatile ("csrw 0x800, %0" : : "r" (0x6088) );
    __asm volatile("csrsi 0x800, 0x8");
    // disable interrupt
    //__asm volatile ("csrw 0x800, %0" : : "r" (0x6000) );
    //__asm volatile("csrci mstatus, 0x8");

    // set start address of vector table
    // mtvec[1:0] -
    // [0]=0 -non vectored format (one handler to all address in mcause)
    // [0]=1 - vectored format (check [1] bit)
    // [1]=0 - is an instruction to jump to the exception or interrupt function,
    // or it can be another instruction - ?????????????????
    // [1]=1 -  the absolute addresses handlers is stored at the vector table
    __asm volatile("la t0, _start");
    __asm volatile("ori t0, t0, 3"); // [1:0] = 1:1 absolute vectored
    __asm volatile("csrw mtvec, t0");

    // asm("jal SystemInit");
    __asm volatile("la t0, main");
    __asm volatile("csrw mepc, t0"); // set exeption prog counter
    __asm volatile("mret");

    // main();
    while (1) {}
}
void Default_Handler()
{
    while (1) {}
}
void Temp_Handler()
{
    while (1) {}
}
