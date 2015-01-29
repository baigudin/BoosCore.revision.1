/**
 * Semaphore module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_interrupt.h"
#include "oscore_thread.h"
#include "oscore_semaphore.h"
#include "oscore_semaphore_list.hpp"

namespace oscore
{
  /** 
   * Constructor
   */
  Semaphore::Semaphore(int32 permits)
  {
    construct(permits, false, NULL);  
  }
  
  /** 
   * Constructor
   */
  Semaphore::Semaphore(int32 permits, const char* name)
           : list_()
  {
    construct(permits, false, name);  
  }  
  
  /** 
   * Constructor
   */
  Semaphore::Semaphore(int32 permits, bool fair) 
  {
    construct(permits, fair, NULL);
  }
  
  /** 
   * Constructor
   */
  Semaphore::Semaphore(int32 permits, bool fair, const char* name) 
           : list_()  
  {
    construct(permits, fair, name);  
  }
  
  /** 
   * Destructor
   */
  Semaphore::~Semaphore()
  {
  }
  
  /**
   * Checking resource is blocked
   *
   * @return bool
   */
  bool Semaphore::isBlocked()
  {
    int32 is = Interrupt::disable();
    int32 index = list_.lock.size() - 1;
    Thread* cur = Thread::currentThread();
    Thread* res = (Thread*)list_.lock.get(index);
    if(cur != res)
    {
      Interrupt::enable(is); 
      return true;
    }
  //return (count_ > 0) ? false : true;
    Interrupt::enable(is);
    return true;
  }

  /**
   * Acquires a permit
   *
   * @param int32 permits
   * @return void
   */  
  void Semaphore::acquire()
  {
    acquire(1);
  }

  /**
   * Acquires the given number of permits
   *
   * @param int32 permits
   * @return void
   */  
  void Semaphore::acquire(int32 permits)
  {
    int32 is, count;
    while(true)
    {
      is = Interrupt::disable();
      count = count_ - permits;
      if(count >= 0 && list_.lock.size() == 0)
      {
        count_ -= permits;
        return Interrupt::enable(is);
      }
      //Locked at semaphore:
      Thread* res = Thread::currentThread();
      while(list_.lock.add(0, res) == false) Thread::yield();
      list_.lock.setPermits(0, permits);
      Thread::block(this);      
      Interrupt::enable(is);
    }
  }
 
  /**
   * Releases a permit
   *
   * @return void
   */  
  void Semaphore::release()
  {
    release(1);
  }
  
  /**
   * Releases the given number of permits
   *
   * @param int32 permits
   * @return void
   */  
  void Semaphore::release(int32 permits)
  {
    int32 is = Interrupt::disable();
    count_ += permits;
    Interrupt::enable(is);
  }

  /**
   * Semaphore constructor
   *
   * @param int32 permits
   * @param bool  fair    
   * @return void
   */
  void Semaphore::construct(int32 permits, bool fair, const char* name)
  {
    name_ = name;
    count_ = permits;  
    fair_ = fair;
  }
}
