/** 
 * Skeletal implementation of the List interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "osutil_abstract_list.h"

namespace osutil
{
  using namespace oscore;
  
  /** 
   * Constructor.
   */
  AbstractList::AbstractList()
  {
  }

  /**
   * Destructor.
   */
  AbstractList::~AbstractList()
  {
  }

  /**
   * Added element to the end of this list.
   *
   * @param obj pointer to any object of system.
   * @return true if element is added.
   */
  bool AbstractList::add(Object* obj)
  {
    return add( size(), obj);
  }
  
  /**
   * Inserts the specified element at the specified position in this list.
   *
   * @param index position in this list.  
   * @param obj pointer to any object of system.
   * @return true if element is inserted.
   */
  bool AbstractList::add(int32 index, Object* obj)
  {
    return false;
  }
  
  
  /**
   * Removes all of the elements from this list.
   */  
  void AbstractList::clear()
  {
  }
  
  /**
   * Remove element from this list.
   *
   * @param obj pointer to any object of system.
   * @return true if element is removed.
   */
  bool AbstractList::remove(Object* obj)
  {
    return false;
  }
  
  /**
   * Remove element from this list.
   *
   * @param index position in this list.  
   * @return true if element is removed.
   */
  bool AbstractList::remove(int32 index)
  {
    return false;
  }  
  
  /**
   * Get element from this list by index.
   *
   * @param index position in this list.  
   * @return pointer to object of system in this list.
   */
  Object* AbstractList::get(int32 index)
  {
    return NULL;
  }
  
  /**
   * Returns the number of elements in this list.
   *
   * @return number of elements.
   */
  int32 AbstractList::size()
  {
    return 0;
  }
  
  /**
   * Returns an array containing all links to the Objects of list.
   *
   * You must call <code>delete[]</code> for return value after it using.
   *
   * @return pointer to pointer of objects, or NULL if no elements.
   */  
  Object** AbstractList::toArray()
  {
    int32 size = this->size();
    if(size == 0) return NULL;
    Object** arr = new Object*[size];
    if(arr == NULL) return NULL;    
    for(int32 i=0; i<size; i++) arr[i] = get(i);
    return arr;
  }
}
