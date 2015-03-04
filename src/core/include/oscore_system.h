/** 
 * Core of Operation System
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_SYSTEM_H_
#define OSCORE_SYSTEM_H_

#include "oscore_object.h"

namespace oscore
{
  class Timer;

  class System : public Object
  {
    public:

      static uint64             currentTimeMillis();
      static uint64             nanoTime();
      static void               exit(int32);

    private:

      static struct Time
      {
        uint64                  cnt;
        uint64                  acc;
        uint64                  nsec;
        uint64                  time;
      }                         time_;
      static struct HwResource
      {
        Timer*                  hwTim;
      }                         res_;

      static bool               init();
      static bool               deinit();
      static void               start();
      static void               sheduler();
      static void               updateTime();
      static void               initClassesLow(); //Describe in ASM file

      friend class              Thread;
  };
}
#endif //OSCORE_SYSTEM_H_
