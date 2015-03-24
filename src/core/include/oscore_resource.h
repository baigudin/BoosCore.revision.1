/** 
 * Abstarct Resource class of system.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_RESOURCE_H_
#define OSCORE_RESOURCE_H_

#include "oscore_object.h"

namespace oscore
{
  class Resource : public Object
  {
    public:

                                Resource();
      virtual                  ~Resource();
      virtual bool              isBlocked();
      int32                     getId() const;

    private:

      int32                     id_;
      static int32              idCount_;
  };
}
#endif //OSCORE_RESOURCE_H_
