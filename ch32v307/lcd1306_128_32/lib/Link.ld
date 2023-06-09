ENTRY( _start )
__stack_size = 0x400;   /*256*/
_Min_Heap_Size = 0x100; /*256*/
PROVIDE( _stack_size = __stack_size );

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
FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 192K
RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .init :
    {
        _sinit = .;
        . = ALIGN(4);
        KEEP(*(SORT_NONE(.init)))
        . = ALIGN(4);
        _einit = .;
    } >FLASH AT>FLASH
  
    .vector :
    {
        *(.vector);
        . = ALIGN(64);
    } >FLASH AT>FLASH
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.t.*)
        . = ALIGN(4);
    } >FLASH AT>FLASH 
    
    .fini :
    {
        KEEP(*(SORT_NONE(.fini)))
        . = ALIGN(4);
    } >FLASH AT>FLASH
    PROVIDE( _etext = . );
    PROVIDE( _eitcm = . );	

    .preinit_array  :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH AT>FLASH 

    .init_array     :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH AT>FLASH 
    
    .fini_array     :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH AT>FLASH 	

    .ctors          :
    {
        /* gcc uses crtbegin.o to find the start of
         * the constructors, so we make sure it is
         * first.  Because this is a wildcard, it
         * doesn't matter if the user does not
         * actually link against crtbegin.o; the
         * linker won't look for a file to match a
         * wildcard.  The wildcard also means that it
         * doesn't matter which directory crtbegin.o
         * is in.  */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*crtbegin?.o(.ctors))
        /* We don't want to include the .ctor section from
         * the crtend.o file until after the sorted ctors.
         * The .ctor section from the crtend file contains the
         * end of ctors marker and it must be last */
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } >FLASH AT>FLASH 
    
    .dtors          :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*crtbegin?.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } >FLASH AT>FLASH  
    
    /* Used by the startup to initialize data */
    _data_start_flash = LOADADDR(.data);

    /* begining of RAM 0x20000000 */
    .stack : /*ORIGIN(RAM) + LENGTH(RAM) - __stack_size :*/
    {            
        . = ALIGN(4);
        PROVIDE(_susrstack = . ); /*stack overflow when below 0x20000000*/
        . = . + __stack_size;
        PROVIDE( _eusrstack = .); /*stack start 0x20000800*/
        PROVIDE(__freertos_irq_stack_top = .); /*FreeRTOS stack IRQ*/
    } >RAM
    
    .data :
    {
        . = ALIGN(4);
        PROVIDE(_data_start_ram = .);
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        . = ALIGN(8);
        PROVIDE( __global_pointer = . + 0x800 );
        *(.sdata .sdata.*)
        *(.sdata2.*)
        *(.gnu.linkonce.s.*)
        . = ALIGN(8);
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)
        . = ALIGN(4);
        PROVIDE( _edata = .);
    } >RAM AT>FLASH

    .bss :
    {
        . = ALIGN(4);
        PROVIDE( _sbss = .);
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss*)
        *(.gnu.linkonce.b.*)
        *(COMMON*)
        . = ALIGN(4);
        PROVIDE( _ebss = .);
    } >RAM AT>FLASH

    PROVIDE( _end = _ebss);
    PROVIDE( end = . );
    
    /* heap, if its needed */
    .heap :
	{
    . = ALIGN(4);
    /*_heap_start = __bss_end__;*/
    _heap_start = .;
    /*PROVIDE(current_ends = 0);*/
    PROVIDE( end = . );                /*_sbrk uses end for malloc*/
		_heap_limit = _heap_start + _Min_Heap_Size;
		. = . + _Min_Heap_Size;
    PROVIDE( _heap_end = . );
	} >RAM /*set data in ROM then in RAM in startup file*/
}

