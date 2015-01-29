/** 
 * Abstract driver class
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "oscore_abstract_driver.h"

namespace oscore
{
  /** 
   * Constructor
   */
  AbstractDriver::AbstractDriver()
  {
  }

  /**
   * Destructor
   */
  AbstractDriver::~AbstractDriver()
  {
  }
  
  /**
   * Open stream
   *
   * @param int32 access
   * @return int32 Error code or zero
   */
  int32 AbstractDriver::open(int32 access)
  {
    return OSE_HW;
  }
  
  /**
   * Close stream
   *
   * @return int32 Error code or zero
   */  
  int32 AbstractDriver::close()
  {
    return OSE_HW;  
  }

  /**
   * Stream read from device
   *
   * @param void* buf 
   * @param int32 size
   * @return int32
   */  
  int32 AbstractDriver::read(void* buf, int32 size)
  {
    return -1;
  }
  
  /**
   * Stream write to device
   *
   * @param void* buf 
   * @param int32 size    
   * @return int32
   */  
  int32 AbstractDriver::write(void* buf, int32 size)
  {
    return -1;
  }
  
  /**
   * Device control command
   *
   * @param int32 command
   * @param void*  arg   
   * @return int32 Error code or zero
   */  
  int32 AbstractDriver::control(int32 command, const void* arg)
  {
    return OSE_HW;
  }
}
