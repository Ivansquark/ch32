ENTRY( _start_boot )

MEMORY
{
    /* CH32V30x_D8C - CH32V305RB-CH32V305FB
     *    CH32V30x_D8 - CH32V303CB-CH32V303RB*/
/*  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 128K
 *  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
 *  */
/* CH32V30x_D8C - CH32V307VC-CH32V307WC-CH32V307RC
 *    CH32V30x_D8 - CH32V303VC-CH32V303RC
 *    FLASH + RAM supports the following configuration
 *    FLASH-192K + RAM-128K
 *    FLASH-224K + RAM-96K
 *    FLASH-256K + RAM-64K
 *    FLASH-288K + RAM-32K  
 *    */
/* CODE-256KB + RAM-64KB
// Kbytes:
// Page 0   0x08000000 - 0x080000FF 256 bytes ...
// Page 7   0x08000700 - 0x080007FF 256 bytes ... bootflasher
// Page 8   0x08000800 - 0x080000FF 256 bytes ... boot flag
// Page 9   0x08000900 - 0x080000FF 256 bytes ... IP + boot flag + align
//
// Page 10   0x08000A00 - 0x08000AFF 256 bytes ... main prog ..

// 128 kB 0x20000
// Page 512   0x08020000 - 0x080200FF 256 bytes ... new prog ..
*/

FLASH_BOOT (rx) : ORIGIN = 0x00000000, LENGTH = 2560
}

SECTIONS
{
   .init_boot :
    {
        . = ALIGN(4);
        KEEP(*(SORT_NONE(.init_boot)))
        . = ALIGN(4);
    } >FLASH_BOOT AT>FLASH_BOOT

    .text_boot :
    {
        . = ALIGN(4);
        *(.text_boot)
        *(.text_boot.*)
        . = ALIGN(4);
    } >FLASH_BOOT AT>FLASH_BOOT
}

