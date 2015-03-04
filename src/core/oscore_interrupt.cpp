/** 
 * Interrupt module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_interrupt_handler.h"
#include "oscore_interrupt.h"
#include "oscore_interrupt_hw.h"
#include "oscore_config.h"
#include "oscore_hardware.h"

#ifdef OS_DEBUG
#  define OS_INTERRUPT_DEBUG
#endif


extern cregister volatile unsigned int CSR;

namespace oscore
{
  Interrupt::Vector Interrupt::vectors_[NUMBER_VECTORS];  
  void*             Interrupt::stack_ = NULL;  
  Register*         Interrupt::register_ = NULL;  
  Register*         Interrupt::pointer_ = NULL;
  
  
  /**
   * Constructor
   */
  Interrupt::Interrupt()
  {
    vector_ = NULL;
  }  
  
  /**
   * Constructor
   *
   * @param enum Source       source
   * @param InterruptHandler* handler   
   */
  Interrupt::Interrupt(Source source, InterruptHandler* handler)
  {
    vector_ = NULL;
    int32 error = getError();
    if(error) return;
    setError( setSource(source, handler) );
  }

  /**
   * Destructor
   */
  Interrupt::~Interrupt()
  {
    removeSource();
    vector_ = NULL;
  }
  
  /**
   * Lock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::lock()
  {
    if(isAlloc() == true) lockLow(vector_->number);
  }
  
  /**
   * Unlock maskable interrupt
   *
   * @return void
   */  
  void Interrupt::unlock()
  {
    if(isAlloc() == true) unlockLow(vector_->number);
  }
  
  /**
   * Set interrupt status
   *
   * @return void
   */  
  void Interrupt::set()
  {
    if(isAlloc() == true) setLow(vector_->number);
  }
  
  /**
   * Clear interrupt status
   *
   * @return void
   */  
  void Interrupt::clear()
  {
    if(isAlloc() == true) clearLow(vector_->number);
  }
  
  /**
   * Jump to interrupt HW vector
   *
   * @return void
   */  
  void Interrupt::jump()
  {
    if(isAlloc() == true) jumpLow(vector_->number);
  }  
  
  /**
   * Init interrupt HW
   *
   * @return bool
   */
  bool Interrupt::init()
  {
    int32 stage = 0;
    CSR |= 0x2;
    REG_MUXH = 0x00000000;
    REG_MUXL = 0x00000000;    
    REG_EXTPOL = 0x00000000;
    memset(vectors_, 0x0, sizeof(vectors_));
    while(true)
    {
      //Stage 1:
      stage++;
      register_ = pointer_ = new Register;
      if(pointer_ == NULL) break;    
      //Stage 2:
      stage++;
      stack_ = new uint8[SYS_STACK_SIZE];
      if(stack_ == NULL) break; 
      //Stage complete:
      stage = 0;
      stack_ = (void*)((uint32)stack_ + SYS_STACK_SIZE - 8);          
      break;
    }
    switch(stage)
    {
      case 2: delete stack_;
      case 1: delete register_;
      default: return false;
      case 0: return true;
    }
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
    delete register_;
    if(stack_ != NULL)
    {
      void* stack = (void*)((uint32)stack_ - SYS_STACK_SIZE + 8);  
      delete[] stack;    
    }
    return true;
  }
  
  /**
   * Set interrupt source
   *
   * @param enum Source       source
   * @param InterruptHandler* handler
   * @return int32 Error code or zero
   */  
  int32 Interrupt::setSource(Source source, InterruptHandler* handler)
  {
    int32 is = disable();  
    if(handler == NULL){ enable(is); return OSE_ARG; }
    if(source == NMI){ enable(is); return OSE_RES; } // TODO
    //Checking interrupt source is alloced:
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(vectors_[i].source == source) 
      { 
        enable(is); 
        return OSE_BUSY;
      }
    }
    for(int32 i=0; i<NUMBER_VECTORS; i++)
    {
      if(vectors_[i].handler != NULL) continue;
      if(setMux(source, i, &vectors_[i].mux) == false){ enable(is); return OSE_HW; }
      vector_ = &vectors_[i];
      vector_->source = source;
      vector_->handler = handler;
      vector_->number = i+4;
      break;
    }  
    enable(is);    
    return 0;
  }
  
  /**
   * Remove this interrupt source
   *
   * @return void
   */  
  void Interrupt::removeSource()
  {
    int32 is = disable();
    if(isAlloc() == false){ enable(is); return; }
    if(lockLow(vector_->number) != 0){ enable(is); return; }
    *vector_->mux.reg &= vector_->mux.mask;
    memset(vector_, 0x0, sizeof(Vector));
    enable(is);  
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
  
  /**
   * Interrupt handle
   *
   * @param int32 vec Hw interrupt vector number
   * @return void
   */  
  void Interrupt::handle(int32 vec)
  {
    (&vectors_[vec-4])->handler->handle();
  }
  
  /**
   * Set save context register pointer
   *
   * @return void
   */  
  void Interrupt::setRegister(Register* reg)
  {
    pointer_ = reg;
  }  
}
