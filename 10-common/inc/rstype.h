
#ifndef _RSTYPE_H
#define _RSTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Type definition */

/* if vc version < vs2010, not fully support c99 */
#if defined(_MSC_VER) && _MSC_VER < 1600
/* "s8 = signed char" is the right choice, but our apps heavily use s8 as char,
   define as this will cause type conversion compile errors.
   So give up to set "s8 = char".

   This alias is fault in Linux ARM platform, where char == u8
*/
//typedef signed char		s8;
typedef char				s8;
typedef unsigned char		u8;
typedef signed short		s16;
typedef unsigned short		u16;
typedef signed int			s32;
typedef unsigned int		u32;
typedef __int64				s64;
typedef unsigned __int64	u64;

typedef long				l32;
typedef unsigned long		ul32;

#else //support c99
#include <stdint.h>
typedef char				s8;
/* see comments above */
//typedef int8_t			s8;
typedef uint8_t				u8;
typedef int16_t				s16;
typedef uint16_t			u16;
typedef int32_t				s32;
typedef uint32_t			u32;
typedef int64_t				s64;
typedef uint64_t			u64;

typedef long				l32;
typedef unsigned long		ul32;

#endif // end of _MSC_VER

typedef s32             BOOL32;
typedef void*			MemId;
typedef float           f32;
typedef double          f64;

#ifndef _MSC_VER
	#ifndef LPSTR
		#define LPSTR   char *
	#endif
	#ifndef LPCSTR
		#define LPCSTR  const s8 *
	#endif
#endif

#ifndef TRUE
	#define TRUE    1
#endif

#ifndef FALSE
	#define FALSE   0
#endif

#ifndef IN
	#define IN
	#define OUT
	#define INOUT
#endif

#ifndef RS_API
	#ifdef _MSC_VER
		#define RS_EXTERN_API extern "C"	__declspec(dllexport)
		#define RS_API						__declspec(dllexport)
	// for gcc
	#else
		#define RS_EXTERN_API extern "C"
		#define RS_API
	#endif
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _RSTYPE_H */
