/** 
 * Resource of OS module
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "os_core_resource.h"

namespace OsCore
{
  uint32 Resource::_idCount = 0;
  
  /** 
   * Constructor
   */
  Resource::Resource()
  {
    _id = ++_idCount;
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
}
