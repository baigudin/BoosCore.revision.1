/** 
 * Interface list class
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSUTIL_LIST_H_
#define OSUTIL_LIST_H_

namespace osutil
{
  class List
  {
    public:

      virtual                  ~List(){}
      virtual bool              add(oscore::Object*) = 0;
      virtual bool              add(int32, oscore::Object*) = 0;
      virtual void              clear() = 0;
      virtual bool              remove(oscore::Object*) = 0;
      virtual bool              remove(int32) = 0;
      virtual oscore::Object*   get(int32) = 0;
      virtual int32             size() = 0;
      virtual oscore::Object**  toArray() = 0;
  };
}
#endif //OSUTIL_LIST_H_
