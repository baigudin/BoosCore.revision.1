/**
 * Semaphore class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
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
                                Semaphore(int32, bool);
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

      int32                     count_;
      bool                      fair_;
      List                      list_;

      void                      construct(int32, bool);
      void                      removeList(SemaphoreList*);
  };
}
#endif //OSCORE_SEMAPHORE_H_
