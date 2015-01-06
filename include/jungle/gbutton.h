#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gview.h"

class GAPI GButton : public GView
{
    DECLARE_GDYNAMIC_CLASS(GButton)
public:
    GButton();
    virtual ~GButton();
};

#endif
