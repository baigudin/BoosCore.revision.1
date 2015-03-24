/**
 * Thread class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_THREAD_H_
#define OSCORE_THREAD_H_

#include "oscore_runnable.h"
#include "oscore_resource.h"

namespace oscore
{
  class  TimerInterrupt;
  class  ThreadHandler;
  struct Register;

  class Thread : public Resource, public Runnable
  {
    public:

      static const int32        MAX_PRIORITY = 10;
      static const int32        MIN_PRIORITY = 1;
      static const int32        NORM_PRIORITY = 5;

                                Thread();
                                Thread(const char*);
                                Thread(Runnable*);
                                Thread(Runnable*, const char*);
      virtual                  ~Thread();
      virtual void              run();
      int32                     getPriority();
      void                      join();
      void                      setPriority(int32);
      void                      start();
      static void               block(Resource*);
      static Thread*            currentThread();
      static void               disable();
      static void               enable();
      static int32              getCurrentThreadId();
      static void               sleep(int64, int32=0);
      static void               yield();

    private:

      enum Status
      {
        UNDEF    = 0,
        NEW      = 1,
        RUNNABLE = 2,
        RUNNING  = 3,
        WAITING  = 4,
        BLOCKED  = 5,
        SLEEPING = 6,
        DEAD     = 7
      };

      struct ThreadList
      {
        Thread*                 current;   //Always first in this struct
        Thread*                 tail;    
      };

      struct InterruptData
      {
        TimerInterrupt*         res;
        ThreadHandler*          handle;
      };

      Register*                 register_;
      Thread*                   prev_;
      Thread*                   next_;
      Runnable*                 target_;
      Resource*                 block_;
      uint8*                    stack_;
      const char*               name_;
      int32                     priority_;
      uint64                    wakeupTime_;
      Status                    status_;
      static ThreadList         thread_;
      static InterruptData      interrupt_;
      static uint32             registerShift_;
      static Thread*            main_;
      static Register*          system_;
      static bool               lockSwitch_;

      void                      construct(Runnable*, const char*);
      void                      destruct();
      void                      unlink();
      bool                      setStatus(Status);
      Status                    getStatus();
      void                      setQuant();
      void                      setRegister();
      void                      resetObjectData();
      static bool               init();
      static bool               deinit();
      static bool               isInit();
      static void               initiate();
      static void               terminate();
      static void               initiateLow();  //Describe in ASM file
      static void               terminateLow(); //Describe in ASM file
      static bool               running();
      static void               sheduler();
      static void               mainVector();
      static void               runVector();
      static void               exitVector();

      friend class              System;
      friend class              ThreadHandler;
  };
}
#endif //OSCORE_THREAD_H_
