MEMORY
{
  OS_IVECT      : origin = 0x00000000, len = 0x00000200
  OS_STACK      : origin = 0x00001000, len = 0x00001000, fill = 0xffffffff
  OS_CODE       : origin = 0x00004000, len = 0x0008c000
  OS_MEMORY     : origin = 0x00090000, len = 0x00060000
  OS_UNUSED     : origin = 0x000F0000, len = 0x00010000
}
SECTIONS
{
  .os_ivect     : { }                                   > OS_IVECT
  .os_stack     : { 
                    _OS_STACK_BEGIN = .; 
                    _OS_STACK_END = 0x00002000; 
                  }                                     > OS_STACK  
  .os_memory    : {
                    _OS_MEMORY_BEGIN = .;
                    _OS_MEMORY_END = 0x000F0000;
                  }                                     > OS_MEMORY  
  .os_text      : { }                                   > OS_CODE
  .cinit        : { }                                   > OS_CODE
  .pinit        : { }                                   > OS_CODE
  .bss          : { }                                   > OS_CODE
  .data         : { }                                   > OS_CODE
  .const        : { }                                   > OS_CODE
  .text         : { }                                   > OS_CODE
  .far          : { }                                   > OS_CODE
  .cio          : { }                                   > OS_CODE
  .switch       : { }                                   > OS_CODE
  .template     : { }                                   > OS_CODE
}
