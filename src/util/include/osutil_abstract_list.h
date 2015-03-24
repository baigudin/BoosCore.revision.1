/** 
 * Skeletal implementation of the List interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSUTIL_ABSTRACT_LIST_H_
#define OSUTIL_ABSTRACT_LIST_H_

#include "oscore_object.h"
#include "osutil_list.h"

namespace osutil
{
  class AbstractList : public oscore::Object, public osutil::List
  {
    public:

                                AbstractList();
      virtual                  ~AbstractList();
      virtual bool              add(int32, oscore::Object*);
      virtual bool              add(oscore::Object*);
      virtual void              clear();
      virtual oscore::Object*   get(int32);
      virtual bool              remove(int32);
      virtual bool              remove(oscore::Object*);
      virtual int32             size();
      virtual oscore::Object**  toArray();
  };
}
#endif //OSUTIL_ABSTRACT_LIST_H_
