/** 
 * Interrupt module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_interrupt.h"
#include "oscore_interrupt_hw.h"

extern cregister volatile unsigned int CSR;

namespace oscore
{
  Interrupt::Vector Interrupt::vectors_[NUMBER_VECTORS];  
  
  /**
   * Constructor
   */
  Interrupt::Interrupt(Source source, void (*handler)())
  {
    vector_ = NULL;
    int32 is = disable();
    if(isAlloc() == true){ setError(OSE_ALLOC); enable(is); return; }
    if(handler == NULL){ setError(OSE_ARG); enable(is); return; }
    if(source == NMI){ setError(OSE_RES); enable(is); return; } // TODO
    //Checking interrupt source is alloced:
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(vectors_[i].source == source) 
      { 
        setError(OSE_BUSY); 
        enable(is); 
        return; 
      }
    }
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(vectors_[i].handler != NULL) continue;
      if(setMux(source, i, &vectors_[i].mux) == false){ setError(OSE_HW); enable(is); return; }
      vector_ = &vectors_[i];
      vector_->source = source;
      vector_->handler = handler;
      vector_->number = i+4;
      break;
    }
    enable(is);
  }

  /**
   * Destructor
   */
  Interrupt::~Interrupt()
  {
    int32 is = disable();
    if(isAlloc() == false){ enable(is); return; }
    if(lowLock(vector_->number) != 0){ enable(is); return; }
    *vector_->mux.reg &= vector_->mux.mask;
    memset(vector_, 0x0, sizeof(Vector));
    vector_ = NULL;
    enable(is);
  }
  
  /**
   * Lock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::lock()
  {
    if(isAlloc() == true) lowLock(vector_->number);
  }
  
  /**
   * Unlock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::unlock()
  {
    if(isAlloc() == true) lowUnlock(vector_->number);
  }
  
  /**
   * Set interrupt status
   *
   * @return void
   */  
  void Interrupt::set()
  {
    if(isAlloc() == true) lowSet(vector_->number);
  }
  
  /**
   * Clear interrupt status
   *
   * @return void
   */  
  void Interrupt::clear()
  {
    if(isAlloc() == true) lowClear(vector_->number);
  }
  
  /**
   * Init interrupt HW
   *
   * @return bool
   */
  bool Interrupt::init()
  {
    CSR |= 0x2;
    REG_MUXH = 0x00000000;
    REG_MUXL = 0x00000000;    
    REG_EXTPOL = 0x00000000;
    memset(vectors_, 0x0, sizeof(vectors_));
    return true;
  }    
  
  /**
   * Deinitialization
   *
   * @return bool
   */
  bool Interrupt::deinit()
  {
    REG_MUXH = 0x00000000;
    REG_MUXL = 0x00000000;    
    REG_EXTPOL = 0x00000000;
    memset(vectors_, 0x0, sizeof(vectors_));
    return true;
  }  
  
  /**
   * Current object is have hw interrupt
   *
   * @return bool
   */
  bool Interrupt::isAlloc()
  {
    return (vector_ == NULL) ? false : true;
  }
  
  /**
   * Set MUX register
   *
   * @return bool
   */  
  bool Interrupt::setMux(Source source, uint32 vi, Mux* mux)
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
