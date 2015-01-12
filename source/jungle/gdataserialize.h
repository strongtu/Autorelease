#ifndef __GDATA_SERIALIZE_H__
#define __GDATA_SERIALIZE_H__

#include "gtype.h"

class GDataSerialize
{
public:
    GDataSerialize(byte* buf, int len);
    GDataSerialize(const byte* buf, int len);
    ~GDataSerialize(void);

    inline byte* getOutBuf() { return m_outBuf;}
    inline int   getOutLen() { return m_outBufEnd - m_outBuf;}
    inline int   getOutLeft(){ return m_outBufEnd - m_writeBuf;}

    inline byte* getWrite()            { return m_writeBuf;}
    inline void  setWrite(byte* write) { m_writeBuf = write;}
    inline int   getWriteLen()         { return m_writeBuf - m_outBuf;}

    inline const byte* getIn()      { return m_inBuf;}
    inline int         getInLen()   { return m_inBufEnd - m_inBuf;}
    inline int         getInLeft()  { return m_inBufEnd - m_readBuf;}

    inline const byte* getRead()    { return m_readBuf;}
    inline int         getReadLen() { return m_readBuf - m_inBuf;}

    bool writeByte(byte value);
    bool writeFixed16(uint16 value);
    bool writeFixed32(uint32 value);
    bool writeVarint32(uint32 value);
    bool writeVarint64(uint64 value);

    bool readByte(byte &value);
    bool readFixed16(uint16 &value);
    bool readFixed32(uint32 &value);
    bool readVarint32(uint32 &value);
    bool readVarint64(uint64 &value);

    bool readRaw(void* value, int len);
    bool readRawHash(void* value, int len);
    bool readSkip(int len);
    bool writeRaw(const void* value, int len);
    bool writeRawHash(const void* value, int len);
    bool writeSkip(int len);

    byte*        writeVarint32ToArray(uint32 value, byte* target);
    byte*        writeVarint64ToArray(uint64 value, byte* target);
    const uint8* readVarint32FromArray(const uint8* buffer, uint32* value);
    bool         readVarint32Fallback(uint32* value);
    bool         readVarint32Slow(uint32* value);
    bool         readVarint64Fallback(uint64* value);
    bool         readVarint64Slow(uint64* value);

    inline int varintSize32(uint32 value);
    inline int varintSize64(uint64 value);

    inline uint32 zigZagEncode32(int32  n) { return (n << 1) ^ (n >> 31);}
    inline  int32 zigZagDecode32(uint32 n) { return (n >> 1) ^ -static_cast<int32>(n & 1);}
    inline uint64 zigZagEncode64(int64  n) { return (n << 1) ^ (n >> 63);}
    inline  int64 zigZagDecode64(uint64 n) { return (n >> 1) ^ -static_cast<int64>(n & 1);}

protected:
    byte* m_outBuf;
    byte* m_outBufEnd;
    byte* m_writeBuf;

    const byte* m_inBuf;
    const byte* m_inBufEnd;
    const byte* m_readBuf;
};

#endif