/*
 link.ld
*/
OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)
phys = 0x8000;
SECTIONS
{
  .text phys  : {
    *(.text)
  }
  .data  : {
    *(.data)
  }
  .bss  :  { 					
    *(.bss)
  }
}
