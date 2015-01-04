// classfac.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "gobject.h"
#include "autoreleasepool.h"
#include "gdata.h"

class TestClass : public GObject
{
	DECLARE_GDYNAMIC_CLASS(TestClass)	// 动态地声明一些用于RTTI的成员
public:
	void print()
	{
		printf("Class Name is: %s \n", _classInfo.getClassName());
	}
};

IMPLEMENT_GDYNAMIC_CLASS(TestClass, GObject)	// 动态地实现一些用于RTTI的成员

class TestClassB : public TestClass
{
	DECLARE_GDYNAMIC_CLASS(TestClassB)
};

IMPLEMENT_GDYNAMIC_CLASS(TestClassB, TestClass)


DWORD g_t1 = 0;
DWORD g_t2 = 0;

int main()
{
    GAutoreleasePool * atp = (GAutoreleasePool*)GAutoreleasePool::createObject();

	TestClass* t = (TestClass*)GClassInfo::createObject("TestClassB");

	t->print();
    t->autorelease();

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
    atp->release();

	return 0;
}