/**
 * Semaphore module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_SEMAPHORE_H_
#define OSCORE_SEMAPHORE_H_

#include "oscore_resource.h"

namespace oscore
{
  class SemaphoreList;

  class Semaphore : public Resource
  {
    public:

                                Semaphore(int32);
                                Semaphore(int32, const char*);
                                Semaphore(int32, bool);
                                Semaphore(int32, bool, const char*);
      virtual                  ~Semaphore();
      virtual bool              isBlocked();
      void                      acquire();
      void                      acquire(int32);
      void                      release();
      void                      release(int32);

    private:

      struct List
      {
        SemaphoreList*          exec;
        SemaphoreList*          lock;
      };

      const char*               name_;
      int32                     count_;
      bool                      fair_;
      List                      list_;

      void                      construct(int32, bool, const char*);
      void                      destruct();
      void                      removeList(SemaphoreList*);
      bool                      isAlloc();
  };
}
#endif //OSCORE_SEMAPHORE_H_
