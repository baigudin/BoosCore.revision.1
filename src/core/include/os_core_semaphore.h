/**
 * Semaphore module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_SEMAPHORE_H_
#define OS_CORE_SEMAPHORE_H_

#include "os_core_resource.h"

namespace OsCore
{
  class Semaphore : public Resource
  {
    public:

                                Semaphore(uint32);
                                Semaphore(uint32, bool);
      virtual                  ~Semaphore();
      virtual bool              isBlocked();
      void                      acquire();
      void                      acquire(uint32 permits);
      void                      release();
      void                      release(uint32 permits);

    protected: 

    private:

      int32                     _count;
      bool                      _fair;
      struct Fifo
      {
      }                         _fifo;

      void                      _construct(uint32, bool);

      friend class              System;
  };
}
#endif //OS_CORE_SEMAPHORE_H_
