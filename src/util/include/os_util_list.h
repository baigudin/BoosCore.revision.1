/** 
 * Interface list class
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_UTIL_LIST_H_
#define OS_UTIL_LIST_H_

namespace OsUtil
{
  class List
  {
    public:

      virtual                  ~List(){}
      virtual bool              add(OsCore::Object*) = 0;
      virtual bool              add(int32, OsCore::Object*) = 0;
      virtual void              clear() = 0;
      virtual bool              remove(OsCore::Object*) = 0;
      virtual bool              remove(int32) = 0;
      virtual OsCore::Object*   get(int32) = 0;
      virtual uint32            size() = 0;
      virtual OsCore::Object**  toArray() = 0;
  };
}
#endif //OS_UTIL_LIST_H_
