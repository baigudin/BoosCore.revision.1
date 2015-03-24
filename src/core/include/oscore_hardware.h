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
  struct Register 
  { 
    uint32                      a0,a1,b0,b1,a2,a3,b2,b3,a4,a5,b4,b5,a6,a7,
                                b6,b7,a8,a9,b8,b9,a10,a11,b10,b11,a12,a13,
                                b12,b13,a14,a15,b14,b15,a16,a17,b16,b17,
                                a18,a19,b18,b19,a20,a21,b20,b21,a22,a23,
                                b22,b23,a24,a25,b24,b25,a26,a27,b26,b27,
                                a28,a29,b28,b29,a30,a31,b30,b31,amr,irp; 
  };
}
