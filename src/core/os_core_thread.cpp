/**
 * Thread module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
 
#include "rts.h"
#include "os_core_config.h"
#include "os_core_system.h"
#include "os_core_thread.h"
#include "os_user_main.h"

#ifdef OS_DEBUG
#  define OS_THREAD_DEBUG
#endif

extern void* __bss__;

namespace OsCore
{
  Thread                 Thread::_main = Thread("Main");
  Thread::HwResource     Thread::_res = {NULL, NULL};
  Thread::ThreadList     Thread::_thread = {NULL, NULL};
  Thread::Register       Thread::_system;  
  uint32                 Thread::_idCount = 0;  
  uint32                 Thread::_registerShift = 0;  
  
  /** 
   * Constructor
   */
  Thread::Thread()
  {
    _construct(this, NULL);  
  }
  
  /** 
   * Constructor
   *
   * @param char* name
   */
  Thread::Thread(char* name)
  {
    _construct(this, name);  
  }
  
  /** 
   * Constructor
   *
   * @param Runnable* target
   */
  Thread::Thread(Runnable* target)
  {
    _construct(target, NULL);
  }

  /** 
   * Constructor
   *
   * @param Runnable* target
   * @param char*     name   
   */
  Thread::Thread(Runnable* target, char* name)
  {
    _construct(target, name);
  }  
  
  /**
   * Thread constructor
   *
   * @param Runnable* target    
   * @return void
   */
  void Thread::_construct(Runnable* target, char* name)
  {
    //Set default values:
    _status = UNDEF;    
    _block = NULL;
    _register = NULL;
    _prev = NULL;
    _next = NULL;
    _target = target;
    _stack = NULL;
    _id = 0;
    _name = name;
    _wakeupTime = 0;    
    if(_isInit() == false){ _error(OSE_INIT); return; }
    _id = ++_idCount;    
    //Memory alloced:    
    _register = new Register;
    _stack = new uint8[STACK_SIZE];
    if( _stack == NULL || _register == NULL ) 
    {
      delete[] _stack;
      delete _register;
      _error(OSE_MEM);
      return;
    }
    //Set default registers value:
    memset(_register, 0x0, sizeof(Register));    
    #ifdef OS_THREAD_DEBUG
    #include "os_core_thread_regset.h"
    #endif
    _register->irp = (uint32)&_runVector;
    _register->b3  = (uint32)&_exitVector;
    _register->b14 = (uint32)&__bss__;
    _register->b15 = (uint32)_stack + STACK_SIZE - 8;
    _setStatus(NEW);
  }
  
  /** 
   * Destructor
   */
  Thread::~Thread()
  {
    _unlink();
    delete[] _stack;
    delete _register;
    _status = UNDEF;    
    _block = NULL;
    _register = NULL;
    _prev = NULL;
    _next = NULL;
    _stack = NULL;
    _name = NULL;
  }
  
  /**
   * Run
   *
   * @return void
   */  
  void Thread::run()
  {
  }
  
  /**
   * Start thread
   *
   * @return void
   */  
  void Thread::start()
  {
    uint32 is = Interrupt::disable();
    if( _getStatus() != NEW ) return Interrupt::enable(is);        
    if( _setStatus(RUNNABLE) == false ) return Interrupt::enable(is);    
    Thread* tail = _thread.tail;
    if(tail == NULL)
    {
      this->_next = this;
      this->_prev = this;      
    }
    else
    {
      this->_next = tail->_next;
      tail->_next = this;
      this->_prev = tail;
      this->_next->_prev = this;
    }
    _thread.tail = this;
    Interrupt::enable(is);
  }
  
  /**
   * Waits for this thread to die
   *
   * @return void
   */  
  void Thread::join()
  {
    while(_getStatus() != DEAD);
  }
  
  /**
   * Causes the currently executing thread to sleep
   *
   * @param uint64 millis The length of time to sleep in milliseconds   
   * @param uint32 nanos  Additional nanoseconds to sleep   
   * @return void
   */  
  void Thread::sleep(uint64 millis, uint32 nanos)
  {
    uint32 is = Interrupt::disable();
    Thread* res = currentThread();
    res->_wakeupTime = System::nanoTime() + millis * 1000000ull + nanos;
    res->_setStatus(SLEEPING);
    Interrupt::enable(is);
    yield();
  }
  
  /**
   * Block current thread on given resourse and yeild task
   *
   * @return void
   */  
  void Thread::block(Resource* block)
  {
    uint32 is = Interrupt::disable();
    Thread* res = currentThread();
    res->_block = block;
    res->_setStatus(BLOCKED);
    Interrupt::enable(is);
    Thread::yield();
  }  

  /**
   * Returns the identifier of this Thread
   *
   * @return uint32
   */  
  uint32 Thread::getCurrentThreadId()
  {
    Thread* res = currentThread();
    if(res == NULL) return 0;
    return res->getId();
  }  

  /**
   * Returns a reference to the currently executing thread object
   *
   * @return Thread*
   */
  Thread* Thread::currentThread()
  {
    return _thread.current;
  }
  
  /**
   * Returns the identifier of this Thread
   *
   * @return uint32
   */  
  uint32 Thread::getId()
  {
    return _id;
  }
  
  /**
   * Initialization
   *
   * @return int32 Error code or zero
   */
  bool Thread::_init()
  {
    Thread thr;
    uint32 stage = 0;
    _res.hwInt = NULL;
    _res.hwTim = NULL;
    _registerShift = (uint32)&thr._register - (uint32)&thr;
    #ifdef OS_THREAD_DEBUG
    memset(&_system, 0xff, sizeof(Register));
    #endif
    while(true)
    {
      //Stage 1:
      stage++;
      _res.hwInt = new Interrupt(Interrupt::TIMER, (void(*)())0xffffffff);      
      if(_res.hwInt == NULL) break;
      if(_res.hwInt->error() != OSE_OK) break;
      _res.hwInt->unlock();
      //Stage 2:
      stage++;      
      _res.hwTim = new Timer(Timer::TIM0);
      if(_res.hwTim == NULL) break;  
      if(_res.hwTim->error() != OSE_OK) break;      
      _res.hwTim->setPeriod(osConfig.quant);
      _res.hwTim->start();
      //Stage complete:
      stage = 0;
      break;
    }
    switch(stage)
    {
      case 2: delete _res.hwTim;
      case 1: delete _res.hwInt;
      default: return false;
      case 0: return true;
    }
  }
  
  /**
   * Check initialization
   *
   * @return bool
   */
  bool Thread::_isInit()
  {
    if(_res.hwInt == NULL) return false;
    if(_res.hwTim == NULL) return false;    
    return true;
  }

  /**
   * Thread sheduler
   *
   * @return void
   */    
  void Thread::_sheduler()
  {
    //Select next thread:
    while(true)
    {
      if(_thread.current->_getStatus() == RUNNABLE) break;    
      System::_sheduler();
      switch(_thread.current->_getStatus())
      {
        case RUNNING: 
        {
          _thread.current->_setStatus(RUNNABLE);
        }
        break;
        case SLEEPING: 
        {
          if(System::nanoTime() >= _thread.current->_wakeupTime)
          {
            _thread.current->_wakeupTime = 0;
            _thread.current->_setStatus(RUNNABLE); 
          }
        }
        break;      
        case BLOCKED: 
        {
          if( _thread.current->_block->isBlocked() == false)
          {
            _thread.current->_setStatus(RUNNABLE); 
          }
        }
        break;              
      }    
      _thread.current = _thread.current->_next;    
    }
    _thread.current->_setStatus(RUNNING);    
  }
  
  /**
   * Start vector function
   *
   * @return void
   */  
  void Thread::_mainVector()
  {
    OsUser::Main::main(NULL);
  }

  /**
   * Start vector function
   *
   * @return void
   */  
  void Thread::_runVector()
  {
    _thread.current->_target->run();
  }

  /**
   * Exit vector function
   *
   * @return void
   */  
  void Thread::_exitVector()
  {
    uint32 is = Interrupt::disable();
    _thread.current->_setStatus(DEAD);    
    uint32 alive = 0;
    Thread* thr = _thread.tail;
    while(true)
    {
      if(thr->_getStatus() != DEAD) alive++;
      thr = thr->_next;
      if(_thread.tail == thr) break;
    }
    if(alive != 0) Thread::yield();    
    Interrupt::enable(is);
    //This PC poit is available only if all thread is dead:
    asm(" nop");
    while(true);
  }  
  
  /**
   * Unlink thread from ring
   *
   * @return void
   */  
  void Thread::_unlink()
  {
    uint32 is = Interrupt::disable();
    if(_next == NULL || _prev == NULL)
    {
      Interrupt::enable(is);      
      return;      
    }    
    else if(_next == this && _prev == this)
    {
      _thread.tail = NULL;
    }
    else
    {
      _next->_prev = _prev;
      _prev->_next = _next;    
      if(_thread.tail == this) _thread.tail = _prev;
    }
    _next = NULL;    
    _prev = NULL;
    Interrupt::enable(is);
  }
    
  /**
   * Enable thread execution
   *
   * Function begin execution fist thread 
   * or return false if error
   *
   * @return bool 
   */  
  bool Thread::_running()
  {
    uint32 is = Interrupt::disable();
    _main._register->irp = (uint32)&_mainVector;
    _main.start();
    if(_thread.tail == NULL) return Interrupt::enableReturn<bool>(is, false);
    if(_thread.current != NULL) return Interrupt::enableReturn<bool>(is, false);
    if(_thread.tail->_getStatus() != RUNNABLE) return Interrupt::enableReturn<bool>(is, false);
    _thread.current = _thread.tail;    
    _thread.current->_setStatus(RUNNING);      
    return true;
  }
 
  /**
   * Set status of thread
   *
   * @param Status status New status of object
   * @return bool
   */  
  bool Thread::_setStatus(Status status)
  {
    bool set = false;
    uint32 is = Interrupt::disable();
    switch(_status)
    {
      // ------------------------------
      case UNDEF:
      {
        switch(status)
        {
          case NEW: set = true;
        }              
      }
      break;
      // ------------------------------
      case NEW:      
      {
        switch(status)
        {
          case RUNNABLE: set = true;
        }      
      }
      break;
      // ------------------------------
      case RUNNABLE:
      {
        switch(status)
        {
          case RUNNING: set = true;
        }        
      }
      break;
      // ------------------------------
      case RUNNING:
      {
        switch(status)
        {
          case RUNNABLE:
          case DEAD:
          case SLEEPING: 
          case BLOCKED: set = true;
        }
      }
      break;
      // ------------------------------
      case WAITING:
      {
      }
      break;
      // ------------------------------
      case BLOCKED:
      {
        switch(status)
        {
          case RUNNABLE: set = true;
        }      
      }
      break;
      // ------------------------------
      case SLEEPING:
      {
        switch(status)
        {
          case RUNNABLE: set = true;
        }      
      }
      break;
      // ------------------------------
      case DEAD:
      {
      }
      break;
    }
    if(set == true) _status = status;
    Interrupt::enable(is);
    return set;
  }

  /**
   * Returns current status of thread
   *
   * @return Status enum
   */  
  Thread::Status Thread::_getStatus()
  {
    return _status;
  }
}
