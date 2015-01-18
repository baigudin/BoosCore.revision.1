/** 
 * Interrupt module HW register map
 * 
 * @author Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#define DREG_MUXH               (volatile uint32*)0x019c0000
#define DREG_MUXL               (volatile uint32*)0x019c0004
#define DREG_EXTPOL             (volatile uint32*)0x019c0008

#define REG_MUXH                (*DREG_MUXH)
#define REG_MUXL                (*DREG_MUXL)
#define REG_EXTPOL              (*DREG_EXTPOL)
