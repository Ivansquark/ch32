ENTRY( _start )

__stack_size = 0x2C0;  /**/
_Min_Heap_Size = 0x400; /*1024*/

PROVIDE( _stack_size = __stack_size );

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{
    /* start two position in vector table */
    .init :
    { 
      _sinit = .;
      . = ALIGN(4);
      KEEP(*(SORT_NONE(.init)))
      . = ALIGN(8); /*for vector table second position 0*/
      _einit = .;
    } >FLASH AT>FLASH
    
    .vector :
    {
      . = ALIGN(8);
        *(.vector);
        /*. = ALIGN(64);*/
    } >FLASH AT>FLASH
    
    .text :
    {
      . = ALIGN(4);
      *(.text)
      *(.text.*)
      *(.rodata)
      *(.rodata*)
      . = ALIGN(4);
    } >FLASH AT>FLASH 

    PROVIDE( _etext = . );
    PROVIDE( _eitcm = . );  

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
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) 
            SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array 
            EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH AT>FLASH 
    
    .fini_array     :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)
            SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array 
            EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
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

    .stack : /*ORIGIN(RAM) + __stack_size :*/
	{
	    . = ALIGN(4);
	    PROVIDE(_susrstack = . );
	    . = . + __stack_size;
	    PROVIDE( _eusrstack = .); /*stack overflow when below 0x20000000*/
	} >RAM 


	    
    .data :
    {
      . = ALIGN(4);
      PROVIDE(_data_start_ram = .);
      . = ALIGN(4);
      *(.data .data.*)
      . = ALIGN(8);
      PROVIDE( __global_pointer = . + 0x400 );
      *(.sdata .sdata.*)
      . = ALIGN(8);
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
    heap_start = .;
    /*PROVIDE(current_ends = 0);*/
		/*_heap_limit = _heap_start + _Min_Heap_Size;*/
		heap_limit = heap_start + _Min_Heap_Size;
		. = . + _Min_Heap_Size;
    PROVIDE( _heap_end = . );
    PROVIDE( heap_end = . );
    PROVIDE( end = . );                /*_sbrk uses end for malloc*/
	} >RAM /*set data in ROM then in RAM in startup file*/

}
