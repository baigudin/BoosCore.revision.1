/** 
 * Memory module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_MEMORY_H_
#define OS_CORE_MEMORY_H_

#include "os_core_object.h"

namespace OsCore
{
  class Memory : public Object
  {
    public:

    static void*                alloc(uint32);
    static int32                free(void*);
  };
}

#endif //OS_CORE_MEMORY_H_
