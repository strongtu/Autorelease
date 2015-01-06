#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "gview.h"

class Container;

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
    Container* GetContainer();

protected:
    GSize m_szMax;
    GSize m_szMin;
    Container* m_pContainer;
};

#endif /*__WINDOW_CONTROLLER_H__*/