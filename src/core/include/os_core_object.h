/** 
 * Object of OS
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_OBJECT_H_
#define OS_CORE_OBJECT_H_

#include "os_core_types.h"

namespace OsCore
{
  class ObjectPage;
  class ObjectMemory;

  class Object
  {
    public:

      enum Error
      {
        OSE_OK,
        OSE_ARG,      
        OSE_MEM,
        OSE_HW,
        OSE_ALLOC,
        OSE_RES,
        OSE_BUSY,
        OSE_INIT
      };

                                Object();
      virtual                  ~Object();
      virtual Error             error();
      void*                     operator new(uint32);
      void*                     operator new[](uint32);
      void*                     operator new(uint32,void*);
      void*                     operator new[](uint32,void*);
      void                      operator delete(void*);
      void                      operator delete[](void*);

    protected:

      Error                     _error(Error);
      static void*              _memAlloc(uint32);
      static int32              _memFree(void*);

    private:

      ObjectMemory*             _memory;
      Error                     _errno;

                                Object(const Object&);     //Copy not allowed
      void                      operator= (const Object&); //Assignment not allowed
      static bool               _init();

      friend class              System;
      friend struct             ObjectPage;
      friend struct             ObjectMemory;
  };
}
#endif //OS_CORE_OBJECT_H_
