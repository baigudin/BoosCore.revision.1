/** 
 * Configuration of OS for TMS320c64xx
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
namespace OsCore
{
  struct OsConfig
  {
    uint32 quant;         // Process switch time in micro seconds
    uint64 frequency;     // CPU frequency in Hz
  };
  const OsConfig osConfig = {1000, 720000000ull};
}
