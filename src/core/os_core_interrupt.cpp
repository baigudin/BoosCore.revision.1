/** 
 * Interrupt module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "os_core_interrupt.h"
#include "os_core_interrupt_hw.h"

extern cregister volatile unsigned int CSR;

namespace OsCore
{
  Interrupt::Vector Interrupt::_vectors[NUMBER_VECTORS];  
  
  /**
   * Constructor
   */
  Interrupt::Interrupt(Source source, void (*handler)())
  {
    _vector = NULL;
    uint32 is = disable();
    if(_isAlloc() == true){ _error(OSE_ALLOC); enable(is); return; }
    if(handler == NULL){ _error(OSE_ARG); enable(is); return; }
    if(source == NMI){ _error(OSE_RES); enable(is); return; } // TODO
    //Checking interrupt source is alloced:
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(_vectors[i].source == source) 
      { 
        _error(OSE_BUSY); 
        enable(is); 
        return; 
      }
    }
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(_vectors[i].handler != NULL) continue;
      if(_setMux(source, i, &_vectors[i].mux) == false){ _error(OSE_HW); enable(is); return; }
      _vector = &_vectors[i];
      _vector->source = source;
      _vector->handler = handler;
      _vector->number = i+4;
      break;
    }
    enable(is);
  }

  /**
   * Destructor
   */
  Interrupt::~Interrupt()
  {
    uint32 is = disable();
    if(_isAlloc() == false){ enable(is); return; }
    if(_lock(_vector->number) != 0){ enable(is); return; }
    *_vector->mux.reg &= _vector->mux.mask;
    memset(_vector, 0x0, sizeof(Vector));
    _vector = NULL;
    enable(is);
  }
  
  /**
   * Lock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::lock()
  {
    if(_isAlloc() == true) _lock(_vector->number);
  }
  
  /**
   * Unlock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::unlock()
  {
    if(_isAlloc() == true) _unlock(_vector->number);
  }
  
  /**
   * Set interrupt status
   *
   * @return void
   */  
  void Interrupt::set()
  {
    if(_isAlloc() == true) _set(_vector->number);
  }
  
  /**
   * Clear interrupt status
   *
   * @return void
   */  
  void Interrupt::clear()
  {
    if(_isAlloc() == true) _clear(_vector->number);
  }
  
  /**
   * Init interrupt HW
   *
   * @return bool
   */
  bool Interrupt::_init()
  {
    CSR |= 0x2;
    REG_MUXH = 0x00000000;
    REG_MUXL = 0x00000000;    
    REG_EXTPOL = 0x00000000;
    memset(_vectors, 0x0, sizeof(_vectors));
    return true;
  }    
  
  /**
   * Current object is have hw interrupt
   *
   * @return bool
   */
  bool Interrupt::_isAlloc()
  {
    return (_vector == NULL) ? false : true;
  }
  
  /**
   * Set MUX register
   *
   * @return bool
   */  
  bool Interrupt::_setMux(Source source, uint32 vi, Mux* mux)
  {
    uint32 shift;
    switch( vi )
    {
      case  0: mux->reg = DREG_MUXL; mux->mask = 0xffffffe0; shift = 0;  break;    
      case  1: mux->reg = DREG_MUXL; mux->mask = 0xfffffc1f; shift = 5;  break;
      case  2: mux->reg = DREG_MUXL; mux->mask = 0xffff83ff; shift = 10; break;
      case  3: mux->reg = DREG_MUXL; mux->mask = 0xffe0ffff; shift = 16; break;
      case  4: mux->reg = DREG_MUXL; mux->mask = 0xfc1fffff; shift = 21; break;
      case  5: mux->reg = DREG_MUXL; mux->mask = 0x83ffffff; shift = 26; break;
      case  6: mux->reg = DREG_MUXH; mux->mask = 0xffffffe0; shift = 0;  break;
      case  7: mux->reg = DREG_MUXH; mux->mask = 0xfffffc1f; shift = 5;  break;
      case  8: mux->reg = DREG_MUXH; mux->mask = 0xffff83ff; shift = 10; break;
      case  9: mux->reg = DREG_MUXH; mux->mask = 0xffe0ffff; shift = 16; break;
      case 10: mux->reg = DREG_MUXH; mux->mask = 0xfc1fffff; shift = 21; break;
      case 11: mux->reg = DREG_MUXH; mux->mask = 0x83ffffff; shift = 26; break;      
      default: return false;
    }
    *mux->reg = ( *mux->reg & mux->mask ) | ( source << shift );
    return true;
  }  
}
