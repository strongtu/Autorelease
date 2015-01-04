#ifndef __GDATA__H__
#define __GDATA__H__

#include "gobject.h"

#define GDATA_SUPPORT_COM

#ifdef GDATA_SUPPORT_COM
#include <guiddef.h>
#include <unknwn.h>
#endif

class GData : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GData)

public:
    typedef char FieldTypeType;
    typedef void* FieldValueType;
    typedef const char* FieldNameType;

    enum GDATA_TYPE
    {
	    GDATA_TYPE_NONE		= 0,
	    GDATA_TYPE_BOOL		= 1,
	    GDATA_TYPE_INT		= 2,
        GDATA_TYPE_DOUBLE   = 4,
	    GDATA_TYPE_DATA		= 5,
	    GDATA_TYPE_ARRAY    = 6,
        GDATA_TYPE_OBJECT   = 7,
        GDATA_TYPE_STRING   = 8,
        GDATA_TYPE_STRINGW  = 9,
#ifdef GDATA_SUPPORT_COM
        GDATA_TYPE_GUID     = 10,
        GDATA_TYPE_INTERFACE= 11,
#endif
    };

public:
    GData(void);
    virtual ~GData(void);

    int  getFieldCount() const;
    FieldTypeType getFieldType(int index) const;
    FieldTypeType getFieldType(FieldNameType szKey) const;
	FieldNameType getFieldName(int index) const;

    void delField(int index);
    void delField(FieldNameType szKey);

    int  getDoc(char* outBuf, int len) const;
    bool setDoc(const char* outBuf, int len);
    void copy(const GData* p, bool overWrite = true);
    void copyField(const char* szKey, const GData* p);

    bool getBool(const char* szKey) const;
    int  getInt(const char* szKey) const;
    double getDouble(const char* szKey) const;
    const char* getString(const char* szKey) const;
    const wchar_t* getStringW(const char* szKey) const;
#ifdef GDATA_SUPPORT_COM
    GUID getGuid(const char* szKey) const;
    IUnknown* getInterface(const char* szKey) const;
#endif

    void setBool(const char* szKey, bool value);
    void setInt(const char* szKey, int value);
    void setDouble(const char* szKey, double value);
    void setString(const char* szKey, const char* str, int len = -1);
    void setString(const char* szKey, const wchar_t* str, int len = -1);
#ifdef GDATA_SUPPORT_COM
    void setGuid(const char* szKey, REFGUID value);
    void setInterface(const char* szKey, IUnknown* value);
#endif

    void clear(void);
    bool reserve(int size);

protected:
    void setValue(FieldNameType szKey, FieldTypeType type, FieldValueType value);
    bool getValue(const char* szKey, FieldTypeType& type, FieldValueType& value) const;
    void freeValue(FieldTypeType type, FieldValueType value);
    void freeItem(int index);

    void* allocBuffer(int size) const;
    void  freeBuffer(void* p) const;
    FieldNameType allocName(const char* szKey) const;
    void  freeName(FieldNameType name) const;

protected:
    typedef struct DATA_ITEM
    {
        FieldTypeType  type;
        FieldNameType  name;
        FieldValueType value;
    };

    int   m_fieldCount;
    int   m_allocCount;
    DATA_ITEM * m_buffer;
};

#endif