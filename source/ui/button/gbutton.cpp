#include "stdafx.h"
#include "gbutton.h"

IMPLEMENT_GDYNAMIC_CLASS(GButton, GView);

GButton::GButton()
: m_state(CS_NORMAL)
{
}

GButton::~GButton()
{
}

void GButton::onMouseDown(const GPoint& pt, uint button, uint keyState, bool& bHandled)
{
    bHandled = true;

    GView::onMouseDown(pt, button, keyState, bHandled);

    int oldState = m_state;
    m_state = CS_PUSHED;
    if (oldState != m_state)
    {
        update();
    }
}

void GButton::onMouseUp(const GPoint& pt, uint button, uint keyState, bool& bHandled)
{
    GView::onMouseUp(pt, button, keyState, bHandled);

    int oldState = m_state;
    m_state = CS_HIGHLIGHT;
    if (oldState != m_state)
    {
        update();
    }
}

void GButton::onMouseEnter()
{
    GView::onMouseEnter();

    int oldState = m_state;
    if (m_state == CS_NORMAL) m_state = CS_HIGHLIGHT;
    if (oldState != m_state)
    {
        update();
    }
}

void GButton::onMouseLeave()
{
    GView::onMouseLeave();

    int oldState = m_state;
    m_state = CS_NORMAL;
    if (oldState != m_state)
    {
        update();
    }
}

void GButton::onPaint(HGCANVAS hCanvas)
{
    HDC hdc = (HDC)hCanvas;
    RECT rc = {0,0,m_size.cx,m_size.cy};
    if (m_state == CS_PUSHED)
    {
        
        ::FillRect(hdc,&rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    }
    else if (m_state == CS_HIGHLIGHT)
    {
        ::SelectObject(hdc, ::GetStockObject(WHITE_PEN));

        ::MoveToEx(hdc, 0, 0, NULL);
        ::LineTo(hdc, m_size.cx, m_size.cy);
        ::MoveToEx(hdc, m_size.cx, 0, NULL);
        ::LineTo(hdc, 0, m_size.cy);
    }
    else
    {
        ::SelectObject(hdc, ::GetStockObject(BLACK_PEN));

        ::MoveToEx(hdc, 0, 0, NULL);
        ::LineTo(hdc, m_size.cx, m_size.cy);
        ::MoveToEx(hdc, m_size.cx, 0, NULL);
        ::LineTo(hdc, 0, m_size.cy);
    }

    ::DrawTextA(hdc, m_id.c_str(), m_id.length(), &rc, DT_CENTER|DT_VCENTER);
}