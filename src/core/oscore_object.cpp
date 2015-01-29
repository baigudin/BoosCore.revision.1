/** 
 * Object of OS
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "oscore_config.h"
#include "oscore_interrupt.h" 
#include "oscore_thread.h" 
#include "oscore_object.h"
#include "oscore_object_page.hpp"
#include "oscore_object_memory.hpp"

namespace oscore
{
  /** 
   * Constructor
   */
  Object::Object()
  {
    memory_ = (ObjectMemory*)((uint32)this - sizeof(ObjectMemory));
    if((memory_->isMemory() && memory_->page_->isMemory()) == false) memory_ = NULL;
    errno_ = OSE_OK;
  } 
  
  /** 
   * Destructor
   */
  Object::~Object()
  {
  }
  
  /**
   * Get error number
   *
   * @return int32 Error code or zero
   */  
  int32 Object::getError()
  {
    return errno_;
  }

  /**
   * Get error string
   *
   * @param int32 error Error code or zero   
   * @return const char*
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
       case OSE_ALLOC: return "Alloced error";
       case OSE_RES:   return "Resource error";
       case OSE_BUSY:  return "Something is busy";
       case OSE_INIT:  return "Initializetion error";
       default: return NULL;
     }
   }
  
  /**
   * Object alloced in heap
   *
   * @return bool
   */  
  bool Object::onHeap()
  {
    return (memory_ == NULL) ? false : true;
  }
  
  /**
   * Operator new
   *
   * @param uint32 size Size in byte
   * @return void* Allocated memory address or NULL, if error
   */
  void* Object::operator new(uint32 size)
  {
    return memAlloc(size);
  }
  
  /**
   * Operator new
   *
   * @param uint32      Unused
   * @param void*  prt Address of memory
   * @return void* Address of memory
   */
  void* Object::operator new(uint32, void* ptr)
  {
    return ptr;
  }
  
  /**
   * Operator delete
   *
   * @param void* addr Address of allocated memory
   * @return void
   */
  void Object::operator delete(void* ptr)
  {
    memFree(ptr);
  }
  
  /**
   * Operator new[]
   *
   * @param uint32     Unused
   * @param void*  prt Address of memory
   * @return void* Address of memory      
   */
  void* Object::operator new[](uint32, void* ptr)
  {
    return NULL;
  }
  
  /**
   * Operator new[]
   */
  void* Object::operator new[](uint32 size)
  {
    return NULL;
  }

  /**
   * Operator delete[]
   */  
  void Object::operator delete[](void* ptr)   
  {
  }
  
  /**
   * Set error number
   *
   * @return int32 Error code or zero
   */  
  int32 Object::setError(int32 errno)
  {
    errno_ = errno;
    return getError();
  }
  
  /**
   * Initialization
   *
   * @return bool
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
   * Deinitialization
   *
   * @return bool
   */
  bool Object::deinit()
  {
    ObjectMemory::dumpAllocated();  
    return true; //TODO
  }

  /**
   * Allocate of memory
   *
   * @param uint32 size Size in byte
   * @return void* Allocated memory address or NULL, if error
   */   
  void* Object::memAlloc(uint32 size)
  {
    return ObjectMemory::create(size);  
  }

  /**
   * Free allocated memory
   *
   * @param void* ptr Address of allocated memory
   * @return bool
   */   
  bool Object::memFree(void* ptr)
  {
    return ObjectMemory::free((Object*)ptr);
  }
  
  /** 
   * Added memory
   *
   * @param void*  addr Address of memory
   * @param uint32 size Size of memory in sizeof
   * @return void*
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
   * Remove memory
   *
   * @param void* addr Address of memory
   * @return bool
   */  
  bool Object::memRemove(void* addr)
  {
    return false; //TODO
  }
}
