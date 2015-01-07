#ifndef __GSYSBUTTON_H__
#define __GSYSBUTTON_H__

#include "gbutton.h"
#include "gstacklayout.h"

class GAPI GSysButton : public GStackLayout
{
    DECLARE_GDYNAMIC_CLASS(GSysButton)
public:
    GSysButton();
    virtual ~GSysButton();

protected:
    GButton* m_pCloseButton;
    GButton* m_pMinButton;
    GButton* m_pMaxButton;
};

#endif