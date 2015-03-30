/** 
 * System driver interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSDRV_DRIVER_H_
#define OSDRV_DRIVER_H_

#include "oscore_object.h"

namespace osdrv
{
  class Driver
  {
    public:

      enum Command {CMD_LAST = 32};
      enum Error {OSE_LAST = oscore::Object::OSE_LAST};

      virtual                  ~Driver(){}
      virtual int32             close() = 0;
      virtual int32             control(int32, const void*) = 0;
      virtual int32             open(int32) = 0;
      virtual int32             read(void*, int32) = 0;
      virtual int32             write(void*, int32) = 0;
  };
}
#endif //OSDRV_DRIVER_H_
