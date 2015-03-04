MEMORY
{
  HWI    : origin = 0x00000000, len = 0x00000200
  CODE   : origin = 0x00001000, len = 0x0008f000
  MEM    : origin = 0x00090000, len = 0x00070000
}

SECTIONS
{
  .hwi       : load = HWI
  .text      : load = CODE
  .data      : load = CODE    
  .bss       : load = CODE  
  .cinit     : load = CODE
  .pinit     : load = CODE
  .const     : load = CODE
  .far       : load = CODE
  .cio       : load = CODE
  .switch    : load = CODE
  .template  : load = CODE
}
