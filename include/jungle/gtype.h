#ifndef __GTYPE_H__
#define __GTYPE_H__

#ifdef __GEXPORT
#define GAPI __declspec(dllexport)
#else
#define GAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

typedef unsigned char       byte;
typedef unsigned int        uint;
typedef unsigned int        ubool;
typedef unsigned long       ulong;
typedef void                *lpvoid;

#ifndef _INT8
#define _INT8
typedef signed char         int8;
#endif

#ifndef _UINT8
#define _UINT8
typedef unsigned char       uint8;
#endif

#ifndef _INT16
#define _INT16
typedef signed short        int16;
#endif

#ifndef _UINT16
#define _UINT16
typedef unsigned short      uint16;
#endif

#ifndef _INT32
#define _INT32
typedef int                 int32;
#endif

#ifndef _UINT32
#define _UINT32
typedef unsigned int        uint32;
#endif

// See the comment above about NSPR and 64-bit.
#if defined(__LP64__) && !defined(OS_MACOSX) && !defined(OS_OPENBSD)
typedef unsigned long uint64;
#else
typedef unsigned long long uint64;
#endif

#if !(defined(__GNUC__) && defined(__GNUC_MINOR__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))) && \
    !(defined(_MSC_VER) && (_MSC_VER >= 1600))
#ifndef nullptr
#define nullptr             (0)
#endif
#endif

#define make_uint16(a, b)   ((uint16)(((byte)((uint)(a) & 0xff)) | ((uint16)((byte)((uint)(b) & 0xff))) << 8))
#define make_uint32(a, b)   ((uint32)(((uint16)((uint)(a) & 0xffff)) | ((uint32)((uint16)((uint)(b) & 0xffff))) << 16))
#define lo_uint16(l)        ((uint16)((uint)(l) & 0xffff))
#define hi_uint16(l)        ((uint16)((uint)(l) >> 16))
#define lo_byte(w)          ((byte)((uint)(w) & 0xff))
#define hi_byte(w)          ((byte)((uint)(w) >> 8))

typedef struct gPoint
{
    int x;
    int y;
} gPoint;

typedef struct gSize
{
    int cx;
    int cy;
} gSize;

typedef struct gRect
{
    int left;
    int top;
    int right;
    int bottom;
} gRect;

typedef void* HGCANVAS;
typedef void* HGBITMAP;

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*__GTYPE_H__*/
