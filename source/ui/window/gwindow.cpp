#include "gwindow.h"
#include "ui/container/windowcontainer.h"

IMPLEMENT_GDYNAMIC_CLASS(GWindow, GView)

GWindow::GWindow() :
    m_bMainWindow(false),
    m_szMax(-1, -1),
    m_szMin(-1, -1),
    m_pContainer(nullptr)
{
}

GWindow::~GWindow()
{
}

void GWindow::SetCaption(const char* szCaption)
{
    if (!szCaption)
    {
        return;
    }

    Container* p = GetContainer();
    if (p)
    {
        p->SetCaption(szCaption);
    }
}

void GWindow::SetBorder(const GRect& border)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetBorder(border);
    }
}

void GWindow::SetMaxSize(const GSize& size)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetMaxSize(size);
    }
}

void GWindow::SetMinSize(const GSize& size)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetMinSize(size);
    }
}

bool GWindow::IsTopMost()
{
    Container* p = GetContainer();
    if (!p) return false;

    return p->IsTopMost();
}

void GWindow::SetTopMost(bool bTopMost)
{
    Container* p = GetContainer();
    if (!p) return;
    if (IsTopMost() == bTopMost)
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

void GWindow::ShowWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->Show();
    }
}

void GWindow::HideWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->Hide();
    }
}

void GWindow::CloseWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->DestroyContainer();
    }
}

void GWindow::SetForegroundWindow()
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetForegroundWindow();
    }
}

void GWindow::Move(const GPoint& ptSrc, const GPoint& ptDst)
{
    Container* p = GetContainer();
    if (p)
    {
        p->SetPos(ptDst);
    }
}

void GWindow::Resize(const GSize& szOld, const GSize& szNew)
{
    Container* p = GetContainer();
    if (p)
    {
        GSize szWin = szNew;
        if (m_szMin.cx >= 0 && szNew.cx < m_szMin.cx)
        {
            szWin.cx = m_szMin.cx;
        }
        if (m_szMin.cy >= 0 && szNew.cy < m_szMin.cy)
        {
            szWin.cy = m_szMin.cy;
        }
        if (m_szMax.cx >= 0 && szNew.cx > m_szMax.cx)
        {
            szWin.cx = m_szMax.cx;
        }
        if (m_szMax.cy >= 0 && szNew.cy > m_szMax.cy)
        {
            szWin.cy = m_szMax.cy;
        }
        p->SetSize(szWin);
    }
}

void GWindow::OnVisibleChangeEvent(const GUIObject* sender)
{
    bool bVisible = sender->isVisible();
    if (bVisible)
    {
        ShowWindow();
    }
    else
    {
        HideWindow();
    }
}

void GWindow::OnEnableChangeEvent(const GUIObject* sender)
{
    bool bEnable = sender->isEnable();
}

void GWindow::OnCloseClick(const GUIObject* sender, const GPoint& pt, uint button, uint keyState, uint clickTimes)
{
    CloseWindow();
}

void GWindow::OnFinalMessage(const Container* sender)
{
    if (sender == m_pContainer)
    {
        m_pContainer->Bind(nullptr);
        m_pContainer->release();
        m_pContainer = nullptr;

        if (m_bMainWindow)
        {
            PostQuitMessage(0);
        }
    }
}

void GWindow::OnContainerActivate(const Container* sender, bool bActivate)
{
    if (sender == m_pContainer)
    {

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
    this->GetRect(rc);

    m_pContainer->CreateContainer(rc);
    m_pContainer->OnActivateEvent += EventObject<OnStateEventFunc>(this, &GWindow::OnContainerActivate);

    m_pContainer->Bind(this);

    return m_pContainer;
}