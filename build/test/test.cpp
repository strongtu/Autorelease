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
DWORD g_t3 = 0;

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

void TestSerialize(void)
{
    GAutoreleasePool* apt = (GAutoreleasePool*)GAutoreleasePool::createObject();

    GData* pdata = (GData*)GData::createObject()->autorelease();
    GData* psubData = (GData*)GData::createObject()->autorelease();

    pdata->setBool("bool", true);
    pdata->setInt("int", -123);
    pdata->setUint("uint", 1234);
    pdata->setFloat("double", 4.5123);
    pdata->setString("string", "i am a string");
    pdata->setString("stringW", L"i am a fat string");

    psubData->setBool("bool", false);
    psubData->setInt("int", -321);
    psubData->setFloat("double", 3215.4);
    psubData->setString("string", "string a am i");
    psubData->setString("stringW", L"string fat a am i");

    pdata->setData("data", psubData);

    g_t1 = GetTickCount();

    for (int i = 0; i < 1; i++)
    {
        int len = pdata->serialize(nullptr, 0);
        byte* buf = new byte[len];
        int outlen = pdata->serialize(buf, len);
        delete[] buf;
    }

    g_t2 = GetTickCount();

    int len = pdata->serialize(nullptr, 0);
    byte* buf = new byte[len];
    int outlen = pdata->serialize(buf, len);

    pdata = (GData*)GData::createObject()->autorelease();
    for (int i = 0; i < 1; i++)
    {
        pdata->unserialize(buf, outlen);
    }

    g_t3 = GetTickCount();

    bool b = pdata->getBool("bool");
    int i = pdata->getInt("int");
    float d = pdata->getFloat("double");
    const char* p = pdata->getString("string");
    const wchar_t* pw = pdata->getStringW("stringW");

    pdata = pdata->getData("data");
    bool b2 = pdata->getBool("bool");
    int i2 = pdata->getInt("int");
    float d2 = pdata->getFloat("double");
    const char* p2 = pdata->getString("string");
    const wchar_t* pw2 = pdata->getStringW("stringW");

    delete[] buf;

    apt->release();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    TestSerialize();
    return 1;

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
