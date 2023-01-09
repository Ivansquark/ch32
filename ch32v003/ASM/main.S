.include "defs.S"

.text
.global main

main:
    la t0, RCC_APB2PCENR
        lw t1, 0(t0)        
        ori t1, t1, RCC_IOPAEN
    sw t1, 0(t0)
 
    la t0, GPIOA_CFGR
        lw t1, 0(t0)
        li t2, 0x30
        slli t2, t2, 24
        or t1, t1, t2
    sw t1, 0(t0)

    la t0, GPIOA_CFGR
        lw t1, 0(t0)
        li t2, GPIO_CFGHR_CNF15
        #li t2, 0xC0
        #        slli t2, t2, 24
        li t3, 0xFFFFFFFF
        xor t2, t2, t3 # invert
        and t1, t1, t2
    sw t1, 0(t0)
    

    la t0, GPIOA_BSHR
main_loop:
        lw t1, 0(t0)
        li t2, 0x80
        slli t2, t2, 8
        or t1, t1, t2;
    sw t1, 0(t0)

    li t2, 0xFFFF
loop1:
    addi t2, t2, -1
    bnez t2, loop1
    
        lw t1, 0(t0)
        li t2, 0x80
        slli t2, t2, 24
        or t1, t1, t2;
    sw t1, 0(t0)

    li t2, 0xFFFF
loop2:
    addi t2, t2, -1
    bnez t2, loop2

    j main_loop

