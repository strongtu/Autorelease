#include "gview.h"

IMPLEMENT_GDYNAMIC_CLASS(GView, GUIObject)

GView::GView(void)
: m_pParent(NULL)
, m_pNext(NULL)
, m_pPrev(NULL)
, m_pChild(NULL)
{
}

GView::~GView(void)
{
}

bool GView::addChild(GView* child)
{
    if (!child) return false;
    if (child->m_pParent == this) return false;

    GView* pObj = this;
    while(pObj)
    {
        if (pObj == child) return false;
        pObj = pObj->getParent();
    }

    child->retain();
    GView* pParent = child->getParent();
    if (pParent) pParent->delChild(child);

    GView* pLastChild = getLastChild();
    if (pLastChild)
    {
        pLastChild->m_pNext = child;
        child->m_pPrev = pLastChild;
    }
    else
    {
        m_pChild = child;
    }

    child->m_pParent = this;

    return true;
}

bool GView::delChild(GView* child)
{
    GView* pPrev = child->m_pPrev;
    GView* pNext = child->m_pNext;
    if (pPrev)
    {
        m_pPrev->m_pNext = child->m_pNext;
    }
    else
    {
        m_pChild = pNext;
    }
    if (pNext)
    {
        pNext->m_pPrev = pPrev;
    }

    child->m_pPrev = nullptr;
    child->m_pNext = nullptr;

    child->m_pParent = nullptr;

    child->release();

    return true;
}

GView* GView::getChild(const char* id) const
{
    if (!id) return nullptr;

    GView* p = getFirstChild();
    while(p)
    {
        if (p->m_id == id)
        {
            return p;
        }
        p = p->getNext();
    }

    return p;
}

GView* GView::getFirstChild(void) const
{
    return m_pChild;
}

GView* GView::getLastChild(void) const
{
    GView* p = m_pChild;
    while(p && p->m_pNext)
    {
        p = p->m_pNext;
    }
    return p;
}

GView* GView::getParent(void) const
{
    return m_pChild;
}

GView* GView::getRoot(void) const
{
    if (!m_pParent) return const_cast<GView*>(this);

    GView* p = m_pParent;
    while(p && p->m_pParent)
    {
        p = p->m_pParent;
    }
    return p;
}

GView* GView::getNext(void) const
{
    return m_pNext;
}
GView* GView::getPrev(void) const
{
    return m_pPrev;
}

GView* GView::childHitTest(const GPoint& pt) const
{
    GView* pObj = getLastChild();
    while(pObj)
    {
        if (pObj->isHit(pt))
        {
            GPoint childPt(pt);
            childPt -= pObj->m_pos;
            return pObj->childHitTest(childPt);
        }
        pObj = pObj->m_pPrev;
    }

    return pObj ? pObj : const_cast<GView*>(this);
}

GView* GView::getFocusObject()
{
    return m_pObjFocus;
}

void GView::setFocusObject(GView* obj)
{
    m_pObjFocus = obj;
}

void GView::Update(const GRect* rc)
{
    GRect rcUpdate(0, 0, m_size.cx, m_size.cy);
    if (rc) rcUpdate.IntersectRect(&rcUpdate, rc);
    if (rcUpdate.IsRectEmpty()) return;

    if (m_pParent)
    {
        rcUpdate += m_pos;
        m_pParent->Update(&rcUpdate);
    }
    else
    {
        GFireEvent(OnChangeEventFunc, OnChange, (this, rcUpdate));
    }
}