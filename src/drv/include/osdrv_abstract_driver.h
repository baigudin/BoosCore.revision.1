/** 
 * This class provides a skeletal implementation of the Driver interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSDRV_ABSTRACT_DRIVER_H_
#define OSDRV_ABSTRACT_DRIVER_H_

#include "osdrv_driver.h"
#include "oscore_resource.h"

namespace osdrv
{
  class AbstractDriver : public oscore::Resource, public Driver
  {
    public:

                                AbstractDriver();
      virtual                  ~AbstractDriver();
      virtual int32             close();
      virtual int32             control(int32, const void*);
      virtual int32             open(int32);
      virtual int32             read(void*, int32);
      virtual int32             write(void*, int32);

  };
}
#endif //OSDRV_ABSTRACT_DRIVER_H_
