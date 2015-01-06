#ifndef __WINDOWCONTAINER_H__
#define __WINDOWCONTAINER_H__

#include "./winupdate.h"
#include "./winhdr.h"
#include "./windib.h"
#include "./container.h"

class WindowContainer : public Container,
                        public CWindowImpl<WindowContainer>
{
public:
    WindowContainer(void);
    virtual ~WindowContainer(void);
public:
    virtual bool CreateContainer(const GRect& rect);
    virtual void DestroyContainer();
    virtual void* GetHost();
    virtual void Hide();
    virtual void Show();
    virtual void SetRect(const GRect& rect);
    virtual void GetRect(GRect& rect) const;
    virtual void SetPos(const GPoint& pt);
    virtual void GetPos(GPoint& pt) const;
    virtual void SetSize(const GSize& sz);
    virtual void GetSize(GSize& sz) const;
    virtual void SetCaption(const char* szCaption);
    virtual void SetBorder(const GRect& border);
    virtual void SetMaxSize(const GSize& size);
    virtual void SetMinSize(const GSize& size);
    virtual void SetCloseWindow(bool close);
    virtual void SetTopMost(bool bTopMost);
    virtual bool IsTopMost();
    virtual void SetForegroundWindow();

public:
    DECLARE_WND_CLASS(L"GWin")

    BEGIN_MSG_MAP_EX(WindowContainer)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_SIZE(OnSize)
        MSG_WM_MOVE(OnMove)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        MSG_WM_MOUSEWHEEL(OnMouseWheel)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
        MSG_WM_RBUTTONDOWN(OnRButtonDown)
        MSG_WM_RBUTTONUP(OnRButtonUp)
        MSG_WM_RBUTTONDBLCLK(OnRButtonDown)
        MSG_WM_MBUTTONDOWN(OnMButtonDown)
        MSG_WM_MBUTTONUP(OnMButtonUp)
        MSG_WM_MBUTTONDBLCLK(OnMButtonDown)
        MSG_WM_KEYDOWN(OnKeyDown)
        MSG_WM_KEYUP(OnKeyUp)
        MSG_WM_NCCALCSIZE(OnNcCalcSize)
        MSG_WM_NCHITTEST(OnNcHitTest)
        MSG_WM_NCPAINT(OnNcPaint)
        MSG_WM_NCACTIVATE(OnNcActivate)
        MSG_WM_ACTIVATE(OnActivate)
        MSG_WM_SETCURSOR(OnSetCursor)
        MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_SETTINGCHANGE(OnSettingChange)
        MSG_WM_ENTERSIZEMOVE(OnEnterSizeMove);
        MSG_WM_EXITSIZEMOVE(OnExitSizeMove);

        MESSAGE_HANDLER(WM_CHAR, OnInputMessage)
        MESSAGE_HANDLER(WM_SYSCHAR, OnInputMessage)
        MESSAGE_HANDLER(WM_SYSKEYDOWN, OnInputMessage)
        MESSAGE_HANDLER(WM_SYSKEYUP, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_SETCONTEXT, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_STARTCOMPOSITION, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_COMPOSITION, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_CONTROL, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_ENDCOMPOSITION, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_COMPOSITIONFULL, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_NOTIFY, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_SELECT, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_CHAR, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_REQUEST, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_KEYDOWN, OnInputMessage)
        MESSAGE_HANDLER(WM_IME_KEYUP, OnInputMessage)
    END_MSG_MAP()
protected:
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();
    void OnPaint(CDCHandle dc);
    BOOL OnEraseBkgnd(CDCHandle dc);
    void OnSize(UINT nType, CSize size);
    void OnMove(CPoint ptPos);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnMouseLeave();
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnRButtonDown(UINT nFlags, CPoint point);
    void OnRButtonUp(UINT nFlags, CPoint point);
    void OnMButtonDown(UINT nFlags, CPoint point);
    void OnMButtonUp(UINT nFlags, CPoint point);
    void OnXButtonDown(int fwButton, int dwKeys, CPoint ptPos);
    void OnXButtonUp(int fwButton, int dwKeys, CPoint ptPos);
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
    UINT OnNcHitTest(CPoint point);
    void OnNcPaint(CRgn rgn);
    BOOL OnNcActivate(BOOL bActive);
    void OnActivate(UINT uState, BOOL bMinimized, HWND hWnd);
    BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
    void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);
    void OnClose();
    void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    void OnEnterSizeMove();
    void OnExitSizeMove();
    LRESULT OnInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void OnFinalMessage(HWND hWnd);
protected:
    virtual void Update(const GRect& rect);
    virtual void SyncRect();
    void RebuildCanvas(int cx, int cy);
protected:
    void DoActivate(bool bActivate);
    void DoFinalMessage();
    void DoClose();
protected:
    HGCANVAS m_hCanvas;
    HGBITMAP m_hBitmap;
    DIB m_dib;
    bool m_bCloseWindow;
    bool m_bMouseEnter;
    BOOL m_bTrackingMouse;
    HCURSOR m_hCursor;
    GRect m_rcUpdate;
    GRect m_border;
    GSize m_szMax;
    GSize m_szMin;
};

#endif /*__WINDOWCONTAINER_H__*/