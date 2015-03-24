/** 
 * This class provides a skeletal implementation of the Driver interface.
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#include "oscore_abstract_driver.h"

namespace oscore
{
  /** 
   * Constructor.
   */
  AbstractDriver::AbstractDriver()
  {
  }

  /**
   * Destructor.
   */
  AbstractDriver::~AbstractDriver()
  {
  }
  
  /**
   * Open stream.
   *
   * @param access access to stream.
   * @return error code or zero.
   */
  int32 AbstractDriver::open(int32 access)
  {
    return OSE_HW;
  }
  
  /**
   * Close stream.
   *
   * @return error code or zero.
   */  
  int32 AbstractDriver::close()
  {
    return OSE_HW;  
  }

  /**
   * Stream read from device.
   *
   * @param buf pointer to memory.
   * @param size size in byte.
   * @return count of reading bytes or -1 if error.
   */  
  int32 AbstractDriver::read(void* buf, int32 size)
  {
    return -1;
  }
  
  /**
   * Stream write to device
   *
   * @param buf pointer to memory.
   * @param size size in byte.
   * @return count of writing bytes or -1 if error.
   */  
  int32 AbstractDriver::write(void* buf, int32 size)
  {
    return -1;
  }
  
  /**
   * Device control command.
   *
   * @param command control command for device.
   * @param arg pointer to any data.
   * @return error code or zero.
   */  
  int32 AbstractDriver::control(int32 command, const void* arg)
  {
    return OSE_HW;
  }
}
