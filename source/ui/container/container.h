#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "gview.h"
#include "gtypes.h"
#include "gevent.h"

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

    void onViewChange(const GUIObject* sender, const GRect& rect);
    void onViewResize(const GUIObject* sender, const GSize& szOld, const GSize& szNew);
    void onViewMove(const GUIObject* sender, const GPoint& ptSrc, const GPoint& ptDst);

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

protected:
    GView*  m_pView;
    GView*  m_pObjMouseIn;
    GView*  m_pObjMouseDown;
    GView*  m_pObjFocus;
};

#endif /*__CONTAINER_H__*/
