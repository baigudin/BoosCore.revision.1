/**
 * Thread module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
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
  class Interrupt;
  class Timer;

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
      void                      start();
      int32                     getId();
      void                      join();
      int32                     getPriority();
      void                      setPriority(int32);
      static Thread*            currentThread();
      static int32              getCurrentThreadId();
      static void               sleep(int64, int32 =0);
      static void               block(Resource*);
      static void               yield();                //Describe in ASM file
      static void               disable();
      static void               enable();

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

      struct Register 
      { 
        uint32                  a0,a1,b0,b1,a2,a3,b2,b3,a4,a5,b4,b5,a6,a7,
                                b6,b7,a8,a9,b8,b9,a10,a11,b10,b11,a12,a13,
                                b12,b13,a14,a15,b14,b15,a16,a17,b16,b17,
                                a18,a19,b18,b19,a20,a21,b20,b21,a22,a23,
                                b22,b23,a24,a25,b24,b25,a26,a27,b26,b27,
                                a28,a29,b28,b29,a30,a31,b30,b31,amr,irp; 
      };

      struct ThreadList
      {
        Thread*                 current;   //Always first in struct
        Thread*                 tail;    
      };

      struct HwResource
      {
        Interrupt*              hwInt;
        Timer*                  hwTim;
      };

      static const uint32       STACK_SIZE = 0x2000;

      Register*                 register_;
      Thread*                   prev_;
      Thread*                   next_;
      Runnable*                 target_;
      Resource*                 block_;
      uint8*                    stack_;
      const char*               name_;
      int32                     id_;
      int32                     priority_;
      uint64                    wakeupTime_;
      Status                    status_;
      static ThreadList         thread_;
      static HwResource         res_;
      static int32              idCount_;
      static uint32             registerShift_;
      static Thread*            main_;
      static Register*          system_;
      static void*              shedulerStack_;
      static bool               lockSwitch_;

      void                      construct(Runnable*, const char*);
      void                      destructor();
      void                      unlink();
      bool                      setStatus(Status);
      Status                    getStatus();
      void                      setQuant();
      void                      resetObjectData();
      static bool               init();
      static bool               deinit();
      static bool               isInit();
      static bool               begin();        //Describe in ASM file
      static bool               lowDisable();   //Describe in ASM file
      static bool               running();
      static void               sheduler();
      static void               mainVector();
      static void               runVector();
      static void               exitVector();

      friend class              System;
  };
}
#endif //OSCORE_THREAD_H_
