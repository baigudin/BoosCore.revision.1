/** 
 * Abstract list class
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "os_util_abstract_list.h"

namespace OsUtil
{
  using namespace OsCore;
  
  /** 
   * Constructor
   */
  AbstractList::AbstractList()
  {
  }

  /**
   * Destructor
   */
  AbstractList::~AbstractList()
  {
  }

  /**
   * Added element to the end of this list
   *
   * @param Object* obj
   * @return bool
   */
  bool AbstractList::add(Object* obj)
  {
    return add( size(), obj);
  }
  
  /**
   * Inserts the specified element at the specified position in this list
   *
   * @param int32 index   
   * @param Object* obj
   * @return bool
   */
  bool AbstractList::add(int32 index, Object* obj)
  {
    return false;
  }
  
  
  /**
   * Removes all of the elements from this list 
   *
   * @return void
   */  
  void AbstractList::clear()
  {
  }
  
  /**
   * Remove element from this list
   *
   * @param Object* obj
   * @return bool
   */
  bool AbstractList::remove(Object* obj)
  {
    return false;
  }
  
  /**
   * Remove element from this list
   *
   * @param int32 index
   * @return bool
   */
  bool AbstractList::remove(int32 index)
  {
    return false;
  }  
  
  /**
   * Get element from this list by index
   *
   * @param int32 index
   * @return Object*
   */
  Object* AbstractList::get(int32 index)
  {
    return NULL;
  }
  
  /**
   * Returns the number of elements in this list
   *
   * @return uint32
   */
  uint32 AbstractList::size()
  {
    return 0;
  }
  
  /**
   * Returns an array containing all links to the Objects
   *
   * You must call "delete[]" for return value after it using
   *
   * @return Object**
   */  
  Object** AbstractList::toArray()
  {
    uint32 size = this->size();
    if(size == 0) return NULL;
    Object** arr = new Object*[size];
    if(arr == NULL) return NULL;    
    for(int32 i=0; i<size; i++) arr[i] = get(i);
    return arr;
  }
}
