#include "stdafx.h"
#include "gstacklayout.h"

IMPLEMENT_GDYNAMIC_CLASS(GStackLayout, GView);

GStackLayout::GStackLayout(void)
{
}

GStackLayout::~GStackLayout(void)
{
}

void GStackLayout::onResize(const GSize& szOld, const GSize& szNew)
{
    GView::onResize(szOld, szNew);

    layout();
}

void GStackLayout::layout(void)
{
    GPoint pt(0,0);

    GView* child = getFirstChild();
    while(child)
    {
        child->setPos(pt);
        GSize sz;
        child->getSize(sz);

        pt.x += sz.cx;

        child = child->getNext();
    };
}