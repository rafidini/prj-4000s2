/* typedef unsigned char uint8_t; */
/* typedef char int8_t; */
/* typedef unsigned short uint16_t; */
/* typedef short int16_t; */
/* typedef unsigned int uint32_t; */
/* typedef int int32_t; */

#ifndef  INT8_MIN
#  define INT8_MIN	(-INT8_MAX - 1)
#endif
#ifndef  INT8_MAX
#  define INT8_MAX	127
#endif
#ifndef  UINT8_MAX
#  define UINT8_MAX	255
#endif
#ifndef  INT16_MIN
#  define INT16_MIN	(-32768)
#endif
#ifndef  INT16_MAX
#  define INT16_MAX	32767
#endif
#ifndef  UINT16_MAX
#  define UINT16_MAX	65535
#endif
#ifndef  INT32_MIN
#  define INT32_MIN	(-INT32_MAX - 1)
#endif
#ifndef  INT32_MAX
#  define INT32_MAX	2147483647
#endif
#ifndef  UINT32_MAX
#  define UINT32_MAX	4294967295U
#endif
#ifndef  INT64_MAX
#  define INT64_MAX	9223372036854775807L
#endif
#ifndef  INT64_MIN
#  define INT64_MIN	(-INT64_MAX - 1L)
#endif
#ifndef  UINT64_MAX
#  define UINT64_MAX	18446744073709551615UL
#endif
#ifndef  FLOAT_MIN
#  define FLOAT_MIN     1.17549435e-38
#endif
#ifndef  FLOAT_MAX
#  define FLOAT_MAX     3.40282347e+38
#endif
#ifndef  DOUBLE_MIN
#  define DOUBLE_MIN    2.2250738585072014e-308
#endif
#ifndef  DOUBLE_MAX
#  define DOUBLE_MAX    1.7976931348623157e+308       
#endif
