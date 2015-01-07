#include "stdafx.h"
#include "gwindow.h"
#include "ui/container/windowcontainer.h"
#include "gsysbutton.h"

IMPLEMENT_GDYNAMIC_CLASS(GWindow, GView)

GWindow::GWindow() 
: m_szMax(-1, -1)
, m_szMin(-1, -1)
, m_pContainer(nullptr)
{
    m_pSysButton = (GSysButton*)GSysButton::createObject();
    m_pSysButton->setSize(GSize(150, 50));
    addChild(m_pSysButton);

    GView* p = m_pSysButton->getChild("closebutton");
    p->OnClick += EventObject<OnClickEventFunc>(this, &GWindow::onButtonClick);
    p = m_pSysButton->getChild("minbutton");
    p->OnClick += EventObject<OnClickEventFunc>(this, &GWindow::onButtonClick);
    p = m_pSysButton->getChild("maxbutton");
    p->OnClick += EventObject<OnClickEventFunc>(this, &GWindow::onButtonClick);
}

GWindow::~GWindow()
{
}

void GWindow::setCaption(const char* szCaption)
{
    if (!szCaption) return;

    Container* p = GetContainer();
    if (p)
    {
        p->SetCaption(szCaption);
    }
}

void GWindow::setBorder(const GRect& border)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetBorder(border);
    }
}

void GWindow::setMaxSize(const GSize& size)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetMaxSize(size);
    }
}

void GWindow::setMinSize(const GSize& size)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetMinSize(size);
    }
}

void GWindow::closeWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->DestroyContainer();
    }
}

void GWindow::setForegroundWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetForegroundWindow();
    }
}

bool GWindow::isTopMost()
{
    Container* p = GetContainer();
    if (!p) return false;

    return p->IsTopMost();
}

void GWindow::setTopMost(bool bTopMost)
{
    Container* p = GetContainer();
    if (!p) return;
    if (isTopMost() == bTopMost)
    {
        return;
    }

    if (bTopMost)
    {
        p->SetForegroundWindow();
        p->SetTopMost(true);
    }
    else
    {
        p->SetTopMost(false);
    }
}

void GWindow::setVisible(bool bVisible)
{
    GView::setVisible(bVisible);

    Container* p = GetContainer();
    if (!p) return;
    if (bVisible)
    {
        p->Show();
    }
    else
    {
        p->Hide();
    }
}

Container* GWindow::GetContainer()
{
    if (m_pContainer)
    {
        return m_pContainer;
    }

    m_pContainer = new WindowContainer();

    GRect rc(0, 0, 0, 0);
    this->getRect(rc);

    m_pContainer->CreateContainer(rc);
    m_pContainer->Bind(this);

    return m_pContainer;
}

void GWindow::onButtonClick(const GUIObject* sender, const GPoint& pt, uint button, uint keyState, uint clickTimes)
{

}