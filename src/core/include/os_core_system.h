/** 
 * Core of Operation System
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_SYSTEM_H_
#define OS_CORE_SYSTEM_H_

#include "os_core_object.h"
#include "os_core_timer.h"
#include "os_core_thread.h"

namespace OsCore
{
  class System : public Object
  {
    public:

      static uint64             currentTimeMillis();
      static uint64             nanoTime();

    private:

      static struct Time
      {
        uint64                  cnt;
        uint64                  acc;
        uint64                  nsec;
        uint64                  time;
      }                         _time;
      static struct HwResource
      {
        Timer*                  hwTim;
      }                         _res;

      static void               _start();
      static bool               _init();
      static void               _sheduler();
      static void               _updateTime();
      static void               _initClasses(); //Describe in ASM file

      friend class              Thread;
  };
}
#endif //OS_CORE_SYSTEM_H_
