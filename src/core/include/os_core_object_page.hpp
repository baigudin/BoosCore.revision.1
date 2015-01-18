/** 
 * Page of memory for system objects
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_OBJECT_PAGE_HPP_
#define OS_CORE_OBJECT_PAGE_HPP_
namespace OsCore
{
  //ObjectPage struct must be align to 8
  struct ObjectPage
  {
    private:

      uint32                    _key;
      ObjectPage*               _prev;
      ObjectPage*               _next;
      uint32                    _tid;    
      uint32                    _attr;    
      uint32                    _size;
      uint32                    _temp[2]; // Align to 8

      static const uint32       KEY_PAGE = 0x19811019;

      static struct List
      {
        ObjectPage*             first;
        ObjectPage*             last;    
      }                         _page;

      /**
       * Operator new
       */   
      void* operator new(uint32, void* ptr)
      {
        if((uint32)ptr & 0x7) ptr = (void*)( ((uint32)ptr & ~0x7) + 0x8 );
        return ptr;
      }

      /** 
       * Create page
       *
       * @param uint32  size Size of page in byte
       * @return ObjectPage*
       */
      static ObjectPage* _init(void* addr, uint32 size)
      {
        ObjectPage* page = new (addr) ObjectPage;
        if(page->_test(size) == false) return NULL;
        page->_key = KEY_PAGE;
        page->_prev = NULL;
        page->_next = NULL;
        page->_tid = 0;    
        page->_attr = 0;
        page->_size = (size & ~0x7) - sizeof(ObjectPage);
        page->_temp[0] = 0xffffffff;
        page->_temp[1] = 0xffffffff;
        page->_include();
        return page;
      }

      /**
       * Get first free data address
       *
       * @return ObjectMemory*
       */
      ObjectMemory* _data()
      {
        return (ObjectMemory*)((uint32)this + sizeof(ObjectPage));
      }


      /**
       * Test of memory page
       *
       * @param void*  page Start address test
       * @param uint32 size Size in byte
       * @return bool
       */
      bool _test(uint32 size)
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
       * Including new memory page into page list
       *
       * @return int32 Error code or zero
       */
      ObjectPage* _include()
      {
        if(_page.first == NULL) 
          _page.first = this;
        if(_page.last == NULL)
          this->_prev = NULL;
        else
        {
          _page.last->_next = this;
          this->_prev = _page.last;
        }
        _page.last = this;
        this->_next = NULL;
        return this;  
      }

      friend class              Object;
      friend class              ObjectMemory;
  };

  ObjectPage::List ObjectPage::_page = {NULL, NULL};
}
#endif //OS_CORE_OBJECT_PAGE_HPP_
