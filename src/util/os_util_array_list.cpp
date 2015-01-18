/** 
 * Array list class
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "os_util_array_list.h"

namespace OsUtil
{
  using namespace OsCore;
  
  /** 
   * Constructor
   */
  ArrayList::ArrayList()
  {
    _tail = NULL;
    _size = 0;
  }

  /**
   * Destructor
   */
  ArrayList::~ArrayList()
  {
    clear();
  }
  
  /**
   * Inserts the specified element at the specified position in this list
   *
   * @param int32 index   
   * @param Object* obj
   * @return bool
   */
  bool ArrayList::add(int32 index, Object* obj)
  {
    if(_isIndexOutOfBounds(index) == true) return false;
    Item* item = new Item;    
    if(item == NULL) return false;
    item->prev = NULL;
    item->next = NULL;
    item->obj = obj;
    item->index = index;
    //Added to empty list:
    if(_tail == NULL)
    {
      item->next = item;
      item->prev = item;
      _tail = item;
    }
    //Added to tail of list:
    else if(index == _size)
    {
      item->next = _tail->next;
      _tail->next = item;
      item->prev = _tail;
      item->next->prev = item;      
      _tail = item;
    }
    else
    {
      Item* temp = _getItem(index);
      if(temp == NULL) 
	  {
	    delete item;
        return false;
      }
      item->next = temp;
      item->prev = temp->prev;
      item->next->prev = item;
      item->prev->next = item;
      //Reindex list:
      while(true)
      {
        temp->index++;
		if(temp == _tail) break;
        temp = temp->next;
      }
    }
    _size++;          
    return true;
  }
  
  /**
   * Removes all of the elements from this list 
   *
   * @return void
   */  
  void ArrayList::clear()
  {
    for(int32 i=_size-1; i>=0; i--) remove(i);
  }

  /**
   * Remove element from this list
   *
   * @param Object* obj
   * @return bool
   */
  bool ArrayList::remove(Object* obj)
  {
    Item* item = _getItem(obj);
    return _removeItem(item);
  }
  
  /**
   * Remove element from this list
   *
   * @param int32 index
   * @return bool
   */
  bool ArrayList::remove(int32 index)
  {
    Item* item = _getItem(index);
    return _removeItem(item);
  }  
  
  /**
   * Get element from this list by index
   *
   * @param int32 index
   * @return Object*
   */
  Object* ArrayList::get(int32 index)
  {
    Item* item = _getItem(index);
    if(item == NULL) return NULL;
    return item->obj;
  }
  
  /**
   * Returns the number of elements in this list
   *
   * @return uint32
   */
  uint32 ArrayList::size()
  {
    return _size;
  }
  
  /**
   * Get item
   *
   * @param Object* obj 
   * @return Item*
   */
  ArrayList::Item* ArrayList::_getItem(Object* obj)
  {
    if(obj == NULL) return NULL;
    if(_tail == NULL) return NULL;
    Item* item = _tail->next;
    while(true)
    {
      if(item->obj == obj) return item;
      item = item->next;
      if(item == _tail->next) break;
    }
    return NULL;
  }
  
  /**
   * Get item
   *
   * @param int32
   * @return Item*
   */
  ArrayList::Item* ArrayList::_getItem(int32 index)
  {
    if(_isIndex(index) == false) return NULL;
    Item* item = _tail->next;
    for(int32 i=0; i<index; i++) item = item->next;
    return item;
  }
  
  /**
   * Remove element from this list
   *
   * @param Object* obj
   * @return bool
   */
  bool ArrayList::_removeItem(Item* item)
  {
    if(item == NULL) return false;
    //Reindex list:
    Item* temp = item;
    while(temp != _tail)
    {
      temp->next->index--;
      temp = temp->next;
    }
    //Remove element:
    if(item->next == item && item->prev == item)
    {
      _tail = NULL;
    }
    else
    {
      item->next->prev = item->prev;
      item->prev->next = item->next;    
      if(_tail == item) _tail = item->prev;
    }
    item->next = NULL;    
    item->prev = NULL;
    delete item;
    _size--;    
    return true;    
  }
  
  /**
   * Check index
   *
   * @param int32
   * @return bool
   */  
  bool ArrayList::_isIndex(int32 index)
  {
    return (0 <= index && index < size()) ? true : false;
  }
  
  /**
   * Check index
   *
   * @param int32
   * @return bool
   */  
  bool ArrayList::_isIndexOutOfBounds(int32 index)
  {
    return (index < 0 || index > size()) ? true : false;
  }
}
