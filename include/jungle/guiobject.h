#ifndef __GUIOBJECT_H__
#define __GUIOBJECT_H__

#include "gobject.h"
#include "gtypes.h"
#include "gevent.h"

class GUIObject;

typedef void (Delegater::*OnClickEventFunc)(const GUIObject* sender, const GPoint& pt, uint button, uint keyState, uint clickTimes);
typedef void (Delegater::*OnMouseEventFunc)(const GUIObject* sender, const GPoint& pt, uint button, uint keyState);
typedef void (Delegater::*OnMouseMoveEventFunc)(const GUIObject* sender, const GPoint& pt, uint keyState);
typedef void (Delegater::*OnMouseWheelEventFunc)(const GUIObject* sender, const GPoint& pt, uint keyState, int16 zDelta);
typedef void (Delegater::*OnKeyEventFunc)(const GUIObject* sender, uint nChar, uint nRepCnt, uint nFlags);
typedef void (Delegater::*OnChangeEventFunc)(const GUIObject* sender, const GRect& rect);
typedef void (Delegater::*OnMoveEventFunc)(const GUIObject* sender, const GPoint& ptSrc, const GPoint& ptDst);
typedef void (Delegater::*OnResizeEventFunc)(const GUIObject* sender, const GSize& szOld, const GSize& szNew);

// UIObject不考虑界面父子关系，只考虑自身属性
// 关于on route 的约定
// 1. onXXX，是指自身的XXX事件发生了
// 2. onNameXXX，是指Name这个UI对象的XXX事件发生了
// 3. routeXXX，是指外部容器指派驱动XXX事件，并不代表XXX就一定会发生
// 4. 2的约定，可以视作为onSelfXXX简写为了onXXX
class GAPI GUIObject : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GUIObject)
public:
    enum MOUSE_BUTTON
    {
        MB_L = 0x01,
        MB_R = 0x02,
        MB_M = 0x10,
    };
    GUIObject(void);
    virtual ~GUIObject(void);

    void setId(const char* id);
    const char* getId(void) const;

    bool isHit(const GPoint& pt) const;

    bool isEnable() const;
    void setEnable(bool bEnable);
    bool isVisible() const;
    void setVisible(bool bVisible);

    void GetPos(GPoint& pt) const;
    void SetPos(const GPoint& pt);
    void GetRect(GRect& rect) const;
    void GetSize(GSize& size) const;
    void SetSize(const GSize& size);

    virtual void Update(const GRect* rc = NULL);

    void onClick(const GPoint& pt, uint button, uint keyState, uint clickTimes, bool& bHandled);
    void onMouseDown(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseUp(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseMove(const GPoint& pt, uint keyState, bool& bHandled);
    void onMouseHover();
    void onMouseEnter();
    void onMouseLeave();
    void onMouseWheel(const GPoint& pt, uint keyState, int16 zDelta, bool& bHandled);
    void onKeyDown(uint nChar, uint nRepCnt, uint nFlags);
    void onKeyUp(uint nChar, uint nRepCnt, uint nFlags);

    void onMove(const GPoint& ptOld, const GPoint& ptNew);
    void onResize(const GSize& szOld, const GSize& szNew);

    void onVisibleChange();
    void onEnableChange();

    void onPaint(HGCANVAS hCanvas);

public:
    EventSource<OnChangeEventFunc> OnChange;
    EventSource<OnResizeEventFunc> OnResize;
    EventSource<OnMoveEventFunc>   OnMove;

protected:
    GPoint m_pos;
    GSize  m_size;

    std::string m_id;

    bool m_bVisible;
    bool m_bEnable;

    bool m_bEnter;
    bool m_bMouseDown;
};

#endif
