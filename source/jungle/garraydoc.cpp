#include "stdafx.h"
#include "gdata.h"
#include "garray.h"
#include "gdataserialize.h"
#include "gfield.h"

namespace
{
    const int  g_headerLen = 4;
    byte const g_arrayHeader21[g_headerLen] = {'T', 'A', '2', '1'};

    bool writeField(GDataSerialize& ser, GArray::DATA_ITEM* p)
    {
        if (!ser.writeByte(p->type) ||
            !writeValue(ser, p->type, p->value))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

int GArray::serialize(byte* outBuf, int len) const
{
    GDataSerialize ser(outBuf, len);

    if (!ser.writeRaw(&g_arrayHeader21[0], g_headerLen) ||
        !ser.writeFixed16(0))
    {
        return 0;
    }

    int fieldCount = 0;
    DATA_ITEM* p = m_buffer;
    for (int i = 0; i < m_fieldCount; ++i, ++p)
    {
        switch(p->type)
        {
        case GDATA_TYPE_BOOL:
        case GDATA_TYPE_INT:
        case GDATA_TYPE_UINT:
        case GDATA_TYPE_INT64:
        case GDATA_TYPE_UINT64:
        case GDATA_TYPE_FLOAT:
        case GDATA_TYPE_STRING:
        case GDATA_TYPE_STRINGW:
        case GDATA_TYPE_BUFFER:
        case GDATA_TYPE_DATA:
        case GDATA_TYPE_ARRAY:
        case GDATA_TYPE_INTVECTOR:
#ifdef GDATA_SUPPORT_COM
        case GDATA_TYPE_GUID:
#endif
        {
            byte* start = ser.getWrite();
            if (!writeField(ser, p))
            {
                ser.setWrite(start);
                break;
            }
            fieldCount++;
            break;
        }
        default:
            break;
        }
    }

    int writeCount = ser.getWriteLen();

    // fixed the fieldCount
    ser.setWrite(outBuf);
    ser.writeSkip(g_headerLen);
    ser.writeFixed16(fieldCount);
    return writeCount;
}

#ifdef __GARRAY_BADREAD__
#undef __GARRAY_BADREAD__
#endif

#define __GARRAY_BADREAD__    clear();             \
                              return nullptr;

const byte* GArray::unserialize(const byte* inbuf, int len)
{
    clear();

    GDataSerialize ser(inbuf, len);

    byte header[g_headerLen] = {0};
    ser.readRaw(&header[0], g_headerLen);
    if (memcmp(&g_arrayHeader21[0], &header[0], g_headerLen))
    {
        return nullptr;
    }

    uint16 fieldCount = 0;
    if (!ser.readFixed16(fieldCount)) return nullptr;

    for (int i = 0; i < fieldCount; ++i)
    {
        byte   fieldType = 0;

        if (!ser.readByte(fieldType)) { return nullptr; }

        FieldValueType value = nullptr;
        switch(fieldType)
        {
        case GDATA_TYPE_BOOL:
        {
            if (!ser.readByte(value.bv))     { __GARRAY_BADREAD__ }
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_INT:
        {
            if (!ser.readVarint32(value.uv)) { __GARRAY_BADREAD__ }
            value.uv = ser.zigZagDecode32(value.uv);
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_UINT:
        {
            if (!ser.readVarint32(value.uv)) { __GARRAY_BADREAD__ }
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_INT64:
        {
            uint64 uv = 0;
            if (!ser.readVarint64(uv)) { __GARRAY_BADREAD__ }
            int64 v = ser.zigZagDecode64(uv);

            value.pi64 = static_cast<int64*>(memcpy(allocBuffer(sizeof(int64)), &v, sizeof(int64)));
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_UINT64:
        {
            uint64 uv = 0;
            if (!ser.readVarint64(uv)) { __GARRAY_BADREAD__ }

            value.pi64 = static_cast<int64*>(memcpy(allocBuffer(sizeof(int64)), &uv, sizeof(int64)));
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_FLOAT:
        {
            if (!ser.readFixed32(value.uv)) { __GARRAY_BADREAD__ }
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_STRING:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GARRAY_BADREAD__ }

            value.ps = static_cast<data_string*>(allocBuffer(sizeof(value.ps->len) + (len + 1) * sizeof(value.ps->c[0])));
            value.ps->len = len;

            ser.readRawHash(&(value.ps->c), len);
            value.ps->c[len] = 0;

            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_STRINGW:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GARRAY_BADREAD__ }

            data_string* ps = static_cast<data_string*>(allocBuffer(sizeof(ps->len) + (len + 1) * sizeof(ps->c[0])));
            ps->len = len;

            ser.readRawHash(&(ps->c), len);
            ps->c[len] = 0;

            int wslen = utf82utf16((ps->c), ps->len, nullptr, 0); 
            data_wstring* psw = static_cast<data_wstring*>(allocBuffer(sizeof(psw->len) + (wslen + 1) * sizeof(psw->wc[0])));
            psw->len = wslen;
            psw->wc[wslen] = 0;
            utf82utf16((ps->c), ps->len, (psw->wc), psw->len);

            freeBuffer(ps);

            value.pv = psw;
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_BUFFER:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GARRAY_BADREAD__ }

            value.pb = static_cast<buffer*>(allocBuffer(sizeof(value.pb->len) + len));
            ser.readRawHash(value.pb->by, len);

            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_DATA:
        {
            GData* data = static_cast<GData*>(GData::createObject());
            int docSize = data->unserialize(ser.getRead(), ser.getInLeft()) - ser.getRead();
            if (!docSize) { data->release(); __GARRAY_BADREAD__ }
            ser.readSkip(docSize);
            value.pdata = data;
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_ARRAY:
        {
            GArray* array = static_cast<GArray*>(GArray::createObject());
            int docSize = array->unserialize(ser.getRead(), ser.getInLeft()) - ser.getRead();
            if (!docSize) { array->release(); __GARRAY_BADREAD__ }
            ser.readSkip(docSize);
            value.parray = array;
            addValue(i, fieldType, value);
            break;
        }
        case GDATA_TYPE_OBJECT:
            break;
        case GDATA_TYPE_INTVECTOR:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len * sizeof(int)) { __GARRAY_BADREAD__ }

            value.piv = static_cast<int_vector*>(allocBuffer((len + 1) * sizeof(int)));
            value.piv->len = len;
            int* pi = value.piv->i;
            for (int i = 0; i < len; i++)
            {
                uint32 uv = 0;
                ser.readVarint32(uv);
                *pi++ = ser.zigZagDecode32(uv);
            }
            addValue(i, fieldType, value);
            break;
        }
#ifdef GDATA_SUPPORT_COM
        case GDATA_TYPE_INTERFACE:
            break;
        case GDATA_TYPE_GUID:
        {
            GUID guid = {0};
            if (!ser.readRaw(&guid, sizeof(guid))) { __GARRAY_BADREAD__ }
            value.pv = memcpy(allocBuffer(sizeof(guid)), &guid, sizeof(guid));
            addValue(i, fieldType, value);
            break;
        }
#endif
        default:
        {   // an unkonw type must be the TLV sructor
            uint32 len = 0;
            if (!ser.readVarint32(len) ||
                !ser.readSkip(len))
            {
                __GARRAY_BADREAD__
            }
            break;
        }
        }
    }
    return ser.getRead();
}
