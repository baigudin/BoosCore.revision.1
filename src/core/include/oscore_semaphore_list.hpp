/** 
 * Semaphore list class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
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

      using osutil::ArrayList::get;
      using osutil::ArrayList::remove;

      /** 
       * Constructor.
       */
      SemaphoreList() : osutil::ArrayList()
      {
        permits_ = -1;
      }

      /** 
       * Destructor.
       */
      virtual ~SemaphoreList()
      {
      }

      /**
       * Push element to fifo.
       *
       * @param res pointer to Thread resource.
       * @param permits permits of semaphore
       */
      void push(Thread* res, int32 permits)
      {
        while(add(0, res) == false) Thread::yield();
        setPermits(0, permits);
      }

      /**
       * Get element from this list by index.
       *
       * @return pointer to Thread resource.
       */
      Thread* pop()
      {
        return (Thread*) get(size() - 1);
      }

      /**
       * Remove last element from this list.
       *
       * @return true if element is removed.
       */
      bool remove()
      {
        return remove(size() - 1);
      }


      /**
       * Get permits semaphore value.
       *
       * @return permits of semaphore.
       */
      int32 getPermits()
      {
        return getPermits(size() - 1);
      }


    private:

      /**
       * Get permits semaphore value.
       *
       * @return permits of semaphore.
       */
      int32 getPermits(int32 index)
      {
        SemaphoreList* item = (SemaphoreList*)getChild(index);
        return item->permits_;
      }

      /**
       * Set permits value
       *
       * @param index position in this list.
       * @param permits permits of semaphore
       */
      void setPermits(int32 index, int32 permits)
      {
        SemaphoreList* item = (SemaphoreList*)getChild(index);
        item->permits_ = permits;
      }

    protected:

      /**
       * Creating new object of this class.
       *
       * @return pointer to new list object.
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
