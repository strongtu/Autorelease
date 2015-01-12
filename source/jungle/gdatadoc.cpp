#include "stdafx.h"
#include "gdata.h"
#include "garray.h"
#include "gdataserialize.h"

namespace
{
    const int  g_dataHeaderLen = 4;
    byte const g_dataHeader21[g_dataHeaderLen] = {'T', 'D', '2', '1'};

    int utf82utf16(const char* us, int uslen, wchar_t* ws, int wslen)
    {
        return MultiByteToWideChar(CP_UTF8, 0, us, uslen, ws, wslen);
    }

    int utf162utf8(const wchar_t* ws, int wslen, char* us, int uslen)
    {
        return WideCharToMultiByte(CP_UTF8, 0, ws, wslen, us, uslen, nullptr, nullptr);
    }

    bool writeField(GDataSerialize& ser, GData::DATA_ITEM* p)
    {
        int len = strlen(p->name);
        if (!ser.writeByte(p->type) ||
            !ser.writeVarint32(len) ||
            !ser.writeRawHash(p->name, len))
        {
            return false;
        }

        GData::FieldValueType value = p->value;
        switch(p->type)
        {
        case GDATA_TYPE_BOOL:
            return ser.writeByte(value.bv);
        case GDATA_TYPE_INT:
            return ser.writeVarint32(ser.zigZagEncode32(value.iv));
        case GDATA_TYPE_UINT:
            return ser.writeVarint32(value.uv);
        case GDATA_TYPE_INT64:
            return ser.writeVarint64(*value.pi64);
        case GDATA_TYPE_UINT64:
            return ser.writeVarint64(ser.zigZagEncode64(*value.pu64));
        case GDATA_TYPE_FLOAT:
            return ser.writeFixed32(value.uv);
        case GDATA_TYPE_STRING:
            return ser.writeVarint32(value.ps->len) &&
                   ser.writeRawHash(value.ps->c, value.ps->len);
        case GDATA_TYPE_STRINGW:
            {
                int utf8len = utf162utf8(value.pws->wc, value.pws->len, nullptr, 0);
                if (!ser.getOutBuf())
                {
                    return ser.writeVarint32(utf8len) &&
                           ser.writeSkip(utf8len);
                }
                else
                {
                    if (!ser.writeVarint32(utf8len) || ser.getOutLeft() < utf8len) return false;
                    // we have enough buffer now
                    utf162utf8(value.pws->wc, value.pws->len, (char*)ser.getWrite(), utf8len);
                    return ser.writeRawHash(ser.getWrite(), utf8len);
                }
            }
        case GDATA_TYPE_BUFFER:
            return ser.writeVarint32(value.pb->len) &&
                   ser.writeRawHash(value.pb->by, value.pb->len);
        case GDATA_TYPE_DATA:
            if (ser.getOutBuf())
            {
                int docSize = value.pdata->serialize(ser.getWrite(), ser.getOutLeft());
                return ser.writeSkip(docSize);
            }
            else
            {
                int docSize = value.pdata->serialize(nullptr, 0);
                return ser.writeSkip(docSize);
            }
        case GDATA_TYPE_ARRAY:
            if (ser.getOutBuf())
            {
                int docSize = value.parray->serialize(ser.getWrite(), ser.getOutLeft());
                return ser.writeSkip(docSize);
            }
            else
            {
                int docSize = value.parray->serialize(nullptr, 0);
                return ser.writeSkip(docSize);
            }
            return false;
        case GDATA_TYPE_INTVECTOR:
        {
            if (!ser.writeVarint32(value.piv->len)) return false;
            if (ser.getOutBuf())
            {
                for (int i = 0; i < value.piv->len; i++)
                {
                    if (!ser.writeVarint32(ser.zigZagEncode32(value.piv->i[i]))) return false;
                }
                return true;
            }
            else
            {
                return ser.writeSkip(value.piv->len * sizeof(int));
            }
        }
#ifdef GDATA_SUPPORT_COM
        case GDATA_TYPE_GUID:
            return ser.writeRaw(value.pv, sizeof(GUID));
#endif
        default:
            return false;
        }
    }
}

// if buffer is not big enough, we write as much as we can
// that is not the best result, by don't care the detail now
int GData::serialize(byte* outBuf, int len) const
{
    GDataSerialize ser(outBuf, len);

    if (!ser.writeRaw(&g_dataHeader21[0], g_dataHeaderLen) ||
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
    ser.writeSkip(g_dataHeaderLen);
    ser.writeFixed16(fieldCount);
    return writeCount;
}

#ifdef __GDATA_BACKREAD__
#undef __GDATA_BACKREAD__
#endif

#define __GDATA_BADREAD__    clear();             \
                             freeName(fieldName); \
                             return nullptr;

const byte* GData::unserialize(const byte* inbuf, int len)
{
    clear();

    GDataSerialize ser(inbuf, len);

    byte header[g_dataHeaderLen] = {0};
    ser.readRaw(&header[0], g_dataHeaderLen);
    if (memcmp(&g_dataHeader21[0], &header[0], g_dataHeaderLen))
    {
        return nullptr;
    }

    uint16 fieldCount = 0;
    if (!ser.readFixed16(fieldCount)) return nullptr;

    for (int i = 0; i < fieldCount; ++i)
    {
        byte   fieldType = 0;
        uint32 fieldNameLen = 0;

        if (!ser.readByte(fieldType) ||
            !ser.readVarint32(fieldNameLen) ||
            ser.getInLeft() < fieldNameLen)
        {
            return nullptr;
        }

        // here is enough bytes left for name
        // so we can read name quickly
        FieldNameType fieldName = allocName(nullptr, fieldNameLen);//(const char*)ser.getRead(), fieldNameLen);
        ser.readRawHash((byte*)fieldName, fieldNameLen);

        FieldValueType value = nullptr;
        switch(fieldType)
        {
        case GDATA_TYPE_BOOL:
        {
            if (!ser.readByte(value.bv))     { __GDATA_BADREAD__ }
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_INT:
        {
            if (!ser.readVarint32(value.uv)) { __GDATA_BADREAD__ }
            value.uv = ser.zigZagDecode32(value.uv);
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_UINT:
        {
            if (!ser.readVarint32(value.uv)) { __GDATA_BADREAD__ }
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_INT64:
        {
            uint64 uv = 0;
            if (!ser.readVarint64(uv)) { __GDATA_BADREAD__ }
            int64 v = ser.zigZagDecode64(uv);

            value.pi64 = static_cast<int64*>(memcpy(allocBuffer(sizeof(int64)), &v, sizeof(int64)));
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_UINT64:
        {
            uint64 uv = 0;
            if (!ser.readVarint64(uv)) { __GDATA_BADREAD__ }

            value.pi64 = static_cast<int64*>(memcpy(allocBuffer(sizeof(int64)), &uv, sizeof(int64)));
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_FLOAT:
        {
            if (!ser.readFixed32(value.uv)) { __GDATA_BADREAD__ }
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_STRING:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GDATA_BADREAD__ }

            value.ps = static_cast<data_string*>(allocBuffer(sizeof(value.ps->len) + (len + 1) * sizeof(value.ps->c[0])));
            value.ps->len = len;

            ser.readRawHash(&(value.ps->c), len);
            value.ps->c[len] = 0;

            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_STRINGW:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GDATA_BADREAD__ }

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
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_BUFFER:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len) { __GDATA_BADREAD__ }

            value.pb = static_cast<buffer*>(allocBuffer(sizeof(value.pb->len) + len));
            ser.readRawHash(value.pb->by, len);

            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_DATA:
        {
            GData* data = static_cast<GData*>(GData::createObject());
            int docSize = data->unserialize(ser.getRead(), ser.getInLeft()) - ser.getRead();
            if (!docSize) { data->release(); __GDATA_BADREAD__ }
            ser.readSkip(docSize);
            value.pdata = data;
            setValue(fieldName, fieldType, value, false);
            break;
        }
        case GDATA_TYPE_ARRAY:
            break;
        case GDATA_TYPE_OBJECT:
            break;
        case GDATA_TYPE_INTVECTOR:
        {
            uint32 len = 0;
            if (!ser.readVarint32(len) || ser.getInLeft() < len * sizeof(int)) { __GDATA_BADREAD__ }

            value.piv = static_cast<int_vector*>(allocBuffer((len + 1) * sizeof(int)));
            value.piv->len = len;
            int* pi = value.piv->i;
            for (int i = 0; i < len; i++)
            {
                uint32 uv = 0;
                ser.readVarint32(uv);
                *pi++ = ser.zigZagDecode32(uv);
            }
            setValue(fieldName, fieldType, value, false);
            break;
        }
#ifdef GDATA_SUPPORT_COM
        case GDATA_TYPE_INTERFACE:
            break;
        case GDATA_TYPE_GUID:
        {
            GUID guid = {0};
            if (!ser.readRaw(&guid, sizeof(guid))) { __GDATA_BADREAD__ }
            value.pv = memcpy(allocBuffer(sizeof(guid)), &guid, sizeof(guid));
            setValue(fieldName, fieldType, value, false);
            break;
        }
#endif
        default:
        {   // an unkonw type must be the TLV sructor
            uint32 len = 0;
            if (!ser.readVarint32(len) ||
                !ser.readSkip(len))
            {
                __GDATA_BADREAD__
            }
            freeName(fieldName);
            break;
        }
        }
    }

    return ser.getRead();
}
