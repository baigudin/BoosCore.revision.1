/**
 * Thread class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "main.h"
#include "oscore_system.h"
#include "oscore_timer_interrupt.h"
#include "oscore_thread.h"
#include "oscore_thread_handler.h"
#include "oscore_config.h"
#include "oscore_hardware.h"

#ifdef OS_DEBUG
#  define OS_THREAD_DEBUG
#endif

extern void* __bss__;

namespace oscore
{
  Thread*                Thread::main_ = NULL;
  Register*              Thread::system_ = NULL;
  Thread::InterruptData  Thread::interrupt_ = {NULL, NULL};
  Thread::ThreadList     Thread::thread_ = {NULL, NULL};
  uint32                 Thread::registerShift_ = 0;  
  bool                   Thread::lockSwitch_ = false;
  
  /** 
   * Constructor.
   */
  Thread::Thread()
  {
    construct(this, NULL);  
  }
  
  /** 
   * Constructor.
   *
   * @param name name of this thread.
   */
  Thread::Thread(const char* name)
  {
    construct(this, name);  
  }
  
  /** 
   * Constructor.
   *
   * @param target the object whose run method is invoked when this thread is started.
   */
  Thread::Thread(Runnable* target)
  {
    construct(target, NULL);
  }

  /** 
   * Constructor.
   *
   * @param target the object whose run method is invoked when this thread is started.
   * @param name name of this thread.
   */
  Thread::Thread(Runnable* target, const char* name)
  {
    construct(target, name);
  }  
  
  /** 
   * Destructor.
   */
  Thread::~Thread()
  {
    destruct();
  }
  
  /**
   * Default run method
   */  
  void Thread::run()
  {
  }
  
  /**
   * Causes this thread to begin execution.
   */  
  void Thread::start()
  {
    int32 is = Interrupt::disable();
    if( getStatus() != NEW ) return Interrupt::enable(is);        
    if( setStatus(RUNNABLE) == false ) return Interrupt::enable(is);    
    Thread* tail = thread_.tail;
    if(tail == NULL)
    {
      this->next_ = this;
      this->prev_ = this;      
    }
    else
    {
      this->next_ = tail->next_;
      tail->next_ = this;
      this->prev_ = tail;
      this->next_->prev_ = this;
    }
    thread_.tail = this;
    Interrupt::enable(is);
  }
  
  /**
   * Waits for this thread to die.
   */  
  void Thread::join()
  {
    while(getStatus() != DEAD);
  }
  
  /**
   * Causes the currently executing thread to sleep.
   *
   * @param millis The length of time to sleep in milliseconds.
   * @param nanos  Additional nanoseconds to sleep.
   */  
  void Thread::sleep(int64 millis, int32 nanos)
  {
    int32 is = Interrupt::disable();
    Thread* res = currentThread();
    res->wakeupTime_ = System::nanoTime() + millis * 1000000ull + nanos;
    res->setStatus(SLEEPING);
    Thread::yield();        
    Interrupt::enable(is);
  }
  
  /**
   * Block current thread on given resourse and yeild task.
   *
   * @param block pointer to resource.
   */  
  void Thread::block(Resource* block)
  {
    int32 is = Interrupt::disable();
    Thread* res = currentThread();
    res->block_ = block;
    res->setStatus(BLOCKED);
    Thread::yield();    
    res->block_ = NULL;    
    Interrupt::enable(is);
  }  

  /**
   * Returns the identifier of currently executing thread.
   *
   * @return thread identifier.
   */  
  int32 Thread::getCurrentThreadId()
  {
    Thread* res = currentThread();
    if(res == NULL) return 0;
    return res->getId();
  }  

  /**
   * Returns a pointer to the currently executing thread.
   *
   * @return pointer on thread.
   */
  Thread* Thread::currentThread()
  {
    return thread_.current;
  }

  /**
   * Yield to next thread.
   */  
  void Thread::yield()
  {
    interrupt_.res->jump();
  }
  
  
  /**
   * Get this thread priority.
   *
   * @return priority value.
   */  
  int32 Thread::getPriority()
  {
    return priority_;
  }  
  
  /**
   * Set this thread priority.
   *
   * @param priority number of priority in range [MIN_PRIORITY, MAX_PRIORITY].
   */  
  void Thread::setPriority(int32 priority)
  {
    if(priority > MAX_PRIORITY) priority = MAX_PRIORITY;
    else if(priority < MIN_PRIORITY) priority = MIN_PRIORITY;
    priority_ = priority;
  }
  
  
  /**
   * Lock switching threads
   */    
  void Thread::disable()
  {
    int32 is = Interrupt::disable();
    lockSwitch_ = true;
    Interrupt::enable(is);    
  }
   
  /**
   * Unlock switching threads.
   */   
  void Thread::enable()
  {
    int32 is = Interrupt::disable();
    lockSwitch_ = false;
    Interrupt::enable(is);
  }
  
  /**
   * Initialization.
   *
   * @return true if no errors.
   */
  bool Thread::init()
  {
    Thread thr;
    int32 stage = 0;
    registerShift_ = (uint32)&thr.register_ - (uint32)&thr;
    while(true)
    {
      //Stage 1:
      stage++;
      interrupt_.handle = new ThreadHandler();
      if(interrupt_.handle == NULL) break;
      //Stage 2:
      stage++;
      interrupt_.res = new TimerInterrupt(TimerInterrupt::TINT0, interrupt_.handle);      
      if(interrupt_.res == NULL) break;
      if(interrupt_.res->getError() != OSE_OK) break;
      interrupt_.res->unlock();
      interrupt_.res->getTimer()->setPeriod(SYS_QUANT);
      interrupt_.res->getTimer()->start();
      //Stage 3:
      stage++;            
      main_ = new Thread("Main");
      if(main_ == NULL) break;
      if(main_->getError() != OSE_OK) break;
      main_->register_->irp = (uint32)&mainVector;      
      main_->setPriority(SYS_MAIN_THREAD_PRIORITY);
      main_->start();
      //Stage 4:
      stage++;            
      system_ = new Register;
      if(system_ == NULL) break;
      #ifdef OS_THREAD_DEBUG
      memset(system_, 0xff, sizeof(Register));
      #endif
      //Stage complete:
      stage = 0;
      break;
    }
    switch(stage)
    {
      case 4: delete system_;    
      case 3: delete main_;
      case 2: delete interrupt_.res;
      case 1: delete interrupt_.handle;
      default: return false;
      case 0: return true;
    }
  }
  
  /**
   * Deinitialization
   *
   * @return true if no errors.
   */
  bool Thread::deinit()
  {
    delete system_;    
    delete main_;
    delete interrupt_.res;
    delete interrupt_.handle;
    system_ = NULL;
    main_ = NULL;
    interrupt_.res = NULL;
    interrupt_.handle = NULL;
    memset(&thread_, 0x0, sizeof(ThreadList));
    return true;
  }  
  
  /**
   * Check initialization.
   *
   * @return true if initialize.
   */
  bool Thread::isInit()
  {
    if(interrupt_.handle == NULL) return false;
    if(interrupt_.res == NULL) return false;    
    return true;
  }

  /**
   * Thread sheduler.
   */    
  void Thread::sheduler()
  {
    //Select next thread:
    while(true)
    {
      System::sheduler();
      switch(thread_.current->getStatus())
      {
        case RUNNING: 
        {
          thread_.current->setStatus(RUNNABLE);
        }
        break;
        case SLEEPING: 
        {
          if(System::nanoTime() >= thread_.current->wakeupTime_)
          {
            thread_.current->wakeupTime_ = 0;
            thread_.current->setStatus(RUNNABLE); 
          }
        }
        break;
        case BLOCKED: 
        {
          if( thread_.current->block_->isBlocked() == false)
          {
            thread_.current->setStatus(RUNNABLE); 
          }
        }
        break;              
      }    
      if(lockSwitch_ == false) thread_.current = thread_.current->next_;
      if(thread_.current->getStatus() == RUNNABLE) break;
    }
    thread_.current->setRegister();
    thread_.current->setStatus(RUNNING);    
    thread_.current->setQuant();    
  }
  
  /**
   * Main user class start vector.
   */  
  void Thread::mainVector()
  {
    ::Main::main(NULL);
  }

  /**
   * Run method of Runnable interface start vector.
   */  
  void Thread::runVector()
  {
    thread_.current->target_->run();
  }

  /**
   * Exit vector function.
   */  
  void Thread::exitVector()
  {
    int32 is = Interrupt::disable();
    thread_.current->setStatus(DEAD);    
    uint32 alive = 0;
    Thread* thr = thread_.tail;
    while(true)
    {
      if(thr->getStatus() != DEAD) alive++;
      thr = thr->next_;
      if(thread_.tail == thr) break;
    }
    if(alive != 0) while(true) Thread::yield();    
    interrupt_.res->lock();
    interrupt_.res->getTimer()->setPeriod(0);
    interrupt_.res->getTimer()->stopCount();    
    Interrupt::enable(is);
    //This PC poit is available only if all thread is dead:
    terminate();
    while(true);
  }  
  
  /**
   * Initiate thread execution.
   */  
  void Thread::initiate()
  {
    initiateLow();
    //This PC point is allowable after call Thread::terminate method
  }

  /**
   * Terminate thread execution.
   */    
   void Thread::terminate()
   {
     thread_.current = NULL;   
     return terminateLow();
     //This PC point is not allowable
   }
  
  /**
   * Enable thread execution.
   *
   * Function begin execution fist thread 
   * or return false if error.
   *
   * @return false if error.
   */  
  bool Thread::running()
  {
    int32 is = Interrupt::disable();
    if(thread_.tail == NULL){Interrupt::enable(is); return false;}
    if(thread_.current != NULL){Interrupt::enable(is); return false;}
    if(thread_.tail->getStatus() != RUNNABLE){Interrupt::enable(is); return false;}
    thread_.current = thread_.tail;    
    thread_.current->setRegister();
    thread_.current->setStatus(RUNNING);      
    thread_.current->setQuant();
    interrupt_.res->clear();    
    return true;
  }

  /**
   * Set interrupt content register.
   */  
  void Thread::setRegister()
  {
    Interrupt::setRegister(register_);    
  }

  /**
   * Set thread quant time
   */  
  void Thread::setQuant()
  {
    interrupt_.res->getTimer()->setCount(0);
    interrupt_.res->getTimer()->setPeriod(priority_ * SYS_QUANT);
  }
 
  /**
   * Set status of thread.
   *
   * @param status new status of this thread.
   * @return true if status is changed.
   */
  bool Thread::setStatus(Status status)
  {
    bool set = false;
    int32 is = Interrupt::disable();
    switch(status_)
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
    if(set == true) status_ = status;
    Interrupt::enable(is);
    return set;
  }

  /**
   * Returns current status of this thread.
   *
   * @return status of this thread.
   */  
  Thread::Status Thread::getStatus()
  {
    return status_;
  }
  
  /**
   * Constructor of this class.
   *
   * @param target the object whose run method is invoked when this thread is started.
   * @param name name of this thread.
   */
  void Thread::construct(Runnable* target, const char* name)
  {
    //Set default values:
    resetObjectData();
    if(isInit() == false){ setError(OSE_INIT); return; }
    target_ = target;
    priority_ = NORM_PRIORITY;
    name_ = name;
    //Memory alloced:    
    register_ = new Register;
    stack_ = new uint8[SYS_STACK_SIZE];
    if(stack_ == NULL || register_ == NULL) 
    {
      delete[] stack_;
      delete register_;
      setError(OSE_MEM);
      return;
    }
    //Set default registers value:
    memset(register_, 0x0, sizeof(Register));    
    #ifdef OS_THREAD_DEBUG
    #include "oscore_thread_regset.h"
    #endif
    register_->irp = (uint32)&runVector;
    register_->b3  = (uint32)&exitVector;
    register_->b14 = (uint32)&__bss__;
    register_->b15 = (uint32)stack_ + SYS_STACK_SIZE - 8;
    setStatus(NEW);
  }

  /**
   * Destructor of this class.
   */  
  void Thread::destruct()
  {
    unlink();
    delete[] stack_;
    delete register_;
    resetObjectData();
  } 
  
  /**
   * Reset object to default.
   */  
  void Thread::resetObjectData()
  {
    register_ = NULL;
    prev_ = NULL;
    next_ = NULL;
    target_ = NULL;
    block_ = NULL;
    stack_ = NULL;
    name_ = NULL;
    priority_ = 0;
    wakeupTime_ = 0;
    status_ = UNDEF;
  }   
  
  /**
   * Unlink thread from ring.
   */  
  void Thread::unlink()
  {
    int32 is = Interrupt::disable();
    if(next_ == NULL || prev_ == NULL)
    {
      Interrupt::enable(is);      
      return;      
    }    
    else if(next_ == this && prev_ == this)
    {
      thread_.tail = NULL;
    }
    else
    {
      next_->prev_ = prev_;
      prev_->next_ = next_;    
      if(thread_.tail == this) thread_.tail = prev_;
    }
    next_ = NULL;    
    prev_ = NULL;
    Interrupt::enable(is);
  }
}
