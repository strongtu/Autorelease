#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gview.h"

enum ControlState
{
    CS_NORMAL    = 0,
    CS_HIGHLIGHT = 1,
    CS_PUSHED    = 2,
};

class GAPI GButton : public GView
{
    DECLARE_GDYNAMIC_CLASS(GButton)
public:
    GButton();
    virtual ~GButton();

public:
    void setCaption(const char* caption);
    const char* getCaption(void);

    void onMouseDown(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseUp(const GPoint& pt, uint button, uint keyState, bool& bHandled);
    void onMouseEnter();
    void onMouseLeave();

    void onPaint(HGCANVAS hCanvas);

protected:
    std::string     m_caption;
    int             m_state;
};

#endif
