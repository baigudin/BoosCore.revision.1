/** 
 * Resource of OS module
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "oscore_resource.h"

namespace oscore
{
  int32 Resource::idCount_ = 0;
  
  /** 
   * Constructor
   */
  Resource::Resource()
  {
    id_ = ++idCount_;
  }
  
  /** 
   * Destructor
   */
  Resource::~Resource()
  {
  }

  /** 
   * Checking resource is blocked
   *
   * @return bool
   */  
  bool Resource::isBlocked()
  {
    return false;
  }
  
  /** 
   * Get resource ID
   *
   * @return int32
   */  
  int32 Resource::getId()
  {
    return id_;
  }  
}
