/** 
 * Abstarct Resource class of system.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "oscore_resource.h"

namespace oscore
{
  int32 Resource::idCount_ = 1;  
  
  /** 
   * Constructor.
   */
  Resource::Resource()
  {
    if(getError() != OSE_OK) return;
    id_ = (idCount_ > 0) ? idCount_++ : -1;    
    if(id_ <= 0) setError(OSE_ID);    
  }
  
  /** 
   * Destructor.
   */
  Resource::~Resource()
  {
  }
  
  /** 
   * Get resource identifier.
   *
   * @return this resource ID.
   */  
  int32 Resource::getId() const
  {
    return id_;
  }  

  /** 
   * Check resource is blocked.
   *
   * @return true if this resource is block.
   */  
  bool Resource::isBlocked()
  {
    return false;
  }
}
