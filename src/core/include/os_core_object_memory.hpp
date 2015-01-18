/** 
 * Memory for system objects
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OS_CORE_OBJECT_MEMORY_HPP_
#define OS_CORE_OBJECT_MEMORY_HPP_

#ifdef OS_DEBUG
#  define OS_MEMORY_DEBUG
#endif

namespace OsCore
{
  //ObjectMemory struct must be align to 8
  struct ObjectMemory    
  {
    private:

      uint32                    _key;
      ObjectMemory*             _prev;
      ObjectMemory*             _next;
      ObjectPage*               _page;
      uint32                    _tid;
      uint32                    _attr;
      uint32                    _isize;
      uint32                    _size;

      static const uint32       KEY_MEM  = 0x19820401;
      static const uint32       ATTR_INIT = 0x00000001;
      static const uint32       ATTR_USED = 0x00008000;

      /**
       * Operator new
       */   
      void* operator new(uint32, void* ptr)
      {
        return ptr;
      }

      /** 
       * Create
       *
       * @param uint32  size Size of page in byte
       * @return ObjectMemory*
       */
      static ObjectMemory* _init(ObjectPage* page)
      {
        ObjectMemory* mem = new (page->_data()) ObjectMemory;
        mem->_reset(page);
        mem->_size = page->_size - sizeof(ObjectMemory);
        return mem;
      }

      /**
       * Allocate object
       *
       * @param uint32 size Size in byte
       * @return Object* Allocated object   
       */  
      static Object* _create(uint32 size)
      {
        Object* obj = NULL;
        uint32 is = Interrupt::disable();
        ObjectPage* page = ObjectPage::_page.first;    
        while(page != NULL)
        {
          obj = _alloc(size, page);
          if(obj != NULL) break;
          page = page->_next;
        }
        Interrupt::enable(is);
        return obj;  
      }  
      
      /**
       * Allocate object
       *
       * @param uint32      size Size in byte
       * @param ObjectPage* page Memory page address
       * @return Object* Allocated object   
       */  
      static Object* _alloc(uint32 size, ObjectPage* page)
      {
        if(page == NULL) return NULL;
        if(size == 0) return NULL;    
        ObjectMemory *nmem, *cmem;
        uint32 isize = size;
        //Added to 32 bit word:
        if(size & 0x3) size = (size & ~0x3) + 0x4;
        //Align to 8:
        if(size & 0x4) size += 0x4;
        cmem = page->_data();
        while(cmem)
        {
          if(cmem->_attr & ATTR_USED)
          {
            cmem = cmem->_next;
            continue;
          }
          if(cmem->_size < size)
          {
            cmem = cmem->_next;
            continue;
          }
          //We have memory block needed for size:
          if(cmem->_size >= size + sizeof(ObjectMemory))
          {
            //We have memory block needed for size and new ObjectMemory:
            nmem = (ObjectMemory*)((uint32)cmem + sizeof(ObjectMemory) + size);
            nmem->_reset(page);
            nmem->_next = cmem->_next;
            nmem->_prev = cmem;
            nmem->_size = cmem->_size - size - sizeof(ObjectMemory);
            if(nmem->_next) nmem->_next->_prev = nmem;        
            cmem->_next = nmem;        
          }
          cmem->_tid  = Thread::getCurrentThreadId();
          cmem->_attr|= ATTR_USED;      
          cmem->_size = size;
          cmem->_isize = isize;      
          return cmem->_data();
        }
        return NULL;
      }

      /**
       * Free allocated memory
       *
       * @param void* obj Address of allocated object
       * @return bool
       */  
      static int32 _delete(Object* obj)
      {
        ObjectMemory *cmem, *pmem, *nmem;    
        bool nfree, pfree;
        
        uint32 is = Interrupt::disable();
        if(_isMemory(obj) == false)
        {
          Interrupt::enable(is);
          return false;
        }
        cmem = (ObjectMemory*)((uint32)obj - sizeof(ObjectMemory));
        pmem = cmem->_prev;
        nmem = cmem->_next;
        pfree = (pmem != NULL && (pmem->_attr & ATTR_USED) == 0) ? true : false;
        nfree = (nmem != NULL && (nmem->_attr & ATTR_USED) == 0) ? true : false;    

        if(pfree && nfree)
        {
          pmem->_size += 2*sizeof(ObjectMemory) + cmem->_size + nmem->_size;
          pmem->_next = nmem->_next;
          if(nmem->_next != NULL) nmem->_next->_prev = pmem;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)pmem+sizeof(ObjectMemory)), 0, pmem->_size);
          #endif
        }
        else if(pfree)
        {
          pmem->_size += sizeof(ObjectMemory) + cmem->_size;
          pmem->_next = nmem;
          if(nmem != NULL) nmem->_prev = pmem;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)pmem+sizeof(ObjectMemory)), 0, pmem->_size);
          #endif      
        }
        else if(nfree)
        {
          cmem->_size += sizeof(ObjectMemory) + nmem->_size;
          cmem->_next = nmem->_next;
          if(nmem->_next != NULL) nmem->_next->_prev = cmem;
          cmem->_tid = 0;
          cmem->_attr = ATTR_INIT;
          cmem->_isize = 0;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)cmem+sizeof(ObjectMemory)), 0, cmem->_size);
          #endif
        }
        else
        {
          cmem->_tid = 0;
          cmem->_attr = ATTR_INIT;
          cmem->_isize = 0;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)cmem+sizeof(ObjectMemory)), 0, cmem->_size);
          #endif      
        }
        Interrupt::enable(is);
        return true;
      }  

      /**
       * Check available memory to free
       *
       * @param void* obj Address of allocated object
       * @return bool
       */  
      static bool _isMemory(Object* obj)
      {
        if(obj == NULL) return false;
        ObjectMemory* mem = (ObjectMemory*)((uint32)obj - sizeof(ObjectMemory));
        if(mem->_key != KEY_MEM) return false;
        if(mem->_page->_key != ObjectPage::KEY_PAGE) return false;
        if(mem->_tid != Thread::getCurrentThreadId()) return false;
        return true;
      }
      
      /**
       * Get first free data address
       *
       * @return Object*
       */
      Object* _data()
      {
        return (Object*)((uint32)this + sizeof(ObjectMemory));
      }            

      /**
       * Initialization memory struct
       *
       * @param ObjectPage* page
       * @return this
       */
      ObjectMemory* _reset(ObjectPage* page)
      {
        _key = KEY_MEM;
        _prev = NULL;
        _next = NULL;
        _page = page;    
        _tid = 0;    
        _attr = ATTR_INIT;                
        _isize = 0;
        _size = 0;
        return this;
      }

      friend class              Object;
  };
}
#endif //OS_CORE_OBJECT_MEMORY_HPP_
