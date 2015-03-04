/**
 * Object initializer
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_INITIALIZER_H_
#define OS_CORE_INITIALIZER_H_

namespace OsCore
{
  class Initializer
  {
    public:

      virtual                  ~Initializer(){}

      virtual void              _init() = 0;
      virtual void              _deinit() = 0;

    protected:

  };
}
#endif //OS_CORE_INITIALIZER_H_
