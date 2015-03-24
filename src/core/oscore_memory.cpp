/** 
 * Heap memory of system.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "oscore_memory.h"

namespace oscore
{
  /**
   * Allocate memory.
   *
   * @param size size in byte.
   * @return pointer to allocate memory or NULL, if no memory.
   */  
  void* Memory::alloc(uint32 size)
  {
    return Object::memAlloc(size);
  }

  /**
   * Free allocated memory.
   *
   * @param void* pointer of allocate memory.
   * @return zero if memory is free or error code.
   */  
  int32 Memory::free(void* prt)
  {
    return Object::memFree(prt);  
  }
}
