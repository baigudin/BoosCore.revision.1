/**
 * Interface Runnable
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_RUNNABLE_H_
#define OS_CORE_RUNNABLE_H_

namespace OsCore
{
  class Runnable
  {
    public:

      virtual                  ~Runnable(){}
      virtual void              run() = 0;
  };
}
#endif //OS_CORE_RUNNABLE_H_
