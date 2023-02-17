extern "C" {

__attribute__((naked, noreturn)) __attribute__((section(".init_boot"))) void
_start_boot();
__attribute__((naked, noreturn)) __attribute__((section(".init_boot"))) void
ResetBoot_Handler();
// void Default_Handler();
__attribute__((section(".init_boot"))) int main_boot();

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
}
