/** 
 * Abstract list class
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_UTIL_ABSTRACT_LIST_H_
#define OS_UTIL_ABSTRACT_LIST_H_

#include "os_core_object.h"
#include "os_util_list.h"

namespace OsUtil
{
  class AbstractList : public OsCore::Object, public OsUtil::List
  {
    public:

                                AbstractList();
      virtual                  ~AbstractList();
      virtual bool              add(OsCore::Object*);
      virtual bool              add(int32, OsCore::Object*);
      virtual void              clear();
      virtual bool              remove(OsCore::Object*);
      virtual bool              remove(int32);
      virtual OsCore::Object*   get(int32);
      virtual uint32            size();
      virtual OsCore::Object**  toArray();
  };
}
#endif //OS_UTIL_ABSTRACT_LIST_H_
