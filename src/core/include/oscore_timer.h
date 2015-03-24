/** 
 * Hardware timer resource.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_TIMER_H_
#define OSCORE_TIMER_H_

#include "oscore_resource.h" 

namespace oscore
{
  class Timer : public Resource
  {
    public:
      //Available timer numders:
      enum Number
      {
        TIM0 = 0,
        TIM1 = 1,
        TIM2 = 2
      };

                                Timer();
                                Timer(Number);
      virtual                  ~Timer();
      uint32                    getCount();
      uint32                    getPeriod();
      void                      setCount(uint32);
      void                      setPeriod(uint32 =0);
      void                      start();
      void                      stopCount();

    private:

      struct Reg
      {
        uint32  ctl;
        uint32  prd;
        uint32  cnt;
      };

      static const uint32       NUMBER_TIMERS = 3;

      Number                    number_;              //Hardware number of timer
      Reg*                      reg_;                 //HW register addresses
      static bool               lock_[NUMBER_TIMERS]; //Timer is locked of some object

      void                      construct(Number number);
      bool                      isAlloc();
      static bool               init();
      static bool               deinit();
      static void               regReset();

      friend class              System;
  };
}
#endif //OSCORE_TIMER_H_
