/** 
 * Timer interrupt module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_TIMER_INTERRUPT_H_
#define OSCORE_TIMER_INTERRUPT_H_

#include "oscore_timer.h"
#include "oscore_interrupt.h"

namespace oscore
{
  class Timer;

  class TimerInterrupt : public Interrupt
  {
    public:

      //Available Timer interrupt sources for TMS320C64x:
      enum Source 
      {
        TINT0 = Interrupt::TINT0,
        TINT1 = Interrupt::TINT1,
        TINT2 = Interrupt::TINT2
      };

                                TimerInterrupt(Source, InterruptHandler*);
      virtual                  ~TimerInterrupt();
      Timer*                    getTimer();
    
    private:
      
      Timer*                     timer_;
  };
}
#endif //OSCORE_TIMER_INTERRUPT_H_
