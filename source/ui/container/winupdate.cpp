#include "stdafx.h"
#include "./winupdate.h"

GUpdateWindow::GUpdateWindow()
{
    m_pfnUpdateLayered = NULL;
    m_hUser32 = LoadLibrary(_T("User32.dll"));
    m_pfnUpdateLayered = (PF_UpdateLayeredWindowIndirect)GetProcAddress(m_hUser32, "UpdateLayeredWindowIndirect");
}

GUpdateWindow::~GUpdateWindow()
{
    m_pfnUpdateLayered = NULL;
    FreeLibrary(m_hUser32);
}

BOOL GUpdateWindow::Update(HWND hWnd, HDC hdcDst, HDC hdcSrc, LPCRECT lpRcDirty, BYTE Alpha)
{
    if (::IsIconic(hWnd))
    {
        return TRUE;
    }

    RECT rcWin = {0, 0, 0, 0};
    GetWindowRect(hWnd, &rcWin);

    const RECT* lpRcWin = &rcWin;

    BLENDFUNCTION Blend;

    Blend.BlendOp = AC_SRC_OVER;        // the only BlendOp defined in Windows 2000
    Blend.BlendFlags = 0;               // nothing else is special ...
    Blend.AlphaFormat = AC_SRC_ALPHA;   // ...
    Blend.SourceConstantAlpha = Alpha;  // AC_SRC_ALPHA

    POINT ptSrc = {0, 0};
    POINT ptWinPos = {lpRcWin->left, lpRcWin->top};
    SIZE szWin = {lpRcWin->right - lpRcWin->left, lpRcWin->bottom - lpRcWin->top};

    BOOL bRet = FALSE;

    if (m_pfnUpdateLayered)
    {
        UPDATELAYEREDWINDOWINFO ULWInfo;
        ULWInfo.cbSize = sizeof(ULWInfo);
        ULWInfo.hdcDst = hdcDst;
        ULWInfo.pptDst = &ptWinPos;
        ULWInfo.psize = &szWin;
        ULWInfo.hdcSrc = hdcSrc;
        ULWInfo.pptSrc = &ptSrc;
        ULWInfo.crKey = 0;
        ULWInfo.pblend = &Blend;
        ULWInfo.dwFlags = ULW_ALPHA;
        ULWInfo.prcDirty = lpRcDirty;

        bRet = m_pfnUpdateLayered(hWnd, &ULWInfo);
    }
    else
    {
        bRet = ::UpdateLayeredWindow(hWnd, hdcDst, &ptWinPos, &szWin, hdcSrc, &ptSrc, 0, &Blend, ULW_ALPHA);
    }

    if (!bRet)
    {
        int err = GetLastError();
    }

    return bRet;
}

GUpdateWindow& GUpdateWindow::GetInstance()    
{
    static GUpdateWindow instance;
    return instance;
}