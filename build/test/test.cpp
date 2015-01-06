// test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test.h"

#include "gobject.h"
#include "gatpool.h"
#include "gdata.h"
#include "gwindow.h"

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

    pWindow->SetPos(GPoint(10, 10));
    pWindow->SetSize(GSize(800, 600));
    pWindow->SetBorder(GRect(10,10,10,10));

    GView * pView = (GView*)GView::createObject();
    pView->SetSize(GSize(50,50));
    pView->SetPos(GPoint(100,100));
    pWindow->addChild(pView);

    GView * pView2 = (GView*)GView::createObject();
    pView2->SetPos(GPoint(400, 100));
    pView2->SetSize(GSize(100, 100));
    pWindow->addChild(pView2);
    pWindow->ShowWindow();

    MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}
