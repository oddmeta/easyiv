#ifndef _RS_MACRO_H_
#define _RS_MACRO_H_

#define SAFE_DELETE(p)		{ if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p); (p)=NULL; } }
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_FREE(x)		{ if(NULL != (x)) { free((x));(x) = NULL ;}}

#define SAFE_COPY(dst, src, count)  \
	do                              \
{                                   \
	s16 i;                          \
	for (i = 0; i < (count); i ++)  \
	*((dst)+i) = *((src)+i);        \
	}while(0)

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/*
#ifdef WIN32
	#pragma comment( lib, "ws2_32.lib" ) 
	#pragma pack( push )
	#pragma pack( 1 )
	#define window( x )	x
	#ifndef PACKED
		#define PACKED
	#endif
#else
	#include <netinet/in.h>
	#define window( x )
	#if defined(__ETI_linux__)
		#define PACKED
	#else
		#ifdef PACKED
		#undef PACKED
		#endif
		#define PACKED __attribute__ ((packed))	// 取消编译器的优化对齐
	#endif
#endif
*/

#endif // _RS_MACRO_H_
