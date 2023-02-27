#include "ch32v30x.h"

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)
#define FLASH_START 0x08000000
#define FLASH_BOOT_FLAGS_ADDR 0x08000800
#define FLASH_IP_ADDR 0x08000900
#define FLASH_BANK1_FIRMWARE ((volatile uint32_t)0x08000A00)
#define FLASH_BANK2_FIRMWARE 0x08020000
#define PAGE_SIZE 0x100
#define BOOT_FLAG 0x01

extern "C" {

__attribute__((naked, noreturn)) __attribute__((section(".init_boot"))) void
_start_boot();
__attribute__((naked, noreturn)) __attribute__((section(".init_boot"))) void
ResetBoot_Handler();
__attribute__((section(".init_boot"))) void main_boot();

// set first instruction: jump to Reset_Handler (naked - no push in stack)
void _start_boot() { asm("j	ResetBoot_Handler"); }

// extern void (*__preinit_array_start[])(void) __attribute__((weak));
// extern void (*__preinit_array_end[])(void) __attribute__((weak));
// from linker constructors
// extern void (*__init_array_start[])(void) __attribute__((weak));
// extern void (*__init_array_end[])(void) __attribute__((weak));
// from linker destructors
// extern void (*__fini_array_start[])(void) __attribute__((weak));
// extern void (*__fini_array_end[])(void) __attribute__((weak));
// dummy This section holds executable instructions that contribute to the
// process initialization code. When a program starts to run, the system
// arranges to execute the code in this section before calling the main
// program entry point (called main for C programs).
// void __attribute__((weak)) _init(void) {}
// dummy заглушка This section holds executable instructions that contribute
// to the process termination code. That is, when a program exits normally,
// the system arranges to execute the code in this section
// void __attribute__((weak)) _fini(void) {}

/* Iterate over all the init routines.  */
/*
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
*/
/* Run all the cleanup routines.  */
/*
void __libc_fini_array(void) {
    //!< destructors not usefull in microcontrollers
    int count;
    int i;
    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--) { __fini_array_start[i - 1](); }
    _fini();
}
*/
// from .ld
extern void *_data_start_flash, *_data_start_ram, *_edata, *_sbss, *_ebss;
extern void *__global_pointer, *_eusrstack;

// naked cause we have not proper stack pointer yet
void ResetBoot_Handler() {
    // set ??? (global pointer to linux) [reduces firmware size]
    __asm volatile("la gp, __global_pointer");
    // set stack
    __asm volatile("la sp, _eusrstack");
    // void** pSource;
    // void** pDest;
    // copy data section from flash to RAM
    // for (pSource = &_data_start_flash, pDest = &_data_start_ram;
    //     pDest != &_edata; pSource++, pDest++) {
    //    if (pDest > &_edata) { break; }
    //    *pDest = *pSource;
    //}
    // clear bss section
    // for (pDest = &_sbss; pDest != &_ebss; pDest++) { *pDest = 0; }

    /* Pipeline and instruction prediction */
    //__asm volatile("li t0, 0x1f");
    //__asm volatile("csrw 0xbc0, t0");
    __asm volatile("csrwi 0xbc0, 0x1f");

    /* Enable nested and hardware stack */
    // asm("li t0, 0x1f");
    // asm("csrw 0x804, t0");
    __asm volatile("csrwi 0x804, 0x1f");

    /* Enable floating point and machine mode*/
    /* 0x800 mapped on mstatus*/
    /* gdb: i r mstatus => FSS=3 - floating point enable MPP=3 - machine mode*/
    // asm("li t0, 0x7800");
    // asm("csrw mstatus, t0");
    __asm volatile("li t0, 0x7800");
    __asm volatile("csrw 0x800, t0");

    // enable interrupt
    //__asm volatile ("csrw 0x800, %0" : : "r" (0x6088) );
    //__asm volatile ("csrsi 0x800, 0x8" );
    // disable interrupt
    //__asm volatile ("csrw 0x800, %0" : : "r" (0x6000) );
    __asm volatile("csrci 0x800, 0x8");

    // set start address of vector table
    //__asm volatile("la t0, vectors");
    //__asm volatile("ori t0, t0, 3"); // add two low bits
    //__asm volatile("csrw mtvec, t0");

    //__libc_init_array();

    // asm("jal SystemInit");
    __asm volatile("la t0, main_boot");
    __asm volatile("csrw mepc, t0"); // set exeption prog counter
    __asm volatile("mret");
    // main_boot();

    // main();
    // while (1) {}
}
// void Default_Handler() {
//    while (1) {}
//}

//__attribute__((interrupt))
// void HardFault_Handler(void) {
//    while (1) {}
//}

void main_boot(void) {
    uint32_t bootFlag = (*(uint32_t*)(FLASH_BOOT_FLAGS_ADDR));
    uint8_t buf[256] = {0};
    if (bootFlag == BOOT_FLAG) {
        // erase boot flag
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
        FLASH->MODEKEYR = FLASH_KEY1;
        FLASH->MODEKEYR = FLASH_KEY2;
        // erase page boot flag
        // no flash memory operation is ongoing (BSY -busy)
        while (FLASH->STATR & FLASH_STATR_BSY) {}
        // reset bit end off operation
        if (FLASH->STATR & FLASH_STATR_EOP) { FLASH->STATR = FLASH_STATR_EOP; }
        // single page erase chosen
        FLASH->CTLR |= FLASH_CTLR_PAGE_ER;
        // choose page address
        FLASH->ADDR |= FLASH_BOOT_FLAGS_ADDR;
        // triggers an ER/PR operation when set.
        FLASH->CTLR |= FLASH_CTLR_STRT;
        while (FLASH->STATR & FLASH_STATR_BSY) {}
        // while not end of operation FLASH->SR = FLASH_SR_EOP;   // reset bit
        FLASH->CTLR &= ~FLASH_CTLR_PAGE_ER;
        FLASH->CTLR |= FLASH_CTLR_LOCK;

        // unlock
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
        FLASH->MODEKEYR = FLASH_KEY1;
        FLASH->MODEKEYR = FLASH_KEY2;

        // start flash new firmware
        // flash.WritePagesFromBank2ToBank1();
        for (volatile int i = 0; i < 500; i++) {
            FLASH->KEYR = FLASH_KEY1;
            FLASH->KEYR = FLASH_KEY2;
            FLASH->MODEKEYR = FLASH_KEY1;
            FLASH->MODEKEYR = FLASH_KEY2;
            // erase page boot flag
            // no flash memory operation is ongoing (BSY -busy)
            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // reset bit end off operation
            if (FLASH->STATR & FLASH_STATR_EOP) {
                FLASH->STATR = FLASH_STATR_EOP;
            }
            // single page erase chosen
            FLASH->CTLR |= FLASH_CTLR_PAGE_ER;
            // choose page address
            // FLASH->ADDR |= 0x08000A00;
            FLASH->ADDR |= FLASH_BANK1_FIRMWARE;
            // triggers an ER/PR operation when set.
            FLASH->CTLR |= FLASH_CTLR_STRT;
            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // while not end of operation FLASH->SR = FLASH_SR_EOP;   // reset
            // bit
            FLASH->CTLR &= ~FLASH_CTLR_PAGE_ER;
            FLASH->CTLR |= FLASH_CTLR_LOCK;

            // programm
            // unlock
            FLASH->KEYR = FLASH_KEY1;
            FLASH->KEYR = FLASH_KEY2;
            FLASH->MODEKEYR = FLASH_KEY1;
            FLASH->MODEKEYR = FLASH_KEY2;

            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // reset bit end off operation
            if (FLASH->STATR & FLASH_STATR_EOP) {
                FLASH->STATR = FLASH_STATR_EOP;
            }
            // choose page address
            FLASH->ADDR |= FLASH_BANK1_FIRMWARE;
            // single page program chosen
            FLASH->CTLR |= FLASH_CTLR_PAGE_PG;
            //!< 1 byte writing>
            for (volatile int i = 0; i < PAGE_SIZE / 4; i++) {
                *((uint32_t*)(FLASH_BANK1_FIRMWARE) + i) =
                    *((uint32_t*)(FLASH_BANK2_FIRMWARE) + i);
                while (FLASH->STATR & FLASH_STATR_WR_BSY) {}
            }
            FLASH->CTLR |= FLASH_CTLR_PG_STRT;
            while (FLASH->STATR & FLASH_STATR_BSY) {}
            // finish programming
            FLASH->CTLR &= ~(FLASH_CTLR_PAGE_PG);
            FLASH->CTLR |= FLASH_CTLR_LOCK;
        }

        // //
        //___________________ Software RESET __________________________________
        NVIC->CFGR = NVIC_KEY3 | (1 << 7);
    } else {
        // goto firmware
        // first deinit all
        __asm volatile("csrci 0x800, 0x8");
        //! ___	here proceed transition to new programm _______________________
        volatile uint32_t mainAppAddr = FLASH_BANK1_FIRMWARE - 0x08000000;
        // uint32_t mainAppStack = *((uint32_t*)(long)mainAppAddr);
        void (*mainApplication)() = (void (*)())(long)(mainAppAddr);
        mainApplication(); // go to start main programm
        //
        //__asm volatile("li t0, 0xA00");
        //__asm volatile("csrw mepc, t0"); // set exeption prog counter
        //__asm volatile("mret");
        /*!< in linker main programm need to set PROGRAMM START ADDRESS to
         * MAIN_PROGRAMM_ADDRESS (0x08020000 - sector 1)>*/
    }
    while (1) {}
}

} // extern "C"
