/** 
 * Hardware Timer register map
 * 
 * @author    Sergey Baigudin <baigudin@mail.ru>
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#define BASE_TIM0                       (uint32*)0x01940000
#define BASE_TIM1                       (uint32*)0x01980000
#define BASE_TIM2                       (uint32*)0x01ac0000

#define REG_CTL0                        (uint32*)0x01940000
#define REG_PRD0                        (uint32*)0x01940004
#define REG_CNT0                        (uint32*)0x01940008

#define REG_CTL1                        (uint32*)0x01980000
#define REG_PRD1                        (uint32*)0x01980004
#define REG_CNT1                        (uint32*)0x01980008

#define REG_CTL2                        (uint32*)0x01ac0000
#define REG_PRD2                        (uint32*)0x01ac0004
#define REG_CNT2                        (uint32*)0x01ac0008

#define TCTL_FUNC                       0x00000001
#define TCTL_INVOUT                     0x00000002
#define TCTL_DATOUT                     0x00000004
#define TCTL_DATIN                      0x00000008
#define TCTL_PWID                       0x00000010
#define TCTL_GO                         0x00000040
#define TCTL_HLD                        0x00000080
#define TCTL_CP                         0x00000100
#define TCTL_CLKSRC                     0x00000200
#define TCTL_INVINP                     0x00000400
#define TCTL_TSTAT                      0x00000800
#define TCTL_SPND                       0x00008000
