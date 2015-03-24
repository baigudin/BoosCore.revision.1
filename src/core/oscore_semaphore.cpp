/**
 * Semaphore class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_system.h"
#include "oscore_interrupt.h"
#include "oscore_thread.h"
#include "oscore_semaphore.h"
#include "oscore_semaphore_list.hpp"

namespace oscore
{
  /** 
   * Constructor.
   *
   * @param permits the initial number of permits available.   
   */
  Semaphore::Semaphore(int32 permits)
  {
    construct(permits, false);  
  }
  
  /** 
   * Constructor.
   *
   * @param permits the initial number of permits available.      
   * @param fair true if this semaphore will guarantee FIFO granting of permits under contention.
   */
  Semaphore::Semaphore(int32 permits, bool fair) 
  {
    construct(permits, fair);
  }
  
  /** 
   * Destructor.
   */
  Semaphore::~Semaphore()
  {
    delete list_.exec;
    delete list_.lock;
  }
  
  /**
   * Check semaphore is blocked.
   *
   * @return true if this semaphore is block.
   */
  bool Semaphore::isBlocked()
  {
    int32 is, permits, count;
    Thread *cur, *res;
    is = Interrupt::disable();
    cur = Thread::currentThread();
    res = list_.lock->pop();
    //Current thread is first in fifo:
    if(cur != res)
    {
      Interrupt::enable(is); 
      return true;
    }
    //Check free space in semaphore zone:
    permits = list_.lock->getPermits();
    count = count_ - permits;
    if(count < 0) 
    {
      Interrupt::enable(is); 
      return true;
    }
    //Unblock thread:
    count_ -= permits;
    if(fair_ == true) list_.exec->push(cur, permits);      
    Interrupt::enable(is);
    return false;
  }
  
  /**
   * Acquires a permit, blocking until one is available.
   */  
  void Semaphore::acquire()
  {
    acquire(1);
  }
 
  /**
   * Releases a permit.
   */  
  void Semaphore::release()
  {
    release(1);
  }    

  /**
   * Acquires the given number of permits.
   *
   * @param permits the number of permits to acquire.
   */  
  void Semaphore::acquire(int32 permits)
  {
    int32 is, count;
    Thread *cur;
    is = Interrupt::disable();
    cur = Thread::currentThread();    
    //Check on free space in semaphore zone:
    count = count_ - permits;
    if(count >= 0 && list_.lock->size() == 0)
    {
      count_ -= permits;
      if(fair_ == true) list_.exec->push(cur, permits);
      return Interrupt::enable(is);
    }
    //Block thread on this semaphore:
    list_.lock->push(cur, permits);
    Thread::block(this);
    //This thread is unblock.
    removeList(list_.lock);
    Interrupt::enable(is);
  }
  
  /**
   * Releases the given number of permits.
   *
   * @param permits the number of permits to release.
   */  
  void Semaphore::release(int32 permits)
  {
    int32 is = Interrupt::disable();
    if(fair_ == true) removeList(list_.exec);
    count_ += permits;    
    Interrupt::enable(is);
  }
  
  /**
   * Remove first element from fifo list.
   *
   * Element is removing if it's element of current thread, else wait.
   * Function complented after removing.
   *
   * @param list pointer to SemaphoreList class
   */  
  void Semaphore::removeList(SemaphoreList* list)
  {
    Thread* cur = Thread::currentThread();        
    while(true)
    {
      if(list->pop() == cur) break;
      Thread::yield();
    }      
    list->remove();
  }  

  /**
   * Semaphore class constructor.
   *
   * @param permits the initial number of permits available.      
   * @param fair true if this semaphore will guarantee FIFO granting of permits under contention.
   */
  void Semaphore::construct(int32 permits, bool fair)
  {
    count_ = permits;
    fair_ = fair;
    list_.exec = new SemaphoreList();
    list_.lock = new SemaphoreList();    
    if(list_.exec == NULL || list_.lock == NULL) 
    {
      delete list_.exec;
      delete list_.lock;
      //Not allocate memory for lists is critical
      //Further execution is realy wrong
      System::exit(OSE_MEM);
    }
  }
}
