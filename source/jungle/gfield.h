#ifndef __GFIELD_H__
#define __GFIELD_H__

#include "gtype.h"
#include "gdatatype.h"
#include "gobject.h"
#include "gdataserialize.h"

typedef struct data_string
{
    uint32 len;
    char   c[];
}data_string;

typedef struct data_wstring
{
    uint32  len;
    wchar_t wc[];
}data_wstring;

typedef struct int_vector
{
    uint32  len;
    int     i[];
}int_vector;

typedef struct buffer
{
    uint32  len;
    byte    by[];
}buffer;

inline data_string* newDataString(const char* str, int len)
{
    // we count length without \0
    if (len < 0) len = strlen(str);

    data_string* ps = static_cast<data_string*>(GObject::allocBuffer(sizeof(ps->len) + (len + 1) * sizeof(ps->c[0])));
    ps->len = len;

    memcpy(ps->c, str, len * sizeof(char));
    (ps->c)[len] = 0;

    return ps;
}

inline data_wstring* newDataString(const wchar_t* str, int len)
{
    if (len < 0) len = wcslen(str);

    data_wstring* pws = static_cast<data_wstring*>(GObject::allocBuffer(sizeof(pws->len) + (len + 1) * sizeof(pws->wc[0])));
    pws->len = len;

    memcpy(pws->wc, str, len * sizeof(wchar_t));
    (pws->wc)[len] = 0;

    return pws;
}

inline buffer* newBuffer(const void* value, int len)
{
    buffer* p = static_cast<buffer*>(GObject::allocBuffer(len + sizeof(p->len)));
    p->len = len;
    memcpy(p->by, value, len);
    return p;
}

inline int_vector* newIntVector(const int* value, int len)
{
    int_vector* p = static_cast<int_vector*>(GObject::allocBuffer(sizeof(p->len) + len * sizeof(int)));
    p->len = len;
    memcpy(p->i, value, len * sizeof(int));
    return p;
}

bool writeValue(GDataSerialize& ser, FieldTypeType type, FieldValueType value);

inline void freeValue(FieldTypeType type, FieldValueType value)
{
    if (value.pv == nullptr) return;
	switch(type)
	{
    case GDATA_TYPE_INT64:
    case GDATA_TYPE_UINT64:
        GObject::freeBuffer(value.pv);
        break;
    case GDATA_TYPE_STRING:
    case GDATA_TYPE_STRINGW:
    case GDATA_TYPE_BUFFER:
        GObject::freeBuffer(value.pv);
        break;
	case GDATA_TYPE_DATA:
    case GDATA_TYPE_ARRAY:
    case GDATA_TYPE_OBJECT:
        ((GObject*)value.pv)->release();
        break;
    case GDATA_TYPE_INTVECTOR:
        GObject::freeBuffer(value.pv);
        break;
#ifdef GDATA_SUPPORT_COM
    case GDATA_TYPE_GUID:
        GObject::freeBuffer(value.pv);
        break;
    case GDATA_TYPE_INTERFACE:
        static_cast<IUnknown*>(value.pv)->Release();
        break;
#endif
    default:
        break;
	}
    value.pv = nullptr;
}

inline int utf82utf16(const char* us, int uslen, wchar_t* ws, int wslen)
{
    return MultiByteToWideChar(CP_UTF8, 0, us, uslen, ws, wslen);
}

inline int utf162utf8(const wchar_t* ws, int wslen, char* us, int uslen)
{
    return WideCharToMultiByte(CP_UTF8, 0, ws, wslen, us, uslen, nullptr, nullptr);
}

#endif