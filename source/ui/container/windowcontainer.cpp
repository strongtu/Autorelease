#include "stdafx.h"
#include "./windowcontainer.h"

WindowContainer::WindowContainer(void) :
    m_hCanvas(0),
    m_hBitmap(0),
    m_bMouseEnter(false),
    m_bTrackingMouse(FALSE),
    m_hCursor(nullptr),
    m_szMax(-1, -1),
    m_szMin(-1, -1),
    m_bCloseWindow(true)
{
    m_rcUpdate.SetRectEmpty();
}

WindowContainer::~WindowContainer(void)
{
    if (m_hCanvas)
    {
//        ::GCanvasDestroy(m_hCanvas);
    }
    if (m_hBitmap)
    {
 //       ::GBitmapDestroy(m_hBitmap);
    }
}

bool WindowContainer::CreateContainer(const GRect& rect)
{
    DWORD dwStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX;
    DWORD dwExStyle = WS_EX_OVERLAPPEDWINDOW /*| WS_EX_LAYERED | WS_EX_TOOLWINDOW*/;
    RECT rcWin = {rect.left, rect.top, rect.right, rect.bottom};

    HWND hWnd = Create(0, &rcWin, nullptr, dwStyle, dwExStyle);
    SetWindowPos(0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
    return true;
}

void WindowContainer::DestroyContainer()
{
    DestroyWindow();
}

void* WindowContainer::GetHost()
{
    return m_hWnd;
}

void WindowContainer::SetCaption(const char* szCaption)
{
    if (szCaption)
    {
        std::wstring wstr;
        SetWindowText(wstr.c_str());
    }
}

void WindowContainer::SetBorder(const GRect& border)
{
    m_border = border;
}

void WindowContainer::SetMaxSize(const GSize& size)
{
    if (m_szMax != size)
    {
        m_szMax = size;
        if (IsWindow())
        {
            CRect rc(0, 0, 0, 0);
            GetWindowRect(&rc);
            GSize szWin(rc.Width(), rc.Height());
            GSize szNew = szWin;
            if (m_szMax.cx >= 0 && szWin.cx > m_szMax.cx)
            {
                szNew.cx = m_szMax.cx;
            }
            if (m_szMax.cy >= 0 && szWin.cy > m_szMax.cy)
            {
                szNew.cy = m_szMax.cy;
            }
            if (szWin != szNew)
            {
                uint flag = SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING;
                SetWindowPos(0, 0, 0, szNew.cx, szNew.cy, flag);
            }
        }
    }
}

void WindowContainer::SetMinSize(const GSize& size)
{
    if (m_szMin != size)
    {
        m_szMin = size;
        if (IsWindow())
        {
            CRect rc(0, 0, 0, 0);
            GetWindowRect(&rc);
            GSize szWin(rc.Width(), rc.Height());
            GSize szNew = szWin;
            if (m_szMin.cx >= 0 && szWin.cx < m_szMin.cx)
            {
                szNew.cx = m_szMin.cx;
            }
            if (m_szMin.cy >= 0 && szWin.cy < m_szMin.cy)
            {
                szNew.cy = m_szMin.cy;
            }
            if (szWin != szNew)
            {
                uint flag = SWP_NOZORDER | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING;
                SetWindowPos(nullptr, 0, 0, szNew.cx, szNew.cy, flag);
            }
        }
    }
}

void WindowContainer::SetCloseWindow(bool close)
{
    m_bCloseWindow = close;
}

void WindowContainer::SetTopMost(bool bTopMost)
{
    HWND hWndInsertAfter = bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST;
    SetWindowPos(hWndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool WindowContainer::IsTopMost()
{
    DWORD dwExStyle = GetExStyle();
    return (dwExStyle & WS_EX_TOPMOST) != 0;
}

void WindowContainer::SetForegroundWindow()
{
    ::SetForegroundWindow(m_hWnd);
}

int WindowContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return 0;
}

void WindowContainer::OnDestroy()
{
    Bind(nullptr);
}

void WindowContainer::OnPaint(CDCHandle dc)
{
    CPaintDC dcPaint(m_hWnd);
    if (!m_pView) return;

    GRect rcClient;
    GetRect(rcClient);
    rcClient.MoveToXY(0, 0);
    m_rcUpdate.IntersectRect(m_rcUpdate, rcClient);

    if (!m_rcUpdate.IsRectEmpty())
    {
        RECT rcClient2 = {m_rcUpdate.left, m_rcUpdate.top, m_rcUpdate.right, m_rcUpdate.bottom};
        dcPaint.FillRect(&rcClient2, 0);

        onPaint((HGCANVAS)(HDC)dcPaint, m_rcUpdate);

        //RECT rc = {m_rcUpdate.left, m_rcUpdate.top, m_rcUpdate.right, m_rcUpdate.bottom};
        //GUpdateWindow::GetInstance().Update(m_hWnd, 0, m_dib, &rc, 0xFF);
    }
    m_rcUpdate.SetRectEmpty();
}

BOOL WindowContainer::OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}

void WindowContainer::OnSize(UINT nType, CSize size)
{
    if (m_pView)
    {
        GSize szViewNew(size.cx, size.cy);
        GSize szView;
        m_pView->getSize(szView);
        if (szViewNew != szView)
        {
            m_pView->setSize(szViewNew);
        }
    }
    RebuildCanvas(size.cx, size.cy);
    m_rcUpdate.SetRect(0, 0, size.cx, size.cy);
    InvalidateRect(nullptr);
    UpdateWindow();
}

void WindowContainer::OnMove(CPoint ptPos)
{
    if (m_pView)
    {
        m_pView->setPos(GPoint(ptPos.x, ptPos.y));
    }
}

void WindowContainer::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bTrackingMouse)
    {
        TRACKMOUSEEVENT trackMouse = {0};
        trackMouse.cbSize = sizeof(trackMouse);
        trackMouse.hwndTrack = m_hWnd;
        trackMouse.dwFlags = TME_LEAVE;
        m_bTrackingMouse = TrackMouseEvent(&trackMouse);
    }

    if (!m_bMouseEnter)
    {
        onMouseEnter();
        m_bMouseEnter = true;
    }
    bool bHandled = false;
    GPoint pt(point.x, point.y);
    onMouseMove(pt, (uint)nFlags, bHandled);

    if (m_border.IsRectNull())
    {
        m_hCursor = NULL;
        return;
    }

    GRect rcWin;
    GetRect(rcWin);
    rcWin.MoveToXY(0, 0);

    GRect rcTop(rcWin.left, rcWin.top, rcWin.right, rcWin.top + m_border.top);
    GRect rcBottom(rcWin.left, rcWin.bottom - m_border.bottom, rcWin.right, rcWin.bottom);
    GRect rcLeft(rcWin.left, rcWin.top, rcWin.left + m_border.left, rcWin.bottom);
    GRect rcRight(rcWin.right - m_border.right, rcWin.top, rcWin.right, rcWin.bottom);

    if (rcTop.PtInRect(pt))
    {
        if (rcLeft.PtInRect(pt))
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE));
        }
        else if (rcRight.PtInRect(pt))
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW));
        }
        else
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
        }
    }
    else if (rcBottom.PtInRect(pt))
    {
        if (rcLeft.PtInRect(pt))
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW));
        }
        else if (rcRight.PtInRect(pt))
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE));
        }
        else
        {
            m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
        }
    }
    else if (rcLeft.PtInRect(pt) || rcRight.PtInRect(pt))
    {
        m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
    }
    else
    {
        m_hCursor = NULL;
    }
}

void WindowContainer::OnMouseLeave()
{
    m_bTrackingMouse = false;
    onMouseLeave();
    m_bMouseEnter = false;
}

BOOL WindowContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
    ScreenToClient(&point);
    GPoint pt(point.x, point.y);
    bool bHandled = false;
    onMouseWheel(pt, (uint)nFlags, zDelta / WHEEL_DELTA, bHandled);
    return TRUE;
}

void WindowContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_pView)
    {
        return;
    }

    if (GetCapture() != m_hWnd)
    {
        SetCapture();
    }

    GPoint pt(point.x, point.y);
    bool bHandled = false;
    onMouseDown(pt, GUIObject::MB_L, (uint)nFlags, bHandled);

    if (!bHandled)
    {
        ReleaseCapture();

        GRect rcWin;
        GetRect(rcWin);
        rcWin.MoveToXY(0, 0);

        if (m_border.IsRectNull())
        {
            ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
            return;
        }

        GRect rcTop(rcWin.left, rcWin.top, rcWin.right, rcWin.top + m_border.top);
        GRect rcBottom(rcWin.left, rcWin.bottom - m_border.bottom, rcWin.right, rcWin.bottom);
        GRect rcLeft(rcWin.left, rcWin.top, rcWin.left + m_border.left, rcWin.bottom);
        GRect rcRight(rcWin.right - m_border.right, rcWin.top, rcWin.right, rcWin.bottom);

        if (rcTop.PtInRect(pt))
        {
            if (rcLeft.PtInRect(pt))
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, 0);
            }
            else if (rcRight.PtInRect(pt))
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, 0);
            }
            else
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, 0);
            }
        }
        else if (rcBottom.PtInRect(pt))
        {
            if (rcLeft.PtInRect(pt))
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, 0);
            }
            else if (rcRight.PtInRect(pt))
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, 0);
            }
            else
            {
                ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, 0);
            }
        }
        else if (rcLeft.PtInRect(pt))
        {
            ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, 0);
        }
        else if (rcRight.PtInRect(pt))
        {
            ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, 0);
        }
        else
        {
            ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
        }
    }
}

void WindowContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (GetCapture() == m_hWnd)
    {
        ReleaseCapture();
    }

    GPoint pt(point.x, point.y);
    bool bHandled = false;
    onMouseUp(pt, GUIObject::MB_L, (uint)nFlags, bHandled);
}

void WindowContainer::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (GetCapture() != m_hWnd)
    {
        SetCapture();
    }

    bool bHandled = false;
    GPoint pt(point.x, point.y);
    onMouseDown(pt, GUIObject::MB_R, (uint)nFlags, bHandled);
}

void WindowContainer::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (GetCapture() == m_hWnd)
    {
        ReleaseCapture();
    }

    GPoint pt(point.x, point.y);
    bool bHandled = false;
    onMouseUp(pt, GUIObject::MB_R, (uint)nFlags, bHandled);
}

void WindowContainer::OnMButtonDown(UINT nFlags, CPoint point)
{
    if (GetCapture() != m_hWnd)
    {
        SetCapture();
    }

    bool bHandled = false;
    GPoint pt(point.x, point.y);
    onMouseDown(pt, GUIObject::MB_M, (uint)nFlags, bHandled);
}

void WindowContainer::OnMButtonUp(UINT nFlags, CPoint point)
{
    if (GetCapture() == m_hWnd)
    {
        ReleaseCapture();
    }

    GPoint pt(point.x, point.y);
    bool bHandled = false;
    onMouseUp(pt, GUIObject::MB_M, (uint)nFlags, bHandled);
}

void WindowContainer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    onKeyDown(nChar, nRepCnt, nFlags);
}

void WindowContainer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    onKeyUp(nChar, nRepCnt, nFlags);
}

LRESULT WindowContainer::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
{
    if (!bCalcValidRects)
    {
        RECT* lprc = (RECT*)(lParam);
        lprc->left = 0;
        lprc->top = 0;
        lprc->right = 0;
        lprc->bottom = 0;
    }
    return 0;
}

UINT WindowContainer::OnNcHitTest(CPoint point)
{
    return HTCLIENT;
}

void WindowContainer::OnNcPaint(CRgn rgn)
{
}

BOOL WindowContainer::OnNcActivate(BOOL bActive)
{
    return TRUE;
}

void WindowContainer::OnActivate(UINT uState, BOOL bMinimized, HWND hWnd)
{
    onActivate(uState == WA_INACTIVE ? false : true);
}

BOOL WindowContainer::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
    if (m_hCursor)
    {
        ::SetCursor(m_hCursor);
        SetMsgHandled(TRUE);
        return TRUE;
    }
    SetMsgHandled(FALSE);
    return FALSE; 
}

void WindowContainer::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
{
    if (m_szMax.cx >= 0)
    {
        lpMMI->ptMaxTrackSize.x = m_szMax.cx;
        lpMMI->ptMaxSize.x = m_szMax.cx;
    }

    if (m_szMax.cy >= 0)
    {
        lpMMI->ptMaxTrackSize.y = m_szMax.cy;
        lpMMI->ptMaxSize.x = m_szMax.cx;
    }

    if (m_szMin.cx >= 0)
    {
        lpMMI->ptMinTrackSize.x = m_szMin.cx;
    }

    if (m_szMin.cy >= 0)
    {
        lpMMI->ptMinTrackSize.y = m_szMin.cy;
    }
}

void WindowContainer::OnClose()
{
    SetMsgHandled(m_bCloseWindow ? FALSE : TRUE);
}

void WindowContainer::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    if (uFlags == SPI_SETWORKAREA)
    {
        RECT rc = {0, 0, 0, 0};
        GetWindowRect(&rc);
        GRect rcWin(rc.left, rc.top, rc.right, rc.bottom);
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
        GRect rcWorkArea(rc.left, rc.top, rc.right, rc.bottom);

        bool bChange = false;

        int x = rcWin.left;
        int y = rcWin.top;
        if (rcWin.right <= rcWorkArea.left)
        {
            x =  rcWorkArea.left;
            bChange = true;
        }

        if (rcWin.bottom <= rcWorkArea.top)
        {
            y =  rcWorkArea.top;
            bChange = true;
        }

        if (rcWin.left >= rcWorkArea.right)
        {
            x =  rcWorkArea.right - rcWin.Width();
            bChange = true;
        }

        if (rcWin.top >= rcWorkArea.bottom)
        {
            y =  rcWorkArea.bottom - rcWin.Height();
            bChange = true;
        }

        if (bChange)
        {
            SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING);
        }
    }
}

void WindowContainer::OnEnterSizeMove()
{
    SetMsgHandled(FALSE);
}

void WindowContainer::OnExitSizeMove()
{
    SetMsgHandled(FALSE);
}

LRESULT WindowContainer::OnInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LRESULT lRes = 0;
    if (m_pView)
    {
        GUIObject* pObj = m_pView->getFocusObject();
        if (pObj && pObj->isEnable() && pObj->isVisible())
        {
 
        }
    }
    return lRes;
}

void WindowContainer::OnFinalMessage(HWND hWnd)
{

}

void WindowContainer::Hide()
{
    if (m_pView)
    {
        if (!m_pView->isVisible())
        {
            ShowWindow(SW_HIDE);
        }
    }
}

void WindowContainer::Show()
{
    if (m_pView)
    {
        if (m_pView->isVisible())
        {
            ShowWindow(SW_SHOW);
            InvalidateRect(nullptr);
            UpdateWindow();
        }
    }
}

void WindowContainer::GetRect(GRect& rect) const
{
    if (IsWindow())
    {
        CRect rcWin;
        GetWindowRect(&rcWin);
        rect.SetRect(rcWin.left, rcWin.top, rcWin.right, rcWin.bottom);
    }
}

void WindowContainer::Update(const GRect& rect)
{
    m_rcUpdate.UnionRect(m_rcUpdate, rect);
    RECT rc = {rect.left, rect.top, rect.right, rect.bottom};
    InvalidateRect(&rc);
}

void WindowContainer::SyncRect()
{
    if (IsWindow() && m_pView)
    {
        GRect rcView;
        m_pView->getRect(rcView);
        CRect rcWin;
        GetWindowRect(&rcWin);
        if (rcView != GRect(rcWin.left, rcWin.top, rcWin.right, rcWin.bottom))
        {
            SetWindowPos(nullptr, rcView.left, rcView.top, rcView.Width(), rcView.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING);
        }
    }
}

void WindowContainer::RebuildCanvas(int cx, int cy)
{
    int x = cx + 1;
    int y = cy + 1;
    if (m_hCanvas)
    {
//        ::GCanvasDestroy(m_hCanvas);
        m_hCanvas = nullptr;
    }

    if (m_hBitmap)
    {
//        ::GBitmapDestroy(m_hBitmap);
        m_hBitmap = nullptr;
    }

//    m_hBitmap = ::GBitmapCreate(x, y);
//    m_hCanvas = ::GCanvasCreateFromBitmap(m_hBitmap);

//    ::GCanvasClear(m_hCanvas, nullptr);

    m_dib.Rebuild(x, y);
}

void WindowContainer::onActivate(bool bActivate)
{
    if (m_pView)
    {
        if (bActivate == false)
        {
            m_pObjFocus = m_pView->getFocusObject();
            m_pView->setFocusObject(nullptr);
        }
        else
        {
            m_pView->setFocusObject(m_pObjFocus);
            m_pObjFocus = nullptr;
        }
    }
}

