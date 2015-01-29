/** 
 * Array list class
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSUTIL_TREE_LIST_H_
#define OSUTIL_TREE_LIST_H_

#include "osutil_abstract_list.h"

namespace osutil
{
  class TreeList : public AbstractList
  {
    public:

                                TreeList();
      virtual                  ~TreeList();
  };
}
#endif //OSUTIL_TREE_LIST_H_
