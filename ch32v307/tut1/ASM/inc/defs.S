.equ RCC_APB2PCENR, (0x40000000 + 0x20000 + 0x1000 + 24)
.equ RCC_IOPAEN, 0x00000004
.equ GPIOA_CFGR, 0x40000000 + 0x10000 + 0x0800 + 4
.equ GPIOA_BSHR, 0x40000000 + 0x10000 + 0x0800 + 16
.equ GPIO_CFGHR_MODE15, 0x30000000
.equ GPIO_CFGHR_CNF15, 0xC0000000
.equ GPIO_BSHR_BS15, 0x00008000
.equ GPIO_BSHR_BR15, 0x80000000

.macro push val
  addi sp, sp, -4
  sw \val, 0(sp)
.endm
.macro pop val
  lw \val, 0(sp)
  addi sp, sp, 4
.endm

#Save caller registers
.macro SAVE_CONTEXT
    addi sp, sp, -64
    sw x1,  0(sp)
    sw x5,  4(sp)
    sw x6,  8(sp)
    sw x7,  12(sp)
    sw x10, 16(sp)
    sw x11, 20(sp)
    sw x12, 24(sp)
    sw x13, 28(sp)
    sw x14, 32(sp)
    sw x15, 36(sp)
    sw x16, 40(sp)
    sw x17, 44(sp)
    sw x28, 48(sp)
    sw x29, 52(sp)
    sw x30, 56(sp)
    sw x31, 60(sp)
.endm

#restore caller registers
.macro RESTORE_CONTEXT
    lw x1,  0(sp)
    lw x5,  4(sp)
    lw x6,  8(sp)
    lw x7,  12(sp)
    lw x10, 16(sp)
    lw x11, 20(sp)
    lw x12, 24(sp)
    lw x13, 28(sp)
    lw x14, 32(sp)
    lw x15, 36(sp)
    lw x16, 40(sp)
    lw x17, 44(sp)
    lw x28, 48(sp)
    lw x29, 52(sp)
    lw x30, 56(sp)
    lw x31, 60(sp)
    addi sp, sp, 64
.endm

# Save the mepc and mstatus
#
.macro SAVE_EPC_STATUS
  csrr x5, CSR_MEPC
  push x5
  csrr x5, CSR_MSTATUS
  push x5
  csrr x5, CSR_MSUBM
  push x5
.endm

###############################################
###############################################
# Restore the mepc and mstatus
#
.macro RESTORE_EPC_STATUS
  pop x5
  csrw CSR_MEPC, x5
  pop x5
  csrw CSR_MSTATUS, x5
  pop x5
  csrw CSR_MSUBM, x5
.endm
