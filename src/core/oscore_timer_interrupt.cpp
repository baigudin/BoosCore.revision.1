/** 
 * Hardware timer interrupt resource.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_timer.h"
#include "oscore_timer_interrupt.h"

namespace oscore
{
  /**
   * Constructor.
   *
   * @param source enum TimerInterrupt::Source.
   * @param handler pointer to user class which implements oscore::InterruptHandler interface.
   */
  TimerInterrupt::TimerInterrupt(Source source, InterruptHandler* handler) : Interrupt()
  {
    timer_ = NULL;
    int32 error = getError();
    if(error) return;
    Timer::Number timNum;
    Interrupt::Source intSource;
    switch(source)
    {
      case TINT0: timNum = Timer::TIM0; intSource = Interrupt::TINT0; break;
      case TINT1: timNum = Timer::TIM1; intSource = Interrupt::TINT1; break;
      case TINT2: timNum = Timer::TIM2; intSource = Interrupt::TINT2; break;
      default: setError(OSE_ARG); return;
    }    
    error = setSource(intSource, handler);
    if(error != OSE_OK) 
    { 
      setError(OSE_HW);
      return;    
    }
    timer_ = new Timer(timNum);
    if(timer_ == NULL)
    {
      removeSource();
      setError(OSE_MEM);
      return;    
    }
    if(timer_->getError() != OSE_OK)
    {
      removeSource();    
      setError(OSE_HW);
      return;
    }
  }

  /**
   * Destructor.
   */
  TimerInterrupt::~TimerInterrupt()
  {
    delete timer_;
  }
  
  /**
   * Get Timer class for this timer interrupt resource.
   *
   * @return pointer to Timer class of this object.
   */
  Timer* TimerInterrupt::getTimer()
  {
    return timer_;
  }
}
