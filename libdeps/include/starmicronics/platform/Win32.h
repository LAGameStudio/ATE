#ifndef win32__
#define win32__

#define _CRT_SECURE_NO_DEPRECATE

#ifdef _DEBUG

#define DEBUG

#endif

//#include <winsock2.h>
#include <windows.h>
#include <wtypes.h>

#pragma warning (disable: 4290)
#pragma warning (disable: 4251)

#define DLL_EXPORT __declspec(dllexport)
//#define DLL_IMPORT __declspec(dllimport)
#define DLL_IMPORT

#ifdef COMPILING_API
  #define STARMICRONICS_API _declspec(dllexport)
#else
  #define STARMICRONICS_API _declspec(dllimport)
#endif

#define CALL_CONVENT  WINAPI

#define SOURCE_ID __FUNCTION__

#include <limits.h>
#include <float.h>

#ifndef SM_BOOLEAN
  #define SM_BOOLEAN __int32
#endif
#define SM_TRUE ((__int32) (1 == 1))
#define SM_FALSE ((__int32) (0 > 1))

#ifndef UCHAR
  #define UCHAR  unsigned char
#endif
#define MAX_UCHAR ((UCHAR)UCHAR_MAX)

#ifndef UINT16
  #define UINT16 unsigned __int16
#endif
#define MAX_UINT16 ((UINT16)_UI16_MAX)

#ifndef UINT32
  #define UINT32 unsigned __int32
#endif
#define MAX_UINT32 ((UINT32)_UI32_MAX)

#ifndef UINT64
  #define UINT64 unsigned __int64
#endif
#define MAX_UINT64 ((UINT64)_UI64_MAX)

#ifndef __wtypes_h__
#ifndef CHAR
  #define CHAR   char
#endif
#endif
#define MIN_CHAR ((CHAR)SCHAR_MIN)
#define MAX_CHAR ((CHAR)SCHAR_MAX)

#ifndef INT16
  #define INT16  __int16
#endif
#define MIN_INT16 ((INT16)_I16_MIN)
#define MAX_INT16 ((INT16)_I16_MAX)

#ifndef INT32
  #define INT32  __int32
#endif
#define MIN_INT32 ((INT32)_I32_MIN)
#define MAX_INT32 ((INT32)_I32_MAX)

#ifndef INT64
  #define INT64  __int64
#endif
#define MIN_INT64 ((INT64)_I64_MIN)
#define MAX_INT64 ((INT64)_I64_MAX)

#ifndef FLOAT
  #define FLOAT  float
#endif
#define DIG_FLOAT FLT_MANT_DIG
#define MIN_FLOAT ((FLOAT)FLT_MIN)
#define MAX_FLOAT ((FLOAT)FLT_MAX)

#ifndef __wtypes_h__
#ifndef DOUBLE
  #define DOUBLE double
#endif
#endif
#define DIG_DOUBLE DBL_MANT_DIG
#define MIN_DOUBLE ((DOUBLE)DBL_MIN)
#define MAX_DOUBLE ((DOUBLE)DBL_MAX)

#define PLUGIN WinPlugin

#define SYMBOL_HANDLE FARPROC
#define PLUGIN_HANDLE HMODULE

#define PATH_SEPERATOR_CHAR '\\'
#define PATH_SEPERATOR_STRING "\\"

#define LF "\r\n"
#define W_LF L"\r\n"

#define atomicIncrement(uint32Ptr) ::InterlockedIncrement((long volatile *) uint32Ptr)
#define atomicDecrement(uint32Ptr) ::InterlockedDecrement((long volatile *) uint32Ptr)

#endif
