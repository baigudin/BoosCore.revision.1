/** 
 * Array list class.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "osutil_array_list.h"

namespace osutil
{
  using namespace oscore;
  
  /** 
   * Constructor.
   */
  ArrayList::ArrayList()
  {
    prev_ = NULL;
    next_ = NULL;
    parent_ = NULL;
    child_ = NULL;
    obj_ = NULL;
    index_ = -1;
    size_ = 0;
  }

  /**
   * Destructor.
   */
  ArrayList::~ArrayList()
  {
    ArrayList *item;
    item = child_;
    while(item != NULL)
    {
      removeChild(item);
      item = child_;      
    }  
  }
  
  /**
   * Added element to the end of this list.
   *
   * @param obj pointer to any object of system.
   * @return true if element is added.
   */
  bool ArrayList::add(Object* obj)
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
  bool ArrayList::add(int32 index, Object* obj)
  {
    if(isIndexOutOfBounds(index) == true) return false;
    ArrayList* item = create();
    if(item == NULL) return false;
    item->prev_ = NULL;
    item->next_ = NULL;
    //Added to empty list:
    if(child_ == NULL)
    {
      item->next_ = item;
      item->prev_ = item;
      child_ = item;
    }
    //Added to tail of list:
    else if(index == size())
    {
      item->next_ = child_->next_;
      child_->next_ = item;
      item->prev_ = child_;
      item->next_->prev_ = item;      
      child_ = item;
    }
    else
    {
      ArrayList* temp = getChild(index);
      if(temp == NULL) 
      {
        delete item;
        return false;
      }
      item->next_ = temp;
      item->prev_ = temp->prev_;
      item->next_->prev_ = item;
      item->prev_->next_ = item;
      //Reindex list:
      while(true)
      {
        temp->index_++;
        if(temp == child_) break;
        temp = temp->next_;
      }
    }
    item->obj_ = obj;
    item->index_ = index;
    item->parent_ = this;    
    size_++;          
    return true;
  }
  
  /**
   * Removes all of the elements from this list.
   */  
  void ArrayList::clear()
  {
    int32 b = size()-1;
    for(int32 i=b; i>=0; i--) remove(i);  
  }

  /**
   * Remove element from this list.
   *
   * @param obj pointer to any object of system.
   * @return true if element is removed.
   */
  bool ArrayList::remove(Object* obj)
  {
    ArrayList* item = getChild(obj);
    if(item == NULL) return false;
    return removeChild(item);
  }
  
  /**
   * Remove element from this list.
   *
   * @param index position in this list.  
   * @return true if element is removed.
   */
  bool ArrayList::remove(int32 index)
  {
    ArrayList* item = getChild(index);
    if(item == NULL) return false;
    return removeChild(item);
  }  
  
  /**
   * Get element from this list by index.
   *
   * @param index position in this list.  
   * @return pointer to object of system in this list.
   */
  Object* ArrayList::get(int32 index)
  {
    ArrayList* item = getChild(index);
    if(item == NULL) return NULL;
    return item->obj_;
  }
  
  /**
   * Returns the number of elements in this list.
   *
   * @return number of elements.
   */
  int32 ArrayList::size()
  {
    return size_;
  }

  /**
   * Creating new object of this class.
   *
   * @return ArrayList*
   */  
  ArrayList* ArrayList::create()
  {
    return new ArrayList();
  } 
  
  /**
   * Get self element from list struct.
   *
   * @param obj pointer to any object of system.
   * @return pointer to private self element.
   */
  ArrayList* ArrayList::getChild(Object* obj)
  {
    if(obj == NULL) return NULL;
    if(child_ == NULL) return NULL;
    ArrayList* item = child_->next_;
    while(true)
    {
      if(item->obj_ == obj) return item;
      item = item->next_;
      if(item == child_->next_) break;
    }
    return NULL;
  }  
  
  /**
   * Get self element from list struct.
   *
   * @param index position in this list.  
   * @return pointer to private self element.
   */
  ArrayList* ArrayList::getChild(int32 index)
  {
    if(isIndex(index) == false) return NULL;
    ArrayList* item = child_->next_;
    for(int32 i=0; i<index; i++) item = item->next_;
    return item;
  } 
  
  
  /**
   * Remove self element from this list
   *
   * @param item pointer to private self element.
   * @return true if element is removed.
   */
  bool ArrayList::removeChild(ArrayList* item)
  {
    if(item == NULL) return false;
    //Reindex list:
    ArrayList* temp = item;
    while(temp != child_)
    {
      temp->next_->index_--;
      temp = temp->next_;
    }
    //Remove element:
    if(item->next_ == item && item->prev_ == item)
    {
      child_ = NULL;
    }
    else
    {
      item->next_->prev_ = item->prev_;
      item->prev_->next_ = item->next_;    
      if(child_ == item) child_ = item->prev_;
    }
    item->next_ = NULL;    
    item->prev_ = NULL;
    size_--;        
    delete item;
    return true;          
  }   
  
  
  /**
   * Check index is present in list
   *
   * @param index checked position in this list.
   * @return true if index is present
   */  
  bool ArrayList::isIndex(int32 index)
  {
    return (0 <= index && index < size()) ? true : false;
  }  
  
  /**
   * Check index is out of list bounds.
   *
   * @param index checked position in this list.
   * @return true if index is out.
   */  
  bool ArrayList::isIndexOutOfBounds(int32 index)
  {
    return (index < 0 || index > size()) ? true : false;
  }  
}
