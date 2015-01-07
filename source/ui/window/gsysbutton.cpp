#include "stdafx.h"
#include "gsysbutton.h"

IMPLEMENT_GDYNAMIC_CLASS(GSysButton, GStackLayout);

GSysButton::GSysButton()
{
    m_pCloseButton = (GButton*)GButton::createObject();
    m_pMinButton = (GButton*)GButton::createObject();
    m_pMaxButton = (GButton*)GButton::createObject();

    m_pCloseButton->setSize(GSize(50,50));
    m_pCloseButton->setId("closebutton");
    m_pMinButton->setSize(GSize(50,50));
    m_pMinButton->setId("minbutton");
    m_pMaxButton->setSize(GSize(50,50));
    m_pMaxButton->setId("maxbutton");

    addChild(m_pCloseButton);
    addChild(m_pMinButton);
    addChild(m_pMaxButton);
}

GSysButton::~GSysButton()
{
    m_pCloseButton->release();
    m_pMinButton->release();
    m_pMaxButton->release();
}

