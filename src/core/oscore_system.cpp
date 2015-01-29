/** 
 * Core of Operation System
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h" 
#include "oscore_config.h"
#include "oscore_interrupt.h"
#include "oscore_timer.h"
#include "oscore_thread.h"
#include "oscore_system.h"

int32 errno = 0;

namespace oscore
{
  System::Time       System::time_ = {0, 0, 0, 0};      
  System::HwResource System::res_ = {NULL};  
  uint32             System::tos_ = SYS_STACK;

  /**
   * Initialization and start system
   *
   * @return uint64
   */  
  uint64 System::currentTimeMillis()
  {
    return nanoTime() / 1000000ull + time_.time;
  }

  /**
   * Current value of the running system
   *
   * @return uint64
   */  
  uint64 System::nanoTime()
  {
    int32 is = Interrupt::disable();
    updateTime();
    uint64 time = time_.nsec;
    Interrupt::enable(is);
    return time;
  }
  
  /**
   * Initialization and start system
   *
   * @return bool
   */
  void System::start()
  {
    //Initialize:
    if(Object::init() == false) return;
    if(Interrupt::init() == false) return;
    if(Timer::init() == false) return;
    if(System::init() == false) return;
    if(Thread::init() == false) return;
    //System start:
    Thread::begin();    
    //Deinitialize:
    Thread::deinit();
    System::deinit();
    Timer::deinit();
    Interrupt::deinit();
    Object::deinit();
  }

  /**
   * Initialization
   *
   * @return bool
   */
  bool System::init()
  {
    uint32 stage = 0;
    memset(&res_, 0x0, sizeof(HwResource));            
    memset(&time_, 0x0, sizeof(Time));        
    lowInitClasses();      
    while(true)
    {
      //Stage 1:
      stage++;      
      res_.hwTim = new Timer(Timer::TIM1);
      if(res_.hwTim == NULL) break;  
      if(res_.hwTim->getError() != OSE_OK) break;
      res_.hwTim->setPeriod();
      res_.hwTim->start();
      //Stage complete:
      stage = 0;
      break;
    }
    switch(stage)
    {
      case 1: delete res_.hwTim;
      default: return false;    
      case 0: return true;
    }
  }
  
  /**
   * Deinitialization
   *
   * @return bool
   */
  bool System::deinit()
  {
    delete res_.hwTim;
    res_.hwTim = NULL;
    return true;
  }  
  
  /**
   * System sheduler
   *
   * @return void
   */    
  void System::sheduler()
  {
    updateTime();
  }
  
  /**
   * Updete system time
   *
   * @return void
   */    
  void System::updateTime()
  {
    uint64 delta;
    int32 is = Interrupt::disable();
    uint32 cnt = res_.hwTim->getCount();
    time_.acc += (uint64)(cnt - time_.cnt) * 8000000000ull;
    delta = time_.acc / SYS_FREQUENCY;
    time_.nsec += delta;
    time_.acc -= delta * SYS_FREQUENCY;
    time_.cnt = cnt;    
    Interrupt::enable(is);
  }  
}
