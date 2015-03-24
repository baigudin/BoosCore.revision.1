/**
 * Runnable interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_RUNNABLE_H_
#define OSCORE_RUNNABLE_H_

namespace oscore
{
  class Runnable
  {
    public:

      virtual                  ~Runnable(){}
      virtual void              run() = 0;
  };
}
#endif //OSCORE_RUNNABLE_H_
