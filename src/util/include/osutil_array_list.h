/** 
 * Array list class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSUTIL_ARRAY_LIST_H_
#define OSUTIL_ARRAY_LIST_H_

#include "osutil_abstract_list.h"

namespace osutil
{
  class ArrayList : public AbstractList
  {
    public:

                                ArrayList();
      virtual                  ~ArrayList();
      virtual bool              add(int32, oscore::Object*);
      virtual bool              add(oscore::Object*);
      virtual void              clear();
      virtual oscore::Object*   get(int32);
      virtual bool              remove(int32);
      virtual bool              remove(oscore::Object*);
      virtual int32             size();

    protected:                  

      virtual ArrayList*        create();
      ArrayList*                getChild(int32);
      ArrayList*                getChild(Object*);
      bool                      removeChild(ArrayList*);

    private:

      ArrayList*                prev_;
      ArrayList*                next_;
      ArrayList*                parent_;
      ArrayList*                child_;
      oscore::Object*           obj_;
      int32                     index_;
      int32                     size_;

      bool                      isIndex(int32);
      bool                      isIndexOutOfBounds(int32);
  };
}
#endif //OSUTIL_ARRAY_LIST_H_
