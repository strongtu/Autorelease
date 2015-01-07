#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "gview.h"


class Container;
class GSysButton;

class GAPI GWindow : public GView
{
    DECLARE_GDYNAMIC_CLASS(GWindow)
public:
    GWindow();
    virtual ~GWindow();

public:
    void setCaption(const char* szCaption);
    void setBorder(const GRect& border);
    void setMaxSize(const GSize& size);
    void setMinSize(const GSize& size);
    void closeWindow();
    void setForegroundWindow();
    void setTopMost(bool bTopMost);
    bool isTopMost();

public:
    virtual void setVisible(bool bVisible);

protected:
    void onButtonClick(const GUIObject* sender, const GPoint& pt, uint button, uint keyState, uint clickTimes);

    Container* GetContainer();

protected:
    GSize m_szMax;
    GSize m_szMin;
    Container* m_pContainer;

    GSysButton* m_pSysButton;
};

#endif /*__WINDOW_CONTROLLER_H__*/