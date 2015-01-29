/** 
 * Memory module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_MEMORY_H_
#define OSCORE_MEMORY_H_

#include "oscore_object.h"

namespace oscore
{
  class Memory : public Object
  {
    public:

      static void*              alloc(uint32);
      static int32              free(void*);
  };
}

#endif //OSCORE_MEMORY_H_
