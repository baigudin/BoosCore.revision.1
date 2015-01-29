/** 
 * Object of OS
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_OBJECT_H_
#define OSCORE_OBJECT_H_

#include "oscore_types.h"

namespace oscore
{
  struct ObjectPage;
  struct ObjectMemory;

  class Object
  {
    public:

      enum Error
      {
        OSE_OK = 0,
        OSE_ARG,      
        OSE_CMD,
        OSE_MEM,
        OSE_HW,
        OSE_ALLOC,
        OSE_RES,
        OSE_BUSY,
        OSE_INIT,
        OSE_LAST = 128
      };

                                Object();
      virtual                  ~Object();
      virtual const char*       getErrorString(int32);
      int32                     getError();
      bool                      onHeap();
      void*                     operator new(uint32);
      void*                     operator new[](uint32);
      void*                     operator new(uint32,void*);
      void*                     operator new[](uint32,void*);
      void                      operator delete(void*);
      void                      operator delete[](void*);

    protected:

      int32                     setError(int32);
      static void*              memAlloc(uint32);
      static bool               memFree(void*);
      static void*              memAdd(void*, uint32);
      static bool               memRemove(void*);

    private:

      ObjectMemory*             memory_;
      int32                     errno_;

                                Object(const Object&);     //Copy not allowed
      void                      operator= (const Object&); //Assignment not allowed
      static bool               init();
      static bool               deinit();

      friend class              System;
      friend struct             ObjectPage;
      friend struct             ObjectMemory;
  };
}
#endif //OSCORE_OBJECT_H_
