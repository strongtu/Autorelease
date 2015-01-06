#include "stdafx.h"
#include "./container.h"

#include <windows.h>

Container::Container(void)
: m_pView(nullptr)
, m_pObjMouseIn(nullptr)
, m_pObjMouseDown(nullptr)
, m_pObjFocus(nullptr)
{
}

Container::~Container(void)
{
    if (m_pView)
    {
        UnlistenViewEvent();
        m_pView->release();
        m_pView = nullptr;
    }
}

bool Container::Bind(GView* pView)
{
    if (m_pView != pView)
    {
        if (m_pView)
        {
            UnlistenViewEvent();
            m_pView->release();
            m_pView = nullptr;
        }
        m_pView = pView;
        if (pView)
        {
            pView->retain();
            ListenViewEvent();
            SyncRect();
        }
        return true;
    }
    return false;
}

void Container::ListenViewEvent()
{
    if (m_pView)
    {
        m_pView->OnChange += EventObject<OnChangeEventFunc>(this, &Container::onViewChange);
        m_pView->OnResize += EventObject<OnResizeEventFunc>(this, &Container::onViewResize);
        m_pView->OnMove += EventObject<OnMoveEventFunc>(this, &Container::onViewMove);
    }
}

void Container::UnlistenViewEvent()
{
    if (m_pView)
    {
        m_pView->OnChange -= EventObject<OnChangeEventFunc>(this, &Container::onViewChange);
        m_pView->OnResize -= EventObject<OnResizeEventFunc>(this, &Container::onViewResize);
        m_pView->OnMove -= EventObject<OnMoveEventFunc>(this, &Container::onViewMove);
    }
}

void Container::onViewChange(const GUIObject* sender, const GRect& rect)
{
    Update(rect);
}

void Container::onViewResize(const GUIObject* sender, const GSize& szOld, const GSize& szNew)
{
    SyncRect();
    GRect rcView;
    m_pView->getRect(rcView);
    Update(rcView);
}

void Container::onViewMove(const GUIObject* sender, const GPoint& ptSrc, const GPoint& ptDst)
{
    SyncRect();
}

bool Container::CheckView()
{
    return (m_pView && m_pView->isEnable() && m_pView->isVisible());
}

void Container::onMouseDown(const GPoint& pt, uint button, uint keyState, bool& bHandled)
{
    if (!CheckView()) return;

    GView* pView = m_pView->childHitTest(pt);
    if (pView != m_pObjMouseIn)
    {
        if (m_pObjMouseIn)
        {
            m_pObjMouseIn->onMouseLeave();
            m_pObjMouseIn = nullptr;
        }

        m_pObjMouseIn = pView;
        if (m_pObjMouseIn)
        {
            m_pObjMouseIn->onMouseEnter();
        }
    }

    m_pObjMouseDown = pView;
    if (m_pObjMouseDown)
    {
        m_pObjMouseDown->onMouseDown(pt, button, keyState, bHandled);
    }
}

void Container::onMouseUp(const GPoint& pt, uint button, uint keyState, bool& bHandled)
{
    if (!CheckView()) return;

    GView* pView = m_pView->childHitTest(pt);

    if (m_pObjMouseDown)
    {
        m_pObjMouseDown->onMouseUp(pt, button, keyState, bHandled);
    }

    // 模拟click事件
    if (pView && pView == m_pObjMouseDown)
    {
        m_pObjMouseDown->onClick(pt, button, keyState, 0, bHandled);
    }

    m_pObjMouseDown = nullptr;
}

void Container::onMouseMove(const GPoint& pt, uint keyState, bool& bHandled)
{
    if (!CheckView()) return;

    GView* pView = m_pView->childHitTest(pt);
    if (m_pObjMouseIn != pView)
    {
        if (m_pObjMouseIn)
        {
            m_pObjMouseIn->onMouseLeave();
            m_pObjMouseIn = nullptr;
        }

        if (pView)
        {
            pView->onMouseEnter();
            m_pObjMouseIn = pView;
        }
    }

    if (m_pObjMouseIn)
    {
        m_pObjMouseIn->onMouseMove(pt, keyState, bHandled);
    }
}

void Container::onMouseEnter()
{

}

void Container::onMouseLeave()
{
    if (!CheckView()) return;
    
    if (m_pObjMouseIn)
    {
        m_pObjMouseIn->onMouseLeave();
        m_pObjMouseIn = nullptr;
    }
}

void Container::onMouseWheel(const GPoint& pt, uint keyState, int16 zDelta, bool& bHandled)
{
    if (!CheckView()) return;

    GView* pView = m_pView->childHitTest(pt);
    if (pView)
    {
        pView->onMouseWheel(pt, keyState, zDelta, bHandled);
    }

    if (!bHandled)
    {
        GView* pObj = m_pView->getFocusObject();
        if (pObj && pObj->isEnable() && pObj->isVisible())
        {
            pObj->onMouseWheel(pt, keyState, zDelta, bHandled);
        }
    }
}

void Container::onKeyDown(uint nChar, uint nRepCnt, uint nFlags)
{
    if (CheckView())
    {
        GUIObject* pObj = m_pView->getFocusObject();
        if (pObj && pObj->isEnable() && pObj->isVisible())
        {
            pObj->onKeyDown(nChar, nRepCnt, nFlags);
        }
    }
}

void Container::onKeyUp(uint nChar, uint nRepCnt, uint nFlags)
{
    if (CheckView())
    {
        GUIObject* pObj = m_pView->getFocusObject();
        if (pObj && pObj->isEnable() && pObj->isVisible())
        {
            pObj->onKeyUp(nChar, nRepCnt, nFlags);
        }
    }
}

void Container::onPaint(HGCANVAS hCanvas, const GRect& rcRender)
{
    if (m_pView)
    {
        m_pView->onPaint(hCanvas);

        GView* child = m_pView->getFirstChild();
        while(child)
        {
            HDC hdc = (HDC)hCanvas;
            POINT ptViewport;
            ::GetWindowOrgEx(hdc, &ptViewport);

            GPoint pos;
            child->getPos(pos);
            POINT ptViewportNew = {ptViewport.x - pos.x, ptViewport.y - pos.y};
            ::SetWindowOrgEx(hdc, ptViewportNew.x, ptViewportNew.y, NULL);

            child->onPaint(hCanvas);

            ::SetWindowOrgEx(hdc, ptViewport.x, ptViewport.y, NULL);
            child = child->getNext();
        }
    }
}
