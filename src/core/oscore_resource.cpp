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
  /** 
   * Constructor
   */
  Resource::Resource()
  {
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
