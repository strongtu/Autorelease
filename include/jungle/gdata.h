#ifndef __GDATA__H__
#define __GDATA__H__

#include "gobject.h"
#include "gdatatype.h"

class GAPI GData : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GData)

public:
    typedef const char* FieldNameType;

    typedef struct DATA_ITEM
    {
        FieldTypeType  type;
        FieldNameType  name;
        FieldValueType value;
    };
public:
    GData(void);
    virtual ~GData(void);

    bool           getBool(const char* szKey) const;
    int            getInt(const char* szKey) const;
    uint32         getUint(const char* szKey) const;
    int64          getInt64(const char* szKey) const;
    uint64         getUint64(const char* szKey) const;
    float          getFloat(const char* szKey) const;
    const char*    getString(const char* szKey) const;
    const wchar_t* getStringW(const char* szKey) const;
    const void*    getBuffer(const char* szKey, int* plen) const;
    GData*         getData(const char* szKey) const;
    GArray*        getArray(const char* szKey) const;
    GObject*       getObject(const char* szKey) const;
    const int*     getIntVector(const char* szKey, int* plen) const;
#ifdef GDATA_SUPPORT_COM
    IUnknown*      getInterface(const char* szKey) const;
    GUID           getGuid(const char* szKey) const;
#endif

    void setBool(const char* szKey, bool value);
    void setInt(const char* szKey, int value);
    void setUint(const char* szKey, uint32 value);
    void setInt64(const char* szKey, int64 value);
    void setUint64(const char* szKey, uint64 value);
    void setFloat(const char* szKey, float value);
    void setString(const char* szKey, const char* value, int len = -1);
    void setString(const char* szKey, const wchar_t* value, int len = -1);
    void setBuffer(const char* szKey, const void* value, int len);
    void setData(const char* szKey, GData* value);
    void setArray(const char* szKey, GArray* value);
    void setObject(const char* szKey, GObject* value);
    void setIntVector(const char* szKey, const int* value, int len);
#ifdef GDATA_SUPPORT_COM
    void setInterface(const char* szKey, IUnknown* value);
    void setGuid(const char* szKey, REFGUID value);
#endif

    void          clear(void);
    bool          reserve(int size);
    int           getFieldCount() const;
    FieldTypeType getFieldType(int index) const;
    FieldTypeType getFieldType(FieldNameType szKey) const;
	FieldNameType getFieldName(int index) const;
    void          delField(int index);
    void          delField(FieldNameType szKey);

    int           serialize(byte* outBuf, int len) const;
    const byte*   unserialize(const byte* inbuf, int len);

protected:
    void setValue(FieldNameType szKey, FieldTypeType type, FieldValueType value, bool copyKey = true);
    bool getValue(const char* szKey, FieldTypeType& type, FieldValueType& value) const;
    void freeItem(int index);

    FieldNameType allocName(const char* szKey, int len = -1) const;
    void          freeName(FieldNameType name) const;

protected:
    int         m_fieldCount;
    int         m_allocCount;
    DATA_ITEM*  m_buffer;
};

#endif