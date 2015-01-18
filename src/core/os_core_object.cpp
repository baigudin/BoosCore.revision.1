/** 
 * Object of OS
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "rts.h"
#include "os_core_interrupt.h" 
#include "os_core_thread.h" 
#include "os_core_object.h"
#include "os_core_object_page.hpp"
#include "os_core_object_memory.hpp"

extern void *OS_MEMORY_BEGIN, *OS_MEMORY_END;

namespace OsCore
{
  /** 
   * Constructor
   */
  Object::Object()
  {
    _memory = (ObjectMemory*)((uint32)this - sizeof(ObjectMemory));
    _errno = OSE_OK;
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
   * @return enum Error
   */  
  Object::Error Object::error()
  {
    return _errno;
  }
  
  /**
   * Set error number
   *
   * @return enum Error
   */  
  Object::Error Object::_error(Object::Error errno)
  {
    _errno = errno;
    return error();
  }
  
  
  /**
   * Operator new
   *
   * @param uint32 size Size in byte
   * @return void* Allocated memory address or NULL, if error
   */
  void* Object::operator new(uint32 size)
  {
    return _memAlloc(size);
  }
  
  /**
   * Operator delete
   *
   * @param void* addr Address of allocated memory
   * @return void
   */
  void Object::operator delete(void* ptr)
  {
    _memFree(ptr);
  }
  
  /**
   * Operator new
   *
   * @param uint32     Unused
   * @param void*  prt Address of memory
   * @return void* Address of memory
   */
  void* Object::operator new(uint32, void* ptr)
  {
    return ptr;
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
    return ptr;
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
   * Allocate of memory
   *
   * @param uint32 size Size in byte
   * @return void* Allocated memory address or NULL, if error
   */   
  void* Object::_memAlloc(uint32 size)
  {
    return ObjectMemory::_create(size);  
  }

  /**
   * Free allocated memory
   *
   * @param void* ptr Address of allocated memory
   * @return int32 Error code or zero
   */   
  int32 Object::_memFree(void* ptr)
  {
    return ObjectMemory::_delete((Object*)ptr);
  }
  
  
  /**
   * Initialization
   *
   * @return int32 Error code or zero
   */
  bool Object::_init()
  {
    void* iPage = &OS_MEMORY_BEGIN;
    uint32 iSize = ((uint32)&OS_MEMORY_END-(uint32)&OS_MEMORY_BEGIN) & ~0x7;
    if(sizeof(ObjectPage) & 0x7) return false;
    if(sizeof(ObjectMemory) & 0x7) return false;    
    if(sizeof(ObjectPage) + sizeof(ObjectMemory) + 16 > iSize) return false;  
    uint32 is = Interrupt::disable();
    //Create page descriptor:
    ObjectPage* page = ObjectPage::_init(iPage, iSize);
    if(page == NULL) 
    {
      Interrupt::enable(is);
      return false;
    }
    //Create memory descriptor:
    ObjectMemory* mem = ObjectMemory::_init(page);
    if(mem == NULL) 
    {
      Interrupt::enable(is);
      return false;
    }    
    Interrupt::enable(is);
    return true;    
  }
}
