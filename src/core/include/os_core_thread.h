/**
 * Thread module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_THREAD_H_
#define OS_CORE_THREAD_H_

#include "os_core_interrupt.h"
#include "os_core_timer.h"
#include "os_core_resource.h"
#include "os_core_runnable.h"

namespace OsCore
{
  class Thread : public Resource, public Runnable
  {
    public:

                                Thread();
                                Thread(char*);
                                Thread(Runnable*);
                                Thread(Runnable*, char*);
      virtual                  ~Thread();
      virtual void              run();
      void                      start();
      uint32                    getId();
      void                      join();
      static Thread*            currentThread();
      static uint32             getCurrentThreadId();
      static void               sleep(uint64, uint32 =0);
      static void               block(Resource*);
      static void               yield(); //Describe in ASM file

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

      static const uint32       STACK_SIZE = 0x2000;

      struct Register 
      { 
        uint32                  a0,a1,b0,b1,a2,a3,b2,b3,a4,a5,b4,b5,a6,a7,
                                b6,b7,a8,a9,b8,b9,a10,a11,b10,b11,a12,a13,
                                b12,b13,a14,a15,b14,b15,a16,a17,b16,b17,
                                a18,a19,b18,b19,a20,a21,b20,b21,a22,a23,
                                b22,b23,a24,a25,b24,b25,a26,a27,b26,b27,
                                a28,a29,b28,b29,a30,a31,b30,b31,amr,irp; 
      }                        *_register;
      Thread*                   _prev;
      Thread*                   _next;
      Runnable*                 _target;
      Resource*                 _block;
      uint8*                    _stack;
      char*                     _name;
      uint32                    _id;
      uint64                    _wakeupTime;
      Status                    _status;

      static struct ThreadList
      {
        Thread*                 current;   //Always first in struct
        Thread*                 tail;    
      }                         _thread;
      static struct HwResource
      {
        Interrupt*              hwInt;
        Timer*                  hwTim;
      }                         _res;
      static uint32             _idCount;
      static uint32             _registerShift;
      static Thread             _main;
      static Register           _system;

      static bool               _init();
      static bool               _isInit();
      void                      _construct(Runnable*, char*);
      void                      _unlink();
      bool                      _setStatus(Status);
      Status                    _getStatus();
      static bool               _enable();
      static bool               _running();
      static void               _sheduler();
      static void               _mainVector();
      static void               _runVector();
      static void               _exitVector();

      friend class              System;
  };
}
#endif //OS_CORE_THREAD_H_
