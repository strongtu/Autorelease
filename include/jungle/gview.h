#ifndef __GVIEW_H__
#define __GVIEW_H__

#include "guiobject.h"

class GAPI GView : public GUIObject
{
    DECLARE_GDYNAMIC_CLASS(GView)
public:
    GView(void);
    virtual ~GView(void);

    bool addChild(GView* child);
    bool delChild(GView* child);

    GView* getChild(const char* id) const;
    GView* getFirstChild(void) const;
    GView* getLastChild(void) const;

    GView* getParent(void) const;
    GView* getRoot(void) const;
    GView* getNext(void) const;
    GView* getPrev(void) const;

    GView* childHitTest(const GPoint& pt) const;

    GView* getFocusObject();
    void setFocusObject(GView* obj);

    void update(const GRect* rc = nullptr);

protected:
    // 界面树
    GView* m_pParent;
    GView* m_pNext;
    GView* m_pPrev;
    GView* m_pChild;

    // 界面状态
    GView* m_pObjMouseDown;
    GView* m_pObjMouseEnter;
    GView* m_pObjFocus;
};

#endif