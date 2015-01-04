#include "gdata.h"
#include "tcmalloc\tcmalloc.h"

IMPLEMENT_GDYNAMIC_CLASS(GData, GObject)

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
    if (index < 0 || index >= m_fieldCount) return NULL;

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

int GData::getDoc(char* outBuf, int len) const
{
    return 0;
}

bool GData::setDoc(const char* outBuf, int len)
{
    return true;
}

bool GData::getBool(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_BOOL;
    FieldValueType value = 0;

    return getValue(szKey, type, value) ? (bool)value : false;
}

int GData::getInt(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_INT;
    FieldValueType value = 0;

    return getValue(szKey, type, value) ? (int)value : 0;
}

#ifdef GDATA_SUPPORT_COM
GUID GData::getGuid(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_GUID;
    FieldValueType value = 0;

    GUID guid = {0};
    getValue(szKey, type, value) ? memcpy(&guid, value, sizeof(GUID)): 0;
    return guid;
}

IUnknown* GData::getInterface(const char* szKey) const
{
    FieldTypeType type = GDATA_TYPE_INTERFACE;
    FieldValueType value = 0;

    return getValue(szKey, type, value) ? (IUnknown*)value : NULL;
}
#endif

void GData::setBool(const char* szKey, bool value)
{
    setValue(szKey, GDATA_TYPE_BOOL, (FieldValueType)value);
}

void GData::setInt(const char* szKey, int value)
{
    setValue(szKey, GDATA_TYPE_INT, (FieldValueType)value);
}

#ifdef GDATA_SUPPORT_COM
void GData::setGuid(const char* szKey, REFGUID value)
{
	GUID * p = (GUID*)memcpy(allocBuffer(sizeof(GUID)), &value, sizeof(GUID));

    setValue(szKey, GDATA_TYPE_GUID, p);
}

void GData::setInterface(const char* szKey, IUnknown* value)
{
    if (value) value->AddRef();

	setValue(szKey, GDATA_TYPE_GUID, value);
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

void GData::setValue(FieldNameType szKey, FieldTypeType type, FieldValueType value)
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
	p->name = allocName(szKey);
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
	if (value == 0) return;
	switch(type)
	{
	case GDATA_TYPE_DATA:
    case GDATA_TYPE_ARRAY:
    case GDATA_TYPE_OBJECT:
        ((GObject*)value)->release();
        break;
#ifdef GDATA_SUPPORT_COM
    case GDATA_TYPE_GUID:
        freeBuffer(value);
        break;
    case GDATA_TYPE_INTERFACE:
        ((IUnknown*)value)->Release();
        break;
#endif
	}
	value = 0;
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

void* GData::allocBuffer(int size) const
{
    return tc_malloc(size);
}

void GData::freeBuffer(void* p) const
{
    return tc_free(p);
}

GData::FieldNameType GData::allocName(const char* szKey) const
{
    size_t len = strlen(szKey) + 1;
    return (FieldNameType)memcpy(allocBuffer(len), szKey, len);
}

void GData::freeName(FieldNameType name) const
{
    freeBuffer((void*)name);
}