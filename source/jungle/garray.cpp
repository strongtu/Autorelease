#include "stdafx.h"
#include "garray.h"
#include "gdata.h"
#include "gfield.h"

IMPLEMENT_GDYNAMIC_CLASS(GArray, GObject);

GArray::GArray(void)
: m_fieldCount(0)
, m_allocCount(0)
, m_buffer(NULL)
{
}

GArray::~GArray(void)
{
    clear();
}

void GArray::addBool(bool value, int idx)
{
    FieldValueType v = value ? (void*)1 : (void*)0;
    addValue(idx, GDATA_TYPE_BOOL, v);
}

void GArray::addInt(int value, int idx)
{
    FieldValueType v = (void*)(value);
    addValue(idx, GDATA_TYPE_INT, v);
}
void GArray::addUint(uint32 value, int idx)
{
    FieldValueType v = (void*)(value);
    addValue(idx, GDATA_TYPE_UINT, v);
}
void GArray::addInt64(int64 value, int idx)
{
    FieldValueType v = memcpy(allocBuffer(sizeof(int64)), &value, sizeof(int64));
    addValue(idx, GDATA_TYPE_INT64, v);
}
void GArray::addUint64(uint64 value, int idx)
{
    FieldValueType v = memcpy(allocBuffer(sizeof(uint64)), &value, sizeof(uint64));
    addValue(idx, GDATA_TYPE_UINT64, v);
}
void GArray::addFloat(float value, int idx)
{
    FieldValueType v;
    v.fv = value;
    addValue(idx, GDATA_TYPE_FLOAT, v);
}
void GArray::addString(const char* str, int len, int idx)
{
    FieldValueType value = newDataString(str, len);
    addValue(idx, GDATA_TYPE_STRING, value);
}
void GArray::addString(const wchar_t* str, int len, int idx)
{
    FieldValueType value = newDataString(str, len);
    addValue(idx, GDATA_TYPE_STRINGW, value);
}
void GArray::addBuffer(const void* value, int len, int idx)
{
    FieldValueType v = newBuffer(value, len);
    addValue(idx, GDATA_TYPE_BUFFER, v);
}
void GArray::addData(GData* value, int idx)
{
    if (value) value->retain();
    FieldValueType v = value;
    addValue(idx, GDATA_TYPE_DATA, v);
}
void GArray::addArray(GArray* value, int idx)
{
    if (value) value->retain();
    FieldValueType v = value;
    addValue(idx, GDATA_TYPE_ARRAY, v);
}
void GArray::addObject(GObject* value, int idx)
{
    if (value) value->retain();
    FieldValueType v = value;
    addValue(idx, GDATA_TYPE_OBJECT, v);
}
void GArray::addIntVector(int* value, int len, int idx)
{
    FieldValueType v = newIntVector(value, len);
    addValue(idx, GDATA_TYPE_INTVECTOR, v);
}
#ifdef GDATA_SUPPORT_COM
void GArray::addInterface(IUnknown* value, int idx)
{
    if (value) value->AddRef();
    FieldValueType v = value;
	addValue(idx, GDATA_TYPE_INTERFACE, v);
}
void GArray::addGuid(REFGUID value, int idx)
{
    FieldValueType v = (GUID*)memcpy(allocBuffer(sizeof(GUID)), &value, sizeof(GUID));
    addValue(idx, GDATA_TYPE_GUID, v);
}
#endif

bool GArray::getBool(int idx) const
{
    FieldTypeType type = GDATA_TYPE_BOOL;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? value.bv : false;
}

int GArray::getInt(int idx) const
{
    FieldTypeType type = GDATA_TYPE_INT;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? value.bv : false;
}

uint32 GArray::getUint(int idx) const
{
    FieldTypeType type = GDATA_TYPE_UINT;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? value.iv : 0;
}
int64 GArray::getInt64(int idx) const
{
    FieldTypeType type = GDATA_TYPE_INT64;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? value.uv : 0;
}
uint64 GArray::getUint64(int idx) const
{
    FieldTypeType type = GDATA_TYPE_UINT64;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? *value.pi64 : 0;
}
float GArray::getFloat(int idx) const
{
    FieldTypeType type = GDATA_TYPE_FLOAT;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? *value.pu64 : 0;
}
const char* GArray::getString(int idx) const
{
    FieldTypeType type = GDATA_TYPE_STRING;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? (value.ps->c) : nullptr;
}
const wchar_t* GArray::getStringW(int idx) const
{
    FieldTypeType type = GDATA_TYPE_STRINGW;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? (value.pws->wc) : nullptr;
}
const void* GArray::getBuffer(int idx, int* plen) const
{
    FieldTypeType type = GDATA_TYPE_BUFFER;
    FieldValueType value = nullptr;

    if (getValue(idx, type, value))
    {
        *plen = value.pb->len;
        return value.pb->by;
    }
    else
    {
        *plen = 0;
        return nullptr;
    }
}
GData* GArray::getData(int idx) const
{
    FieldTypeType type = GDATA_TYPE_DATA;
    FieldValueType value = nullptr;
    return getValue(idx, type, value) ? value.pdata : nullptr;
}
GArray* GArray::getArray(int idx) const
{
    FieldTypeType type = GDATA_TYPE_ARRAY;
    FieldValueType value = nullptr;
    return getValue(idx, type, value) ? value.parray : nullptr;
}
GObject* GArray::getObject(int idx) const
{
    FieldTypeType type = GDATA_TYPE_OBJECT;
    FieldValueType value = nullptr;
    return getValue(idx, type, value) ? static_cast<GObject*>(value.pv) : nullptr;

}
const int* GArray::getIntVector(int idx, int* plen) const
{
    FieldTypeType type = GDATA_TYPE_INTVECTOR;
    FieldValueType value = nullptr;

    if (getValue(idx, type, value))
    {
        *plen = value.piv->len;
        return value.piv->i;
    }
    else
    {
        *plen = 0;
        return nullptr;
    }
}
#ifdef GDATA_SUPPORT_COM
IUnknown* GArray::getInterface(int idx) const
{
    FieldTypeType type = GDATA_TYPE_INTERFACE;
    FieldValueType value = nullptr;

    return getValue(idx, type, value) ? static_cast<IUnknown*>(value.pv) : nullptr;
}
GUID GArray::getGuid(int idx) const
{
    FieldTypeType type = GDATA_TYPE_GUID;
    FieldValueType value = nullptr;

    GUID guid = {0};
    getValue(idx, type, value) ? memcpy(&guid, value.pv, sizeof(GUID)): 0;
    return guid;
}
#endif

void GArray::setBool(int idx, bool value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = value ? (void*)1 : (void*)0;
    setValue(idx, GDATA_TYPE_BOOL, v);
}

void GArray::setInt(int idx, int value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = (void*)(value);
    setValue(idx, GDATA_TYPE_INT, v);
}
void GArray::setUint(int idx, uint32 value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = (void*)(value);
    setValue(idx, GDATA_TYPE_UINT, v);
}
void GArray::setInt64(int idx, int64 value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = memcpy(allocBuffer(sizeof(int64)), &value, sizeof(int64));
    setValue(idx, GDATA_TYPE_INT64, v);
}
void GArray::setUint64(int idx, uint64 value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = memcpy(allocBuffer(sizeof(uint64)), &value, sizeof(uint64));
    setValue(idx, GDATA_TYPE_UINT64, v);
}
void GArray::setFloat(int idx, float value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v;
    v.fv = value;
    setValue(idx, GDATA_TYPE_FLOAT, v);
}
void GArray::setString(int idx, const char* str, int len)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType value = newDataString(str, len);
    setValue(idx, GDATA_TYPE_STRING, value);
}
void GArray::setString(int idx, const wchar_t* str, int len)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType value = newDataString(str, len);
    setValue(idx, GDATA_TYPE_STRINGW, value);
}
void GArray::setBuffer(int idx, const void* value, int len)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = newBuffer(value, len);
    setValue(idx, GDATA_TYPE_BUFFER, v);
}
void GArray::setData(int idx, GData* value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    if (value) value->retain();
    FieldValueType v = value;
    setValue(idx, GDATA_TYPE_DATA, v);
}
void GArray::setArray(int idx, GArray* value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    if (value) value->retain();
    FieldValueType v = value;
    setValue(idx, GDATA_TYPE_ARRAY, v);
}
void GArray::setObject(int idx, GObject* value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    if (value) value->retain();
    FieldValueType v = value;
    setValue(idx, GDATA_TYPE_OBJECT, v);
}
void GArray::setIntVector(int idx, int* value, int len)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = newIntVector(value, len);
    setValue(idx, GDATA_TYPE_INTVECTOR, v);
}
#ifdef GDATA_SUPPORT_COM
void GArray::setInterface(int idx, IUnknown* value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    if (value) value->AddRef();
    FieldValueType v = value;
	setValue(idx, GDATA_TYPE_INTERFACE, v);
}
void GArray::setGuid(int idx, REFGUID value)
{
    if (idx >= m_fieldCount || idx < 0) return;
    FieldValueType v = (GUID*)memcpy(allocBuffer(sizeof(GUID)), &value, sizeof(GUID));
    setValue(idx, GDATA_TYPE_GUID, v);
}
#endif

void GArray::clear(void)
{
    DATA_ITEM * p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        freeValue(p->type, p->value);
    }

    freeBuffer(m_buffer);
    m_buffer = NULL;
    m_fieldCount = 0;
    m_allocCount = 0;
}

bool GArray::reserve(int size)
{
    const int alloc_size = 8;

	if ((size - m_allocCount) <= 0)
	{
		return false;
	}

    int new_count = (((size - 1) / alloc_size) + 1) * alloc_size;
    int old_count = m_fieldCount;

    DATA_ITEM* new_buffer = (DATA_ITEM*)allocBuffer(sizeof(DATA_ITEM) * new_count);
    if (!new_buffer)
    {
        return false;
    }
    memset(new_buffer, 0, new_count * sizeof(DATA_ITEM));
    memcpy(new_buffer, m_buffer, old_count * sizeof(DATA_ITEM));

    freeBuffer(m_buffer);

    m_buffer = new_buffer;
    m_allocCount = new_count;

	return true;
}

int  GArray::getFieldCount() const
{
    return m_fieldCount;
}

FieldTypeType GArray::getFieldType(int index) const
{
    if (index < 0 || index >= m_fieldCount) return GDATA_TYPE_NONE;

    return m_buffer[index].type;
}

void GArray::addValue(int idx, FieldTypeType type, FieldValueType value)
{
    reserve(m_fieldCount + 1);

    if (idx < 0 || idx >= m_fieldCount)
    {
        m_buffer[m_fieldCount].type = type;
        m_buffer[m_fieldCount].value = value;
        m_fieldCount++;
    }
    else
    {
        memmove(&m_buffer[idx + 1], &m_buffer[idx], (m_fieldCount - idx) * sizeof(DATA_ITEM));
        m_buffer[idx].type = type;
        m_buffer[idx].value = value;
        m_fieldCount++;
    }
}

void GArray::setValue(int idx, FieldTypeType type, FieldValueType value)
{
    freeValue(m_buffer[idx].type, m_buffer[idx].value);
    m_buffer[idx].type = type;
    m_buffer[idx].value = value;
}

bool GArray::getValue(int idx, FieldTypeType& type, FieldValueType& value) const
{
    if (idx >= m_fieldCount || idx < 0) return false;
    
    if (type != GDATA_TYPE_NONE)
    {
        if (type == m_buffer[idx].type)
        {
            value = m_buffer[idx].value;
            return true;
        }
    }
    else
    {
        type = m_buffer[idx].type;
        value = m_buffer[idx].value;
        return true;
    }
    return false;
}
