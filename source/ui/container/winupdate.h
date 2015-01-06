#ifndef __WINUPDATE_H__
#define __WINUPDATE_H__

#include "./winhdr.h"

class GUpdateWindow
{
public:
    GUpdateWindow();
    ~GUpdateWindow();

    BOOL Update(HWND hWnd, HDC hdcDst, HDC hdcSrc, LPCRECT lpRcDirty, BYTE Alpha);

    static GUpdateWindow& GetInstance();

protected:
    typedef BOOL (WINAPI *PF_UpdateLayeredWindowIndirect)(HWND hWnd, UPDATELAYEREDWINDOWINFO CONST *pULWInfo);
    PF_UpdateLayeredWindowIndirect m_pfnUpdateLayered;
    HMODULE m_hUser32;
};


#endif /*__WINUPDATE_H__*/
