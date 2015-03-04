/** 
 * Memory for system objects
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_OBJECT_MEMORY_HPP_
#define OSCORE_OBJECT_MEMORY_HPP_

#ifdef OS_DEBUG
#  define OS_MEMORY_DEBUG
#endif

namespace oscore
{
  //ObjectMemory struct must be align to 8
  struct ObjectMemory    
  {
    private:

      uint32                    key_;
      ObjectMemory*             prev_;
      ObjectMemory*             next_;
      ObjectPage*               page_;
      uint32                    tid_;
      uint32                    attr_;
      uint32                    isize_;
      uint32                    size_;

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
       * Initialization
       *
       * @param uint32  size Size of page in byte
       * @return ObjectMemory*
       */
      static ObjectMemory* init(ObjectPage* page)
      {
        ObjectMemory* mem = new (page->data()) ObjectMemory;
        mem->reset(page);
        mem->size_ = page->size_ - sizeof(ObjectMemory);
        return mem;
      }

      /** 
       * Dump allocated blocks
       *
       * @return void
       */
      static void dumpAllocated()
      {
        #ifdef OS_MEMORY_DEBUG
        char str[64];
        Object* obj;
        uint32 is = Interrupt::disable();
        std::cout << "-----------------\n";
        std::cout << "Allocated memory:\n";
        ObjectPage* page = ObjectPage::page_.first;    
        while(page != NULL)
        {
          ObjectMemory* mem = page->data();
          while(mem)
          {
            std::cout << ((mem->attr_ & ATTR_USED) ? "USED" : "FREE") <<" ";            
            sprintf(str, "0x%08X", (uint32)mem);
            std::cout << "Addr: " << (const char*)str <<"; ";
            sprintf(str, "0x%08X", mem->tid_);
            std::cout << "TID: " << (const char*)str <<"; ";
            sprintf(str, "0x%08X", mem->size_);
            std::cout << "Size: " << (const char*)str <<"; "; 
            sprintf(str, "0x%08X", mem->isize_);
            std::cout << "iSize: " << (const char*)str <<"; "; 
            if(mem->attr_ & ATTR_USED)
            {
              obj = mem->data();
              if(obj->memory_ == mem)
              {
                sprintf(str, "0x%08X", obj->getId());
                std::cout << "OID: " << (const char*)str <<"; "; 
              }
            }
            std::cout << "\n";
            mem = mem->next_;
          }
          page = page->next_;
        }
        Interrupt::enable(is);
        #endif
      }


      /**
       * Allocate object
       *
       * @param uint32 size Size in byte
       * @return Object* Allocated object   
       */  
      static Object* create(uint32 size)
      {
        Object* obj = NULL;
        uint32 is = Interrupt::disable();
        ObjectPage* page = ObjectPage::page_.first;    
        while(page != NULL)
        {
          obj = alloc(size, page);
          if(obj != NULL) break;
          page = page->next_;
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
      static Object* alloc(uint32 size, ObjectPage* page)
      {
        if(page == NULL) return NULL;
        if(size == 0) return NULL;    
        ObjectMemory *nmem, *cmem;
        uint32 isize = size;
        //Added to 32 bit word:
        if(size & 0x3) size = (size & ~0x3) + 0x4;
        //Align to 8:
        if(size & 0x4) size += 0x4;
        cmem = page->data();
        while(cmem)
        {
          if(cmem->attr_ & ATTR_USED)
          {
            cmem = cmem->next_;
            continue;
          }
          if(cmem->size_ < size)
          {
            cmem = cmem->next_;
            continue;
          }
          //We have memory block needed for size:
          if(cmem->size_ >= size + sizeof(ObjectMemory))
          {
            //We have memory block needed for size and new ObjectMemory:
            nmem = (ObjectMemory*)((uint32)cmem + sizeof(ObjectMemory) + size);
            nmem->reset(page);
            nmem->next_ = cmem->next_;
            nmem->prev_ = cmem;
            nmem->size_ = cmem->size_ - size - sizeof(ObjectMemory);
            if(nmem->next_) nmem->next_->prev_ = nmem;        
            cmem->next_ = nmem;        
          }
          cmem->tid_  = Thread::getCurrentThreadId();
          cmem->attr_|= ATTR_USED;      
          cmem->size_ = size;
          cmem->isize_ = isize;      
          return cmem->data();
        }
        return NULL;
      }

      /**
       * Free allocated memory
       *
       * @param void* obj Address of allocated object
       * @return bool
       */  
      static bool free(Object* obj)
      {
        ObjectMemory *cmem, *pmem, *nmem;
        bool nfree, pfree;
        uint32 is = Interrupt::disable();
        if(isDelete(obj) == false)
        {
          Interrupt::enable(is);
          return false;
        }
        cmem = (ObjectMemory*)((uint32)obj - sizeof(ObjectMemory));
        pmem = cmem->prev_;
        nmem = cmem->next_;
        pfree = (pmem != NULL && (pmem->attr_ & ATTR_USED) == 0) ? true : false;
        nfree = (nmem != NULL && (nmem->attr_ & ATTR_USED) == 0) ? true : false;    

        if(pfree && nfree)
        {
          pmem->size_ += 2*sizeof(ObjectMemory) + cmem->size_ + nmem->size_;
          pmem->next_ = nmem->next_;
          if(nmem->next_ != NULL) nmem->next_->prev_ = pmem;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)pmem+sizeof(ObjectMemory)), 0, pmem->size_);
          #endif
        }
        else if(pfree)
        {
          pmem->size_ += sizeof(ObjectMemory) + cmem->size_;
          pmem->next_ = nmem;
          if(nmem != NULL) nmem->prev_ = pmem;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)pmem+sizeof(ObjectMemory)), 0, pmem->size_);
          #endif      
        }
        else if(nfree)
        {
          cmem->size_ += sizeof(ObjectMemory) + nmem->size_;
          cmem->next_ = nmem->next_;
          if(nmem->next_ != NULL) nmem->next_->prev_ = cmem;
          cmem->tid_ = 0;
          cmem->attr_ = ATTR_INIT;
          cmem->isize_ = 0;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)cmem+sizeof(ObjectMemory)), 0, cmem->size_);
          #endif
        }
        else
        {
          cmem->tid_ = 0;
          cmem->attr_ = ATTR_INIT;
          cmem->isize_ = 0;
          #ifdef OS_MEMORY_DEBUG
          memset((void*)((uint32)cmem+sizeof(ObjectMemory)), 0, cmem->size_);
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
      static bool isDelete(Object* obj)
      {
        if(obj == NULL) return false;
        ObjectMemory* mem = (ObjectMemory*)((uint32)obj - sizeof(ObjectMemory));    
        if(mem->isMemory() == false) return false;
        if(mem->page_->isMemory() == false) return false;
        if(mem->tid_ != Thread::getCurrentThreadId()) return false;
        return true;
      }

      /**
       * Check available page
       *
       * @return bool
       */  
      bool isMemory()
      {
        return (key_ == KEY_MEM) ? true : false;
      }
      
      /**
       * Get first free data address
       *
       * @return Object*
       */
      Object* data()
      {
        return (Object*)((uint32)this + sizeof(ObjectMemory));
      }            

      /**
       * Initialization memory struct
       *
       * @param ObjectPage* page
       * @return this
       */
      ObjectMemory* reset(ObjectPage* page)
      {
        key_ = KEY_MEM;
        prev_ = NULL;
        next_ = NULL;
        page_ = page;    
        tid_ = 0;    
        attr_ = ATTR_INIT;                
        isize_ = 0;
        size_ = 0;
        return this;
      }

      friend class              Object;
  };
}
#endif //OSCORE_OBJECT_MEMORY_HPP_
