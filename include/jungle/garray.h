#ifndef __GARRAY_H__
#define __GARRAY_H__

#include "gobject.h"
#include "gdatatype.h"

class GAPI GArray : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GArray)

public:
    typedef struct DATA_ITEM
    {
        FieldTypeType  type;
        FieldValueType value;
    };

    GArray(void);
    virtual ~GArray(void);
    void addBool(bool value, int idx = -1);
    void addInt(int value, int idx = -1);
    void addUint(uint32 value, int idx = -1);
    void addInt64(int64 value, int idx = -1);
    void addUint64(uint64 value, int idx = -1);
    void addFloat(float value, int idx = -1);
    void addString(const char* value, int len = -1, int idx = -1);
    void addString(const wchar_t* value, int len = -1, int idx = -1);
    void addBuffer(const void* value, int len, int idx = -1);
    void addData(GData* value, int idx = -1);
    void addArray(GArray* value, int idx = -1);
    void addObject(GObject* value, int idx = -1);
    void addIntVector(int* value, int len, int idx = -1);
#ifdef GDATA_SUPPORT_COM
    void addInterface(IUnknown* value, int idx = -1);
    void addGuid(REFGUID value, int idx = -1);
#endif

    bool           getBool(int idx) const;
    int            getInt(int idx) const;
    uint32         getUint(int idx) const;
    int64          getInt64(int idx) const;
    uint64         getUint64(int idx) const;
    float          getFloat(int idx) const;
    const char*    getString(int idx) const;
    const wchar_t* getStringW(int idx) const;
    const void*    getBuffer(int idx, int* plen) const;
    GData*         getData(int idx) const;
    GArray*        getArray(int idx) const;
    GObject*       getObject(int idx) const;
    const int*     getIntVector(int idx, int* plen) const;
#ifdef GDATA_SUPPORT_COM
    IUnknown*      getInterface(int idx) const;
    GUID           getGuid(int idx) const;
#endif

    void setBool(int idx, bool value);
    void setInt(int idx, int value);
    void setUint(int idx, uint32 value);
    void setInt64(int idx, int64 value);
    void setUint64(int idx, uint64 value);
    void setFloat(int idx, float value);
    void setString(int idx, const char* value, int len = -1);
    void setString(int idx, const wchar_t* value, int len = -1);
    void setBuffer(int idx, const void* value, int len);
    void setData(int idx, GData* value);
    void setArray(int idx, GArray* value);
    void setObject(int idx, GObject* value);
    void setIntVector(int idx, int* value, int len);
#ifdef GDATA_SUPPORT_COM
    void setInterface(int idx, IUnknown* value);
    void setGuid(int idx, REFGUID value);
#endif

    void          clear(void);
    bool          reserve(int size);
    int           getFieldCount() const;
    FieldTypeType getFieldType(int index) const;

    int          serialize(byte* outBuf, int len) const;
    const byte*  unserialize(const byte* inbuf, int len);
protected:
    void addValue(int idx, FieldTypeType type, FieldValueType value);
    void setValue(int idx, FieldTypeType type, FieldValueType value);
    bool getValue(int idx, FieldTypeType& type, FieldValueType& value) const;

protected:
    int         m_fieldCount;
    int         m_allocCount;
    DATA_ITEM*  m_buffer;
};

#endif