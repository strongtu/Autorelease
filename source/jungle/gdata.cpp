#include "stdafx.h"
#include "gdata.h"
#include "garray.h"
#include "tcmalloc\tcmalloc.h"

IMPLEMENT_GDYNAMIC_CLASS(GData, GObject);

GData::GData(void)
: m_fieldCount(0)
, m_allocCount(0)
, m_buffer(NULL)
{

}

GData::~GData(void)
{
    clear();
}

int GData::getFieldCount() const
{
    return m_fieldCount;
}

bool GData::getBool(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_BOOL;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? value.bv : false;
}

int GData::getInt(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_INT;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? value.iv : 0;
}

uint32 GData::getUint(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_UINT;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? value.uv : 0;
}

int64 GData::getInt64(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_INT64;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? *value.pi64 : 0;
}

uint64 GData::getUint64(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_UINT64;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? *value.pu64 : 0;
}

float GData::getFloat(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_FLOAT;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? value.fv : 0;
}

const char* GData::getString(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_STRING;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? (value.ps->c) : nullptr;
}

const wchar_t* GData::getStringW(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_STRINGW;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? (value.pws->wc) : nullptr;
}

const void* GData::getBuffer(const char* szKey, int* plen) const
{
    FieldTypeType type = GDATA_TYPE_BUFFER;
    FieldValueType value = nullptr;

    if (getValue(szKey, type, value))
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

GData* GData::getData(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_DATA;
    FieldValueType value = nullptr;
    return getValue(szKey, type, value) ? value.pdata : nullptr;
}

GArray* GData::getArray(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_ARRAY;
    FieldValueType value = nullptr;
    return getValue(szKey, type, value) ? value.parray : nullptr;
}

GObject* GData::getObject(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_OBJECT;
    FieldValueType value = nullptr;
    return getValue(szKey, type, value) ? static_cast<GObject*>(value.pv) : nullptr;
}

const int* GData::getIntVector(const char* szKey, int* plen) const
{
    FieldTypeType type = GDATA_TYPE_INTVECTOR;
    FieldValueType value = nullptr;

    if (getValue(szKey, type, value))
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
GUID GData::getGuid(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_GUID;
    FieldValueType value = nullptr;

    GUID guid = {0};
    getValue(szKey, type, value) ? memcpy(&guid, value.pv, sizeof(GUID)): 0;
    return guid;
}

IUnknown* GData::getInterface(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_INTERFACE;
    FieldValueType value = nullptr;

    return getValue(szKey, type, value) ? static_cast<IUnknown*>(value.pv) : nullptr;
}
#endif

void GData::setBool(const char* szKey, bool value)
{
    FieldValueType v = value ? (void*)1 : (void*)0;
    setValue(szKey, GDATA_TYPE_BOOL, v);
}

void GData::setInt(const char* szKey, int value)
{
    FieldValueType v = (void*)(value);
    setValue(szKey, GDATA_TYPE_INT, v);
}

void GData::setUint(const char* szKey, uint32 value)
{
    FieldValueType v = (void*)(value);
    setValue(szKey, GDATA_TYPE_UINT, v);
}

void GData::setInt64(const char* szKey, int64 value)
{
    FieldValueType v = memcpy(allocBuffer(sizeof(int64)), &value, sizeof(int64));
    setValue(szKey, GDATA_TYPE_INT64, v);
}

void GData::setUint64(const char* szKey, uint64 value)
{
    FieldValueType v = memcpy(allocBuffer(sizeof(uint64)), &value, sizeof(uint64));
    setValue(szKey, GDATA_TYPE_UINT64, v);
}

void GData::setFloat(const char* szKey, float value)
{
    FieldValueType v;
    v.fv = value;
    setValue(szKey, GDATA_TYPE_FLOAT, v);
}

void GData::setString(const char* szKey, const char* str, int len)
{
    // we count length without \0
    if (len < 0) len = strlen(str);

    FieldValueType value;
    value.ps = static_cast<data_string*>(allocBuffer(sizeof(value.ps->len) + (len + 1) * sizeof(value.ps->c[0])));
    value.ps->len = len;

    memcpy(value.ps->c, str, len * sizeof(char));
    ((value.ps->c))[len] = 0;

    setValue(szKey, GDATA_TYPE_STRING, value);
}

void GData::setString(const char* szKey, const wchar_t* str, int len)
{
    // we count length without \0
    if (len < 0) len = wcslen(str);

    FieldValueType value;
    value.pws = static_cast<data_wstring*>(allocBuffer(sizeof(value.pws->len) + (len + 1) * sizeof(value.pws->wc[0])));
    value.pws->len = len;

    memcpy(value.pws->wc, str, len * sizeof(wchar_t));
    ((value.pws->wc))[len] = 0;

    setValue(szKey, GDATA_TYPE_STRINGW, value);
}

void GData::setBuffer(const char* szKey, const void* value, int len)
{
    buffer* p = static_cast<buffer*>(allocBuffer(len + sizeof(p->len)));
    p->len = len;
    memcpy(p->by, value, len);
    
    FieldValueType v = p;
    setValue(szKey, GDATA_TYPE_BUFFER, v);
}

void GData::setData(const char* szKey, GData* value)
{
    if (value) value->retain();
    FieldValueType v = value;
    setValue(szKey, GDATA_TYPE_DATA, v);
}

void GData::setArray(const char* szKey, GArray* value)
{
    if (value) value->retain();
    FieldValueType v = value;
    setValue(szKey, GDATA_TYPE_ARRAY, v);
}

void GData::setObject(const char* szKey, GObject* value)
{
    if (value) value->retain();
    FieldValueType v = value;
    setValue(szKey, GDATA_TYPE_OBJECT, v);
}

void GData::setIntVector(const char* szKey, int* value, int len)
{
    int_vector* p = static_cast<int_vector*>(allocBuffer(sizeof(p->len) + len * sizeof(int)));
    p->len = len;
    memcpy(p->i, value, len * sizeof(int));
    FieldValueType v = p;
    setValue(szKey, GDATA_TYPE_INTVECTOR, v);
}

#ifdef GDATA_SUPPORT_COM
void GData::setInterface(const char* szKey, IUnknown* value)
{
    if (value) value->AddRef();
    FieldValueType v;
    v.pv = value;

	setValue(szKey, GDATA_TYPE_GUID, v);
}

void GData::setGuid(const char* szKey, REFGUID value)
{
    FieldValueType v;
    v.pv = (GUID*)memcpy(allocBuffer(sizeof(GUID)), &value, sizeof(GUID));

    setValue(szKey, GDATA_TYPE_GUID, v);
}
#endif

void GData::clear(void)
{
    DATA_ITEM * p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        freeValue(p->type, p->value);
        freeName(p->name);
    }

    freeBuffer(m_buffer);
    m_buffer = NULL;
    m_fieldCount = 0;
    m_allocCount = 0;
}

bool GData::reserve(int size)
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

GData::FieldTypeType GData::getFieldType(int index) const
{
    if (index < 0 || index >= m_fieldCount) return GDATA_TYPE_NONE;

    return m_buffer[index].type;
}

GData::FieldTypeType GData::getFieldType(FieldNameType szKey) const
{
    DATA_ITEM * p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        if (p->name = szKey)
        {
            return p->type;
        }
    }

    return GDATA_TYPE_NONE;
}

GData::FieldNameType  GData::getFieldName(int index) const
{
    if (index < 0 || index >= m_fieldCount) return nullptr;

    return m_buffer[index].name;
}

void GData::delField(int index)
{
    freeItem(index);
}

void GData::delField(FieldNameType szKey)
{
    DATA_ITEM * p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        if (!strcmp(p->name, szKey))
        {
            freeItem(i);
            return;
        }   
    }
}

void GData::setValue(FieldNameType szKey, FieldTypeType type, FieldValueType value, bool copyKey)
{
	reserve(m_fieldCount + 1);

    DATA_ITEM * p = m_buffer;
	for (int i = 0; i < m_fieldCount; ++i, ++p)
	{
		if (!strcmp(p->name, szKey))
		{
            freeValue(p->type, p->value);
            p->type = type;
            p->value = value;
			return;
		}
	}

	++m_fieldCount;
	p->name = copyKey ? allocName(szKey) : szKey;
	p->type = type;	
	p->value = value;
}

bool GData::getValue(const char* szKey, FieldTypeType& type, FieldValueType& value) const
{
    DATA_ITEM * p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        if (!strcmp(p->name, szKey))
        {
            if (type != GDATA_TYPE_NONE)
            {
                if (type == p->type)
                {
                    value = p->value;
                    return true;
                }
            }
            else
            {
                type = p->type;
                value = p->value;
                return true;
            }
            return false;
        }
    }
    return false;
}

void GData::freeValue(FieldTypeType type, FieldValueType value)
{
    if (value.pv == nullptr) return;
	switch(type)
	{
    case GDATA_TYPE_INT64:
    case GDATA_TYPE_UINT64:
        freeBuffer(value.pv);
        break;
    case GDATA_TYPE_STRING:
    case GDATA_TYPE_STRINGW:
    case GDATA_TYPE_BUFFER:
        freeBuffer(value.pv);
        break;
	case GDATA_TYPE_DATA:
    case GDATA_TYPE_ARRAY:
    case GDATA_TYPE_OBJECT:
        ((GObject*)value.pv)->release();
        break;
    case GDATA_TYPE_INTVECTOR:
        freeBuffer(value.pv);
        break;
#ifdef GDATA_SUPPORT_COM
    case GDATA_TYPE_GUID:
        freeBuffer(value.pv);
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

void GData::freeItem(int index)
{
    if (index < 0 || index >= m_fieldCount) return;

    freeValue(m_buffer[index].type, m_buffer[index].value);
    freeName(m_buffer[index].name);

    if (index != m_fieldCount - 1)
    {
        m_buffer[index] = m_buffer[m_fieldCount - 1];
    }

    --m_fieldCount;
}

GData::FieldNameType GData::allocName(const char* szKey, int len) const
{
    if (len < 0) len = strlen(szKey);

    void* p = allocBuffer(len + 1);
    if (szKey) memcpy(p, szKey, len);
    ((byte*)p)[len] = 0;

    return (FieldNameType)p;
}

void GData::freeName(FieldNameType name) const
{
    freeBuffer((void*)name);
}