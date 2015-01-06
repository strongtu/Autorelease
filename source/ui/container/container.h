#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "gview.h"
#include "gtypes.h"
#include "event.h"

class Container;
typedef void (Delegater::*OnStateEventFunc)(const Container* sender, bool bState);

class Container : public GObject
{
public:
    Container(void);
    virtual ~Container(void);
public:
    virtual bool CreateContainer(const GRect& rect) = 0;
    virtual void DestroyContainer() = 0;
    bool Bind(GView* pView);
    virtual void* GetHost() = 0;
    virtual void Hide() = 0;
    virtual void Show() = 0;
    virtual void SetRect(const GRect& rect) = 0;
    virtual void GetRect(GRect& rect) const = 0;
    virtual void SetPos(const GPoint& pt) = 0;
    virtual void GetPos(GPoint& pt) const = 0;
    virtual void SetSize(const GSize& sz) = 0;
    virtual void GetSize(GSize& sz) const = 0;
    virtual void SetCaption(const char* szCaption) = 0;
    virtual void SetBorder(const GRect& border) = 0;
    virtual void SetMaxSize(const GSize& size) = 0;
    virtual void SetMinSize(const GSize& size) = 0;
    virtual void SetCloseWindow(bool close) = 0;
    virtual void SetTopMost(bool bTopMost) = 0;
    virtual bool IsTopMost() = 0;
    virtual void SetForegroundWindow() = 0;
protected:
    void ListenViewEvent();
    void UnlistenViewEvent();
protected:
    void onViewChange(const GUIObject* sender, const GRect& rect);
    void onViewResize(const GUIObject* sender, const GSize& szOld, const GSize& szNew);
    bool CheckView();
protected:
    virtual void Update(const GRect& rect) = 0;
    virtual void SyncRect() = 0;

protected:
    void onClick(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseDown(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseUp(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseMove(const GPoint& pt, uint keyState, bool& bHandled);
    void onMouseEnter();
    void onMouseLeave();
    void onMouseWheel(const GPoint& pt, uint keyState, int16 zDelta, bool& bHandled);
    void onKeyDown(uint nChar, uint nRepCnt, uint nFlags);
    void onKeyUp(uint nChar, uint nRepCnt, uint nFlags);
    void onPaint(HGCANVAS hCanvas, const GRect& rcRender);

public:
    EventSource<OnStateEventFunc> OnActivateEvent;

protected:
    GView*  m_pView;
    GView*  m_pObjMouseIn;
    GView*  m_pObjMouseDown;
    GView*  m_pObjFocus;
    uint32  m_uLastClickTime;
    uint32  m_uLastClickTimes;
    GPoint  m_ptLastClickPos;
    uint    m_uLastButton;
};

#endif /*__CONTAINER_H__*/
