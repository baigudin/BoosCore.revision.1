/** 
 * Core of Operation System
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h" 
#include "os_core_config.h"
#include "os_core_interrupt.h"
#include "os_core_timer.h"
#include "os_core_thread.h"
#include "os_core_system.h"

int32 errno = 0;

namespace OsCore
{
  System::Time       System::_time = {0, 0, 0, 0};      
  System::HwResource System::_res = {NULL};  

  /**
   * Initialization and start system
   *
   * @return uint64
   */  
  uint64 System::currentTimeMillis()
  {
    return nanoTime() / 1000000ull + _time.time;
  }

  /**
   * Current value of the running system
   *
   * @return uint64
   */  
  uint64 System::nanoTime()
  {
    uint32 is = Interrupt::disable();
    _updateTime();
    uint64 time = _time.nsec;
    Interrupt::enable(is);
    return time;
  }
  
  /**
   * Initialization and start system
   *
   * @return bool
   */
  void System::_start()
  {
    //Initialize HW:
    if(Object::_init() == false) return;
    if(Interrupt::_init() == false) return;
    if(Timer::_init() == false) return;
    if(Thread::_init() == false) return;
    if(System::_init() == false) return;
    //System start:
    Thread::_enable();    
  }
  
  /**
   * Initialization
   *
   * @return int32
   */
  bool System::_init()
  {
    uint32 stage = 0;
    memset(&_res, 0x0, sizeof(HwResource));            
    memset(&_time, 0x0, sizeof(Time));        
    _initClasses();      
    while(true)
    {
      //Stage 1:
      stage++;      
      _res.hwTim = new Timer(Timer::TIM1);
      if(_res.hwTim == NULL) break;  
      if(_res.hwTim->error() != OSE_OK) break;
      _res.hwTim->setPeriod();
      _res.hwTim->start();
      //Stage complete:
      stage = 0;
      break;
    }
    switch(stage)
    {
      case 1: delete _res.hwTim;
      default: return false;    
      case 0: return true;
    }
  }
  
  /**
   * System sheduler
   *
   * @return void
   */    
  void System::_sheduler()
  {
    _updateTime();
  }
  
  /**
   * Updete system time
   *
   * @return void
   */    
  void System::_updateTime()
  {
    uint64 delta;
    uint32 is = Interrupt::disable();
    uint32 cnt = _res.hwTim->getCount();
    _time.acc += (uint64)(cnt - _time.cnt) * 8000000000ull;
    delta = _time.acc / osConfig.frequency;
    _time.nsec += delta;
    _time.acc -= delta * osConfig.frequency;
    _time.cnt = cnt;    
    Interrupt::enable(is);
  }  
}
