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
    void SetCaption(const char* szCaption);
    void SetBorder(const GRect& border);
    void SetMaxSize(const GSize& size);
    void SetMinSize(const GSize& size);
    void ShowWindow();
    void HideWindow();
    void CloseWindow();
    void SetForegroundWindow();
    void SetTopMost(bool bTopMost);
    bool IsTopMost();

protected:
    virtual void Move(const GPoint& ptSrc, const GPoint& ptDst);
    virtual void Resize(const GSize& szOld, const GSize& szNew);
    void OnVisibleChangeEvent(const GUIObject* sender);
    void OnEnableChangeEvent(const GUIObject* sender);
    void OnCloseClick(const GUIObject* sender, const GPoint& pt, uint button, uint keyState, uint clickTimes);
    void OnFinalMessage(const Container* sender);
    void OnClose(const Container* sender);
    void OnContainerActivate(const Container* sender, bool bActivate);

protected:
    Container* GetContainer();

protected:
    bool m_bMainWindow;
    GSize m_szMax;
    GSize m_szMin;
    Container* m_pContainer;
};

#endif /*__WINDOW_CONTROLLER_H__*/