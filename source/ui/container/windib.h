#ifndef __WINDIB_H__
#define __WINDIB_H__

#include "./winhdr.h"

class DIB
{
public:
    DIB(void);
    ~DIB(void);
    void Rebuild(int cx, int cy);
    byte* GetData();
    operator HDC() { return m_hDC;}
    operator HBITMAP() { return m_hDIB;}
protected:
    void CreateDevice();
protected:
    HDC     m_hDC;
    HBITMAP m_hDIB;
    byte*  m_lpByte;
};

#endif /*__WINDIB_H__*/
