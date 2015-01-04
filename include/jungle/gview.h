#ifndef __GVIEW_H__
#define __GVIEW_H__

#include "gobject.h"

class GAPI GView : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GView)
public:
    GView(void);
    virtual ~GView(void);

    bool addChild(GView* child);
    bool delChild(GView* child);

protected:

protected:
    GView* m_pParent;
    GView* m_pNext;
    GView* m_pPrev;
    GView* m_pChild;
};

#endif