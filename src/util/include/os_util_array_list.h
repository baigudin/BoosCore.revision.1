/** 
 * Array list class
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_UTIL_ARRAY_LIST_H_
#define OS_UTIL_ARRAY_LIST_H_

#include "os_util_abstract_list.h"

namespace OsUtil
{
  class ArrayList : public AbstractList
  {
    public:

      using                     AbstractList::add;

                                ArrayList();
      virtual                  ~ArrayList();
      virtual bool              add(int32, OsCore::Object*);
      virtual void              clear();
      virtual bool              remove(OsCore::Object*);
      virtual bool              remove(int32);
      virtual OsCore::Object*   get(int32);
      virtual uint32            size();

    private:

      struct Item
      {
        Item*                     prev;
        Item*                     next;
        OsCore::Object*           obj;
        int32                     index;
      };

      Item*                       _tail;
      uint32                      _size;

      Item*                       _getItem(Object*);
      Item*                       _getItem(int32);
      bool                        _removeItem(Item*);
      bool                        _isIndex(int32);
      bool                        _isIndexOutOfBounds(int32);
  };
}
#endif //OS_UTIL_ARRAY_LIST_H_
