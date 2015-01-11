#include "stdafx.h"
#include "gdataserialize.h"

namespace
{
    static const int kMaxVarintBytes = 10;
    static const int kMaxVarint32Bytes = 5;
}

GDataSerialize::GDataSerialize(byte* buf, int len)
: m_outBuf(buf)
, m_writeBuf(buf)
, m_inBuf(nullptr)
, m_readBuf(nullptr)
{
    if (buf)
    {
        m_outBufEnd = buf + len;
    }
    else
    {
        m_outBufEnd = (byte*)-1;
    }
}

GDataSerialize::GDataSerialize(const byte* buf, int len)
: m_outBuf(nullptr)
, m_writeBuf(nullptr)
, m_inBuf(buf)
, m_readBuf(buf)
{
    if (buf)
    {
        m_inBufEnd = buf + len;
    }
    else
    {
        m_inBufEnd = (byte*)-1;
    }
}

GDataSerialize::~GDataSerialize(void)
{
}

bool GDataSerialize::writeByte(byte value)
{
    if (!m_outBuf) return m_writeBuf++;

    if (m_writeBuf < m_outBufEnd)
    {
        *m_writeBuf++ = value;
        return true;
    }
    return false;
}

bool GDataSerialize::writeFixed16(uint16 value)
{
    if (!m_outBuf) return m_writeBuf += sizeof(value);
    
    if (getOutLeft() >= sizeof(value))
    {
        *m_writeBuf++ = static_cast<uint8>(value);
        *m_writeBuf++ = static_cast<uint8>(value >> 8);
        return true;
    }
    return false;
}

bool GDataSerialize::writeFixed32(uint32 value)
{
    if (!m_outBuf) return m_writeBuf += sizeof(value);

    if (getOutLeft() >= sizeof(value))
    {
        *m_writeBuf++ = static_cast<uint8>(value);
        *m_writeBuf++ = static_cast<uint8>(value >> 8);
        *m_writeBuf++ = static_cast<uint8>(value >> 16);
        *m_writeBuf++ = static_cast<uint8>(value >> 24);
        return true;
    }
    return false;
}

bool GDataSerialize::writeVarint32(uint32 value)
{
  if (!m_outBuf) { m_writeBuf += varintSize32(value); return true;}

  if (getOutLeft() >= kMaxVarint32Bytes) {
    uint8* target = m_writeBuf;
    uint8* end = writeVarint32ToArray(value, target);
    m_writeBuf = end;
    return true;
  } else {
    uint8 bytes[kMaxVarint32Bytes];
    int size = 0;
    while (value > 0x7F) {
      bytes[size++] = (static_cast<uint8>(value) & 0x7F) | 0x80;
      value >>= 7;
    }
    bytes[size++] = static_cast<uint8>(value) & 0x7F;
    return writeRaw(bytes, size);
  }
}

bool GDataSerialize::writeVarint64(uint64 value)
{
  if (!m_outBuf) { m_writeBuf += varintSize64(value); return true;}

  if (getOutLeft() >= kMaxVarintBytes) {
    uint8* target = m_writeBuf;
    uint8* end = writeVarint64ToArray(value, target);
    m_writeBuf = end;
  } else {
    uint8 bytes[kMaxVarintBytes];
    int size = 0;
    while (value > 0x7F) {
      bytes[size++] = (static_cast<uint8>(value) & 0x7F) | 0x80;
      value >>= 7;
    }
    bytes[size++] = static_cast<uint8>(value) & 0x7F;
    return writeRaw(bytes, size);
  }
}

bool GDataSerialize::readByte(byte &value)
{
    if (m_readBuf >= m_inBufEnd) return false;
    value = *m_readBuf++;
    return true;
}

bool GDataSerialize::readFixed16(uint16 &value)
{
    if (getInLeft() < sizeof(value)) return false;

    value = *m_readBuf++;
    value = value | ((*m_readBuf++) << 8);

    return true;
}

bool GDataSerialize::readFixed32(uint32 &value)
{
    if (getInLeft() < sizeof(value)) return false;

    value = *m_readBuf++;
    value = value | ((*m_readBuf++) << 8);
    value = value | ((*m_readBuf++) << 16);
    value = value | ((*m_readBuf++) << 24);

    return true;
}

bool GDataSerialize::readVarint32(uint32 &value)
{
  if (m_readBuf < m_inBufEnd && *m_readBuf < 0x80) {
    value = *m_readBuf++;
    return true;
  } else {
    return ReadVarint32Fallback(&value);
  }
}

const uint8* GDataSerialize::ReadVarint32FromArray(const uint8* buffer, uint32* value) {
  // Fast path:  We have enough bytes left in the buffer to guarantee that
  // this read won't cross the end, so we can skip the checks.
  const uint8* ptr = buffer;
  uint32 b;
  uint32 result;

  b = *(ptr++); result  = (b & 0x7F)      ; if (!(b & 0x80)) goto done;
  b = *(ptr++); result |= (b & 0x7F) <<  7; if (!(b & 0x80)) goto done;
  b = *(ptr++); result |= (b & 0x7F) << 14; if (!(b & 0x80)) goto done;
  b = *(ptr++); result |= (b & 0x7F) << 21; if (!(b & 0x80)) goto done;
  b = *(ptr++); result |=  b         << 28; if (!(b & 0x80)) goto done;

  // If the input is larger than 32 bits, we still need to read it all
  // and discard the high-order bits.
  for (int i = 0; i < kMaxVarintBytes - kMaxVarint32Bytes; i++) {
    b = *(ptr++); if (!(b & 0x80)) goto done;
  }

  // We have overrun the maximum size of a varint (10 bytes).  Assume
  // the data is corrupt.
  return nullptr;

 done:
  *value = result;
  return ptr;
}

bool GDataSerialize::ReadVarint32Fallback(uint32* value) {
  if ((m_inBufEnd - m_readBuf) >= kMaxVarintBytes ||
      (m_inBufEnd > m_readBuf && !(m_inBufEnd[-1] & 0x80))) {
    const uint8* end = ReadVarint32FromArray(m_readBuf, value);
    if (end == nullptr) return false;
    
    m_readBuf = end;
    return true;
  } else {
    // Really slow case: we will incur the cost of an extra function call here,
    // but moving this out of line reduces the size of this function, which
    // improves the common case. In micro benchmarks, this is worth about 10-15%
    return ReadVarint32Slow(value);
  }
}

bool GDataSerialize::ReadVarint32Slow(uint32* value) {
  uint64 result;
  // Directly invoke ReadVarint64Fallback, since we already tried to optimize
  // for one-byte varints.
  if (!ReadVarint64Fallback(&result)) return false;
  *value = (uint32)result;
  return true;
}

bool GDataSerialize::readVarint64(uint64 &value)
{
  if ((m_inBufEnd > m_readBuf) && *m_readBuf < 0x80) {
    value = *m_readBuf++;
    return true;
  } else {
    return ReadVarint64Fallback(&value);
  }
}

bool GDataSerialize::ReadVarint64Fallback(uint64* value) {
  if ((m_inBufEnd - m_readBuf) >= kMaxVarintBytes ||
      (m_inBufEnd > m_readBuf && !(m_inBufEnd[-1] & 0x80))) {
    const uint8* ptr = m_readBuf;
    uint32 b;

    // Splitting into 32-bit pieces gives better performance on 32-bit
    // processors.
    uint32 part0 = 0, part1 = 0, part2 = 0;

    b = *(ptr++); part0  = (b & 0x7F)      ; if (!(b & 0x80)) goto done;
    b = *(ptr++); part0 |= (b & 0x7F) <<  7; if (!(b & 0x80)) goto done;
    b = *(ptr++); part0 |= (b & 0x7F) << 14; if (!(b & 0x80)) goto done;
    b = *(ptr++); part0 |= (b & 0x7F) << 21; if (!(b & 0x80)) goto done;
    b = *(ptr++); part1  = (b & 0x7F)      ; if (!(b & 0x80)) goto done;
    b = *(ptr++); part1 |= (b & 0x7F) <<  7; if (!(b & 0x80)) goto done;
    b = *(ptr++); part1 |= (b & 0x7F) << 14; if (!(b & 0x80)) goto done;
    b = *(ptr++); part1 |= (b & 0x7F) << 21; if (!(b & 0x80)) goto done;
    b = *(ptr++); part2  = (b & 0x7F)      ; if (!(b & 0x80)) goto done;
    b = *(ptr++); part2 |= (b & 0x7F) <<  7; if (!(b & 0x80)) goto done;

    // We have overrun the maximum size of a varint (10 bytes).  The data
    // must be corrupt.
    return false;

done:
    m_readBuf = ptr;
    *value = (static_cast<uint64>(part0)      ) |
             (static_cast<uint64>(part1) << 28) |
             (static_cast<uint64>(part2) << 56);
    return true;
  } else {
    return ReadVarint64Slow(value);
  }
}

bool GDataSerialize::ReadVarint64Slow(uint64* value) {
  // Slow path:  This read might cross the end of the buffer, so we
  // need to check and refresh the buffer if and when it does.

  uint64 result = 0;
  int count = 0;
  uint32 b;

  do {
    if (count == kMaxVarintBytes) return false;
    if (m_inBufEnd >= m_readBuf) {
     return false;
    }
    b = *m_readBuf++;
    result |= static_cast<uint64>(b & 0x7F) << (7 * count);
    ++count;
  } while (b & 0x80);

  *value = result;
  return true;
}

bool GDataSerialize::readRaw(void* value, int len)
{
    if (!m_inBuf) { m_readBuf += len; return true;}

    if ((m_inBufEnd - m_readBuf) >= len)
    {
        memcpy(value, m_readBuf, len);
        m_readBuf += len;
        return true;
    }
    return false;
}

bool GDataSerialize::readRawHash(void* value, int len)
{
    if (!m_inBuf) { m_readBuf += len; return true;}

    if ((m_inBufEnd - m_readBuf) >= len)
    {
        byte* pb = static_cast<byte*>(value);
        byte hashv = ((byte)(len)) ^ ((byte)(len >> 8));
        for (int i = 0; i < len; ++i)
        {
            *pb++ = ~(*m_readBuf++) ^ hashv;
        }
        return true;
    }
    return false;
}

bool GDataSerialize::readSkip(int len)
{
    if (!m_inBuf) { m_readBuf += len; return true;}

    if ((m_inBufEnd - m_readBuf) >= len)
    {
        m_readBuf += len;
        return true;
    }
    return false;
}

bool GDataSerialize::writeRaw(const void* value, int len)
{
    if (!m_outBuf) { m_writeBuf += len; return true;}

    if ((m_outBufEnd - m_writeBuf) >= len)
    {
        memcpy(m_writeBuf, value, len);
        m_writeBuf += len;
        return true;
    }
    return false;
}

bool GDataSerialize::writeSkip(int len)
{
    if (!m_outBuf) { m_writeBuf += len; return true;}

    if ((m_outBufEnd - m_writeBuf) >= len)
    {
        m_writeBuf += len;
        return true;
    }
    return false;
}

bool GDataSerialize::writeRawHash(const void* value, int len)
{
    if (!m_outBuf) { m_writeBuf += len; return true;}

    if ((m_outBufEnd - m_writeBuf) >= len)
    {
        const byte* pb = static_cast<const byte*>(value);
        byte hashv = ((byte)(len)) ^ ((byte)(len >> 8));
        for (int i = 0; i < len; ++i)
        {
            *m_writeBuf++ = ~*pb++ ^ hashv;
        }

        return true;
    }
    return false;
}

byte* GDataSerialize::writeVarint32ToArray(uint32 value, byte* target) {
  target[0] = static_cast<uint8>(value | 0x80);
  if (value >= (1 << 7)) {
    target[1] = static_cast<uint8>((value >>  7) | 0x80);
    if (value >= (1 << 14)) {
      target[2] = static_cast<uint8>((value >> 14) | 0x80);
      if (value >= (1 << 21)) {
        target[3] = static_cast<uint8>((value >> 21) | 0x80);
        if (value >= (1 << 28)) {
          target[4] = static_cast<uint8>(value >> 28);
          return target + 5;
        } else {
          target[3] &= 0x7F;
          return target + 4;
        }
      } else {
        target[2] &= 0x7F;
        return target + 3;
      }
    } else {
      target[1] &= 0x7F;
      return target + 2;
    }
  } else {
    target[0] &= 0x7F;
    return target + 1;
  }
}

byte* GDataSerialize::writeVarint64ToArray(uint64 value, byte* target) {
  uint32 part0 = static_cast<uint32>(value      );
  uint32 part1 = static_cast<uint32>(value >> 28);
  uint32 part2 = static_cast<uint32>(value >> 56);

  int size;

  if (part2 == 0) {
    if (part1 == 0) {
      if (part0 < (1 << 14)) {
        if (part0 < (1 << 7)) {
          size = 1; goto size1;
        } else {
          size = 2; goto size2;
        }
      } else {
        if (part0 < (1 << 21)) {
          size = 3; goto size3;
        } else {
          size = 4; goto size4;
        }
      }
    } else {
      if (part1 < (1 << 14)) {
        if (part1 < (1 << 7)) {
          size = 5; goto size5;
        } else {
          size = 6; goto size6;
        }
      } else {
        if (part1 < (1 << 21)) {
          size = 7; goto size7;
        } else {
          size = 8; goto size8;
        }
      }
    }
  } else {
    if (part2 < (1 << 7)) {
      size = 9; goto size9;
    } else {
      size = 10; goto size10;
    }
  }

  // can not be here

  size10: target[9] = static_cast<uint8>((part2 >>  7) | 0x80);
  size9 : target[8] = static_cast<uint8>((part2      ) | 0x80);
  size8 : target[7] = static_cast<uint8>((part1 >> 21) | 0x80);
  size7 : target[6] = static_cast<uint8>((part1 >> 14) | 0x80);
  size6 : target[5] = static_cast<uint8>((part1 >>  7) | 0x80);
  size5 : target[4] = static_cast<uint8>((part1      ) | 0x80);
  size4 : target[3] = static_cast<uint8>((part0 >> 21) | 0x80);
  size3 : target[2] = static_cast<uint8>((part0 >> 14) | 0x80);
  size2 : target[1] = static_cast<uint8>((part0 >>  7) | 0x80);
  size1 : target[0] = static_cast<uint8>((part0      ) | 0x80);

  target[size-1] &= 0x7F;
  return target + size;
}

inline int GDataSerialize::varintSize32(uint32 value) {
  if (value < (1 << 7)) {
    return 1;
  } else if (value < (1 << 14)) {
    return 2;
  } else if (value < (1 << 21)) {
    return 3;
  } else if (value < (1 << 28)) {
    return 4;
  } else {
    return 5;
  }
}

inline int GDataSerialize::varintSize64(uint64 value) {
  if (value < (1ull << 35)) {
    if (value < (1ull << 7)) {
      return 1;
    } else if (value < (1ull << 14)) {
      return 2;
    } else if (value < (1ull << 21)) {
      return 3;
    } else if (value < (1ull << 28)) {
      return 4;
    } else {
      return 5;
    }
  } else {
    if (value < (1ull << 42)) {
      return 6;
    } else if (value < (1ull << 49)) {
      return 7;
    } else if (value < (1ull << 56)) {
      return 8;
    } else if (value < (1ull << 63)) {
      return 9;
    } else {
      return 10;
    }
  }
}
/*
inline uint32 GDataSerialize::zigZagEncode32(int32 n)
{
    return (n << 1) ^ (n >> 31); 
}

inline int32 GDataSerialize::zigZagDecode32(uint32 n) 
{
  return (n >> 1) ^ -static_cast<int32>(n & 1);
}

inline uint64 GDataSerialize::zigZagEncode64(int64 n)
{
    return (n << 1) ^ (n >> 63);
}

inline int64 GDataSerialize::zigZagDecode64(uint64 n) 
{
  return (n >> 1) ^ -static_cast<int64>(n & 1);
}
*/