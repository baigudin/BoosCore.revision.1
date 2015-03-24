/** 
 * Root class of the system class hierarchy.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_interrupt.h" 
#include "oscore_thread.h" 
#include "oscore_object.h"
#include "oscore_object_page.hpp"
#include "oscore_object_memory.hpp"
#include "oscore_config.h"

namespace oscore
{
  /** 
   * Constructor.
   */
  Object::Object()
  {
    memory_ = (ObjectMemory*)((uint32)this - sizeof(ObjectMemory));
    if((memory_->isMemory() && memory_->page_->isMemory()) == false) memory_ = NULL;
    setError(OSE_OK);
  } 
  
  /** 
   * Destructor.
   */
  Object::~Object()
  {
  }
  
  /**
   * Get error number.
   *
   * @return error code or zero.
   */  
  int32 Object::getError()
  {
    return error_;
  }
  
  /**
   * Set error number.
   *
   * @param error error number of enum Error.
   */  
  void Object::setError(int32 error)
  {
    error_ = error;
  }  

  /**
   * Get error string.
   *
   * @param error error number.
   * @return pointer to error string.
   */    
  const char* Object::getErrorString(int32 error)
  {
    switch(error)
    {
      case OSE_OK:    return "No errors";
      case OSE_ARG:   return "Argument error";
      case OSE_CMD:   return "Command error";       
      case OSE_MEM:   return "Memory error";
      case OSE_HW:    return "Hardware error";
      case OSE_ID:    return "Error object ID";       
      case OSE_ALLOC: return "Alloced error";
      case OSE_RES:   return "Resource error";
      case OSE_BUSY:  return "Something is busy";
      case OSE_INIT:  return "Initializetion error";
      default: return NULL;
    }
  }
  
  /**
   * Check this Object allocates.
   *
   * @return true if this object alloceted in heap.
   */  
  bool Object::onHeap()
  {
    return (memory_ == NULL) ? false : true;
  }
  
  /**
   * Initialization.
   *
   * @return true if no errors.
   */
  bool Object::init()
  {
    if(sizeof(ObjectPage) & 0x7) return false;
    if(sizeof(ObjectMemory) & 0x7) return false;    
    uint32 n = sizeof(SYS_MEMORY) / (2 * sizeof(uint32));    
    for(int32 i=0; i<n; i++)
    {
      uint32 addr = SYS_MEMORY[i].addr;
      uint32 size = SYS_MEMORY[i].size;    
      if(sizeof(ObjectPage) + sizeof(ObjectMemory) + 16 > size & ~0x7) continue;  
      memAdd((void*)addr, size);
    }
    ObjectMemory::dumpAllocated();
    return true;
  }
  
  /**
   * Deinitialization.
   *
   * @return true if no errors.
   */
  bool Object::deinit()
  {
    ObjectMemory::dumpAllocated();  
    return true; //TODO
  }

  /**
   * Allocate new memory block in heap.
   *
   * @param size size in byte.
   * @return pointer to allocated memory address or NULL, if error.
   */   
  void* Object::memAlloc(uint32 size)
  {
    return ObjectMemory::create(size);  
  }

  /**
   * Free allocated memory block.
   *
   * @param ptr pointer to allocated memory address.
   * @return true if block is removed.
   */   
  bool Object::memFree(void* ptr)
  {
    return ObjectMemory::free((Object*)ptr);
  }
  
  /** 
   * Added new memory page.
   *
   * @param addr address of memory page.
   * @param size size of memory page in sizeof.
   * @return real memory page address.
   */
  void* Object::memAdd(void* addr, uint32 size)
  {
    int32 is = Interrupt::disable();
    //Create page descriptor:
    ObjectPage* page = ObjectPage::init(addr, size);
    if(page == NULL) 
    {
      Interrupt::enable(is);
      return NULL;
    }
    //Create memory descriptor:
    ObjectMemory* mem = ObjectMemory::init(page);
    if(mem == NULL) 
    {
      Interrupt::enable(is);
      return NULL;
    }    
    Interrupt::enable(is);
    return page;      
  }
  
  /** 
   * Remove memory page.
   *
   * @param real memory page address.
   * @return true if page is removed.
   */  
  bool Object::memRemove(void* addr)
  {
    return false; //TODO
  }
  
  /**
   * Operator new.
   *
   * @param size size in byte.
   * @return allocated memory address or NULL, if error.
   */
  void* Object::operator new(uint32 size)
  {
    return memAlloc(size);
  }
  
  /**
   * Operator new.
   *
   * @param size unused.
   * @param prt  address of memory.
   * @return address of memory.
   */
  void* Object::operator new(uint32, void* ptr)
  {
    return ptr;
  }
  
  /**
   * Operator new[].
   *
   * @param size unused.
   * @param prt  address of memory.
   * @return address of memory.      
   */
  void* Object::operator new[](uint32, void* ptr)
  {
    return NULL;
  }
  
  /**
   * Operator new[].
   *
   * @param size size in byte.
   * @return NULL.
   */
  void* Object::operator new[](uint32 size)
  {
    return NULL;
  }
  
  /**
   * Operator delete.
   *
   * @param ptr address of allocated memory.
   * @return void.
   */
  void Object::operator delete(void* ptr)
  {
    memFree(ptr);
  }

  /**
   * Operator delete[].
   *
   * @param ptr address of allocated memory.
   * @return void.   
   */  
  void Object::operator delete[](void* ptr)   
  {
  }
  
  /**
   * Operator delete.
   *
   * @param ptr unused.
   * @param unused unused.   
   * @return void.    
   */  
  void Object::operator delete(void*, void*)
  {
  }
  
  /**
   * Operator delete[].
   *
   * @param ptr unused.
   * @param unused unused.   
   * @return void.    
   */  
  void Object::operator delete[](void*, void*)
  {
  }
}
