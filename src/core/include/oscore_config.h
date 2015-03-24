/** 
 * Configuration of OS for TMS320c64xx
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
namespace oscore
{
  /**
   * Process switch time in micro seconds
   */
  static const uint32 SYS_QUANT = 1000;                                          
  /**
   * CPU frequency in Hz
   */
  static const uint64 SYS_FREQUENCY = 720000000ull;                              
  /**
   * Size of default stack
   */
  static const uint32 SYS_STACK_SIZE = 0x2000;           
  /**
   * Priority of main thread
   */
  static const uint32 SYS_MAIN_THREAD_PRIORITY = 5;
  /**
   * System stack TOS
   */
  static const struct {uint32 addr, size;} SYS_MEMORY[] = {
    {0x00090000, 0x00060000}
  };
}
