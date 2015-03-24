/**
 * Interrupt handler interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_INTERRUPT_HANDLER_H_
#define OSCORE_INTERRUPT_HANDLER_H_

namespace oscore
{
  class InterruptHandler
  {
    public:

      virtual                  ~InterruptHandler(){}
      virtual void              handle() = 0;
  };
}
#endif //OSCORE_INTERRUPT_HANDLER_H_
