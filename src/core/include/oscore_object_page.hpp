/** 
 * Page of memory for system objects
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_OBJECT_PAGE_HPP_
#define OSCORE_OBJECT_PAGE_HPP_
namespace oscore
{
  //ObjectPage struct must be align to 8
  struct ObjectPage
  {
    private:

      uint32                    key_;
      ObjectPage*               prev_;
      ObjectPage*               next_;
      uint32                    tid_;    
      uint32                    attr_;    
      uint32                    size_;
      uint32                    temp_[2]; // Align to 8

      static const uint32       KEY_PAGE = 0x19811019;

      static struct List
      {
        ObjectPage*             first;
        ObjectPage*             last;    
      }                         page_;

      /**
       * Operator new.
       *
       * @param size unused.
       * @param prt  address of memory.
       * @return address of memory.
       */   
      void* operator new(uint32, void* ptr)
      {
        if((uint32)ptr & 0x7) ptr = (void*)( ((uint32)ptr & ~0x7) + 0x8 );
        return ptr;
      }

      /** 
       * Create page.
       *
       * @param addr address of page.
       * @param size size of page in sizeof.
       * @return pointer to ObjectPage.
       */
      static ObjectPage* init(void* addr, uint32 size)
      {
        ObjectPage* page = new (addr) ObjectPage;
        if(page->test(size) == false) return NULL;
        return page->reset(size)->link();
      }

      /**
       * Get first free data address.
       *
       * @return pointer to ObjectMemory.
       */
      ObjectMemory* data()
      {
        return (ObjectMemory*)((uint32)this + sizeof(ObjectPage));
      }


      /**
       * Test of memory page.
       *
       * @param size size in byte.
       * @return true if test complete.
       */
      bool test(uint32 size)
      {
        uint32 i;
        uint8* ptr;
        
        for( ptr=(uint8*)this, i=0; i<size; i++, ptr++ ) 
          (*ptr) = i;
        for( ptr=(uint8*)this, i=0; i<size; i++, ptr++ ) 
          if( (*ptr) != (uint8)( i & 0x000000ff ) ) 
            return false;
            
        memset(this, 0x55, size);
        ptr = (uint8*)this;
        for( uint32 i=0; i<size; i++, ptr++ ) 
          if( (*ptr) != 0x55 ) 
            return false;    
            
        memset(this, 0xaa, size);
        ptr = (uint8*)this;
        for( uint32 i=0; i<size; i++, ptr++ ) 
          if( (*ptr) != 0xaa )     
            return false;    
            
        memset(this, 0, size);
        return true;
      }

      /**
       * Reset page data struct.
       *
       * @return pointer to this object.
       */
      ObjectPage* reset(uint32 size)
      {
        key_ = KEY_PAGE;
        prev_ = NULL;
        next_ = NULL;
        tid_ = 0;    
        attr_ = 0;
        size_ = (size & ~0x7) - sizeof(ObjectPage);
        temp_[0] = 0xffffffff;
        temp_[1] = 0xffffffff;
        return this;  
      }

      /**
       * Linking new memory page into page list.
       *
       * @return pointer to this object.
       */
      ObjectPage* link()
      {
        if(page_.first == NULL) 
          page_.first = this;
        if(page_.last == NULL)
          this->prev_ = NULL;
        else
        {
          page_.last->next_ = this;
          prev_ = page_.last;
        }
        page_.last = this;
        next_ = NULL;
        return this;  
      }

      /**
       * Check available page.
       *
       * @return true if page is available.
       */  
      bool isMemory()
      {
        return (key_ == KEY_PAGE) ? true : false;
      }

      friend class              Object;
      friend struct             ObjectMemory;
  };

  ObjectPage::List ObjectPage::page_ = {NULL, NULL};
}
#endif //OSCORE_OBJECT_PAGE_HPP_
