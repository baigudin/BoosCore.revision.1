/** 
 * Semaphore list class
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_SEMAPHORE_LIST_HPP_
#define OSCORE_SEMAPHORE_LIST_HPP_

#include "osutil_array_list.h"

namespace oscore
{
  class SemaphoreList : public osutil::ArrayList
  {
    public:

      /** 
       * Constructor
       */
      SemaphoreList() : osutil::ArrayList()
      {
        permits_ = -1;
      }

      /** 
       * Destructor
       */
      virtual ~SemaphoreList()
      {
      }

      /**
       * Get permits value
       *
       * @return bool
       */
      int32 getPermits(int32 index)
      {
        SemaphoreList* item = (SemaphoreList*)getChild(index);
        return item->permits_;
      }

      /**
       * Set permits value
       *
       * @return bool
       */
      void setPermits(int32 index, int32 permits)
      {
        SemaphoreList* item = (SemaphoreList*)getChild(index);
        item->permits_ = permits;
      }

    protected:

      /**
       * Creating new object
       *
       * @return ArrayList*
       */  
      virtual SemaphoreList* create()
      {
        return new SemaphoreList();
      }

    private:
     
      int32                     permits_;
  };
}
#endif //OSCORE_SEMAPHORE_LIST_HPP_
