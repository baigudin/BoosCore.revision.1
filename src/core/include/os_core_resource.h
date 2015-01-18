/** 
 * Resource of OS module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_RESOURCE_H_
#define OS_CORE_RESOURCE_H_

#include "os_core_object.h"

namespace OsCore
{
  class Resource : public Object
  {
    public:

                                Resource();
      virtual                  ~Resource();
      virtual bool              isBlocked();

    protected:
  
      uint32                    _id;         //Resource ID

    private:
  
      static uint32             _idCount;
  };
}
#endif //OS_CORE_RESOURCE_H_
