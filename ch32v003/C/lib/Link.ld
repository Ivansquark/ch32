ENTRY( _start )

__stack_size = 0x200;

PROVIDE( _stack_size = __stack_size );

MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
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
      . = ALIGN(4);
    } >FLASH AT>FLASH 

    PROVIDE( _etext = . );
    PROVIDE( _eitcm = . );  
    /* Used by the startup to initialize data */
    _data_start_flash = LOADADDR(.data);


	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
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
      PROVIDE( __global_pointer = . + 0x800 );
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
}



