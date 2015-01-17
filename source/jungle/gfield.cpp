#include "stdafx.h"
#include "gfield.h"
#include "gdata.h"
#include "garray.h"

bool writeValue(GDataSerialize& ser, FieldTypeType type, FieldValueType value)
{
    switch(type)
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