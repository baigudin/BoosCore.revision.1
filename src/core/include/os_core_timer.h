/** 
 * Hardware Timer resource
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_TIMER_H_
#define OS_CORE_TIMER_H_

#include "os_core_resource.h" 

namespace OsCore
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

                                Timer(Number);
      virtual                  ~Timer();
      uint32                    getCount();
      void                      setCount(uint32);
      void                      stopCount();
      void                      allowCount();
      void                      setPeriod(uint32 =0);
      void                      start();
      void                      stop();
      void                      reset();

    private:

      static const uint32       NUMBER_TIMERS = 3;

      Number                    _number;              //Hardware number of timer
      struct Reg
      {
        uint32  ctl;
        uint32  prd;
        uint32  cnt;
      }                        *_reg;                 //HW register addresses
      static bool               _lock[NUMBER_TIMERS]; //Timer is locked of some object

      static bool               _init();
      bool                      _isAlloc();

      friend class              System;
  };
}
#endif //OS_CORE_TIMER_H_
