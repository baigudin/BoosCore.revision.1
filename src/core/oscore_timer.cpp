/** 
 * Hardware timer resource.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_interrupt.h" 
#include "oscore_timer.h"
#include "oscore_timer_hw.h"
#include "oscore_config.h"

namespace oscore
{
  bool Timer::lock_[NUMBER_TIMERS] = {false, false, false};
  
  /** 
   * Constructor.
   *
   * Construct Timer object and alloc first free HW timer.
   */
  Timer::Timer()
  {
    for(int32 i=0; i<NUMBER_TIMERS; i++) 
    {
      setError(OSE_OK);
      construct((Number)i);
      if(getError() == OSE_OK) break;
    }
  }  

  /** 
   * Constructor.
   *
   * @param number enum Timer::Number.
   */
  Timer::Timer(Number number)
  {
    construct(number);
  }
  
  /**
   * Timer class constructor.
   *
   * @param number enum Timer::Number.
   */
  void Timer::construct(Number number)
  {
    reg_ = NULL; 
    number_ = number;
    if(number_ >= NUMBER_TIMERS){ setError(OSE_ARG); return; }
    uint32 is = Interrupt::disable();
    if(lock_[number_] == true) 
    {
      setError(OSE_ALLOC); 
      Interrupt::enable(is);
      return;
    }
    switch(number_)
    {
      case TIM0: reg_ = (Reg*)BASE_TIM0; break;
      case TIM1: reg_ = (Reg*)BASE_TIM1; break;
      case TIM2: reg_ = (Reg*)BASE_TIM2; break;            
    }
    reg_->ctl = 0;
    reg_->prd = 0;
    reg_->cnt = 0;
    lock_[number_] = true;
    Interrupt::enable(is);
  }  

  /** 
   * Destructor.
   */
  Timer::~Timer()
  {
    uint32 is = Interrupt::disable();
    if(isAlloc() == false){ Interrupt::enable(is); return; }
    lock_[number_] = false;
    reg_->ctl = 0;
    reg_->prd = 0;
    reg_->cnt = 0;
    reg_ = NULL;
    Interrupt::enable(is);
  }
  
  /**
   * Get timer period.
   *
   * @return timer period register value.
   */  
  uint32 Timer::getPeriod()
  {
    if(isAlloc() == false) return 0;
    return reg_->prd;
  }  
  
  /**
   * Set timer period.
   *
   * @param us timer period in microseconds, zero is set period to maxinum value.
   */  
  void Timer::setPeriod(uint32 us)
  {
    if(isAlloc() == false) return;
    if(us == 0) 
    {
      reg_->prd = 0xffffffff;
    }
    else
    {
      uint64 prd = (uint64)(us * SYS_FREQUENCY) / 8000000ull;
      if( (prd & 0xffffffff00000000ull) == 0) reg_->prd = (uint32)prd;
      else return setPeriod();
    }
  }
  
  /**
   * Get timer counter.
   *
   * @return timer counter register value.   
   */  
  uint32 Timer::getCount()
  {
    if(isAlloc() == false) return 0;  
    return reg_->cnt;
  }
  
  /**
   * Set timer counter.
   *
   * @param count timer counter rerister value.
   */  
  void Timer::setCount(uint32 count)
  {
    if(isAlloc() == false) return;    
    if(count > reg_->prd) return;
    reg_->cnt = count;
  }      
  
  /**
   * Start timer count.
   */  
  void Timer::start()
  {
    if(isAlloc() == true) reg_->ctl = TCTL_SPND | TCTL_CLKSRC | TCTL_HLD | TCTL_GO;
  }
      
  /**
   * Stop timer count.
   */  
  void Timer::stopCount()
  {
    if(isAlloc() == true) reg_->ctl = TCTL_SPND | TCTL_CLKSRC;
  }
  
  /**
   * Current object is have hw timer.
   *
   * @return true if object is have timer source.   
   */  
  bool Timer::isAlloc()
  {
    return (reg_ == NULL) ? false : true;
  }
  
  /**
   * Initialization.
   *
   * @return true if no errors.
   */
  bool Timer::init()
  {
    regReset();
    return true;
  }
  
  /**
   * Deinitialization.
   *
   * @return true if no errors.
   */
  bool Timer::deinit()
  {
    regReset();
    return true;
  } 
 
  /**
   * Reset all HW registers.
   */
  void Timer::regReset()
  {
    *REG_CTL0 = 0;
    *REG_PRD0 = 0;
    *REG_CNT0 = 0;

    *REG_CTL1 = 0;
    *REG_PRD1 = 0;
    *REG_CNT1 = 0;

    *REG_CTL2 = 0;
    *REG_PRD2 = 0;
    *REG_CNT2 = 0;
  }
}
