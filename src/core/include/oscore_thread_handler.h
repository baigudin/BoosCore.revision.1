/**
 * Thread interrupt handler
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_THREAD_HANDLER_H_
#define OSCORE_THREAD_HANDLER_H_

#include "oscore_interrupt_handler.h"
#include "oscore_object.h"

namespace oscore
{
  class ThreadHandler : public Object, public InterruptHandler
  {
    public:

                                ThreadHandler();
      virtual                  ~ThreadHandler();
      virtual void              handle();
  };
}
#endif //OSCORE_THREAD_HANDLER_H_
