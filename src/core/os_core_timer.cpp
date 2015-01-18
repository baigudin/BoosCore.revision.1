/** 
 * Hardware Timer resource
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "os_core_interrupt.h" 
#include "os_core_timer.h"
#include "os_core_timer_hw.h"
#include "os_core_config.h" 

namespace OsCore
{
  bool Timer::_lock[NUMBER_TIMERS] = {false, false, false};

  /** 
   * Constructor
   *
   * @param uint32 number Number of system timer
   */
  Timer::Timer(Number number) : _reg(NULL), _number(number)
  {
    if(_number >= NUMBER_TIMERS){ _error(OSE_ARG); return; }
    uint32 is = Interrupt::disable();
    if(_lock[_number] == true) 
    {
      _error(OSE_ALLOC); 
      Interrupt::enable(is);
      return;
    }
    switch(_number)
    {
      case TIM0: _reg = (Reg*)BASE_TIM0; break;
      case TIM1: _reg = (Reg*)BASE_TIM1; break;
      case TIM2: _reg = (Reg*)BASE_TIM2; break;            
    }
    _reg->ctl = 0;
    _reg->prd = 0;
    _reg->cnt = 0;
    _lock[_number] = true;
    Interrupt::enable(is);
  }

  /** 
   * Destructor
   */
  Timer::~Timer()
  {
    uint32 is = Interrupt::disable();
    if(_isAlloc() == false){ Interrupt::enable(is); return; }
    _lock[_number] = false;
    _reg->ctl = 0;
    _reg->prd = 0;
    _reg->cnt = 0;
    _reg = NULL;
    Interrupt::enable(is);
  }
  
  /**
   * Set timer period
   *
   * @param uint32 us Period in micro seconds
   * @return void
   */  
  void Timer::setPeriod(uint32 us)
  {
    if(_isAlloc() == false) return;
    if(us == 0) 
    {
      _reg->prd = 0xffffffff;
    }
    else
    {
      uint64 prd = (uint64)(us * osConfig.frequency) / 8000000ull;
      if( (prd & 0xffffffff00000000ull) == 0) _reg->prd = (uint32)prd;      
    }
  }
  
  /**
   * Get timer count
   *
   * @return uint32
   */  
  uint32 Timer::getCount()
  {
    if(_isAlloc() == false) return 0;  
    return _reg->cnt;
  }
  
  /**
   * Timer reset
   *
   * @param uint32 count Counter value
   * @return void
   */  
  void Timer::setCount(uint32 count)
  {
    if(_isAlloc() == false) return;    
    if(count > _reg->prd) return;
    _reg->cnt = count;
  }      
  
  /**
   * Timer start
   *
   * @return this
   */  
  void Timer::start()
  {
    if(_isAlloc() == true) _reg->ctl = TCTL_SPND | TCTL_CLKSRC | TCTL_HLD | TCTL_GO;
  }
      
  /**
   * Timer stop
   *
   * @return void
   */  
  void Timer::stopCount()
  {
    if(_isAlloc() == true) _reg->ctl = TCTL_SPND | TCTL_CLKSRC;
  }
  
  void Timer::allowCount()
  {
    _reg->ctl = TCTL_SPND | TCTL_CLKSRC | TCTL_HLD;
  }
  
  /**
   * Current object is have hw timer
   *
   * @return bool
   */  
  bool Timer::_isAlloc()
  {
    return (_reg == NULL) ? false : true;
  }
  
  /**
   * Initialization
   *
   * @return int32 Error code or zero
   */
  bool Timer::_init()
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
    return true;
  }
}
