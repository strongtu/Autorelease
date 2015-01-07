// test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test.h"

#include "gobject.h"
#include "gatpool.h"
#include "gdata.h"
#include "gwindow.h"
#include "gbutton.h"

DWORD g_t1 = 0;
DWORD g_t2 = 0;

void TestData()
{
    GAutoreleasePool * atp = (GAutoreleasePool*)GAutoreleasePool::createObject();
    atp->release();

    GData* pdata = (GData*)GData::createObject();

    DWORD dw = GetTickCount();
    for (int i = 0; i < 1000000; i++)
    {
        pdata->setBool("b123456789", true);
        pdata->delField("b123456789");
    }
    g_t1 = GetTickCount() - dw;

    pdata->setBool("b3123456789", TRUE);
    pdata->setInt("i2123456789", 100);

    for (int i = 0; i < 1000000; i++)
    {
        bool b = pdata->getBool("b3123456789");
        int k = pdata->getInt("i2123456789");
    }

    g_t2 = GetTickCount() - dw;

    pdata->release();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    GWindow *pWindow = (GWindow*)GWindow::createObject();

    pWindow->setPos(GPoint(10, 10));
    pWindow->setSize(GSize(800, 600));
    pWindow->setBorder(GRect(10,10,10,10));

    GView * pView = (GView*)GView::createObject();
    pView->setSize(GSize(50,50));
    pView->setPos(GPoint(100,100));
    pWindow->addChild(pView);

    GView * pView2 = (GView*)GButton::createObject();
    pView2->setId("test");
    pView2->setPos(GPoint(400, 100));
    pView2->setSize(GSize(100, 100));
    pWindow->addChild(pView2);
    pWindow->setVisible(true);

    MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}
