/**
 * Semaphore module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "os_core_interrupt.h"
#include "os_core_semaphore.h"
#include "os_core_thread.h"

namespace OsCore
{
  /** 
   * Constructor
   */
  Semaphore::Semaphore(uint32 permits)
  {
    _construct(permits, false);  
  }
  
  /** 
   * Constructor
   */
  Semaphore::Semaphore(uint32 permits, bool fair)
  {
    _construct(permits, fair);
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
    return (_count > 0) ? false : true;
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
  void Semaphore::acquire(uint32 permits)
  {
    uint32 is;
    int32 count;
    while(true)
    {
      is = Interrupt::disable();
      count = _count - permits;
      if(count >= 0)
      {
        _count -= permits;
        return Interrupt::enable(is);
      }
      //Locked at semaphore:
      if(_fair)
      {
        Thread* res = Thread::currentThread();
        asm(" nop");        
      }
      Interrupt::enable(is);
      Thread::block(this);
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
  void Semaphore::release(uint32 permits)
  {
    uint32 is = Interrupt::disable();
    _count += permits;
    Interrupt::enable(is);
  }

  /**
   * Semaphore constructor
   *
   * @param int32 permits
   * @param bool  fair    
   * @return void
   */
  void Semaphore::_construct(uint32 permits, bool fair)
  {
    _count = permits;  
    _fair = fair;
  }
}
