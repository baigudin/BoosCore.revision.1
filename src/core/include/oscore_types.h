/** 
 * OS types
 * 
 * @author    Sergey Baigudin, baigudin@mail.ru
 * @copyright 2014-2015 Sergey Baigudin
 * @license   http://baigudin.com/license/
 * @link      http://baigudin.com
 */
#ifndef OSCORE_TYPES_H_
#define OSCORE_TYPES_H_

#ifndef __cplusplus
#  error "Please do not use C style sources with OS sources!"
#endif

#ifndef NULL
#  define NULL 0
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;

#endif //OSCORE_TYPES_H_
