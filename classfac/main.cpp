// classfac.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "gobject.h"
#include "autoreleasepool.h"

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


int main()
{
    GAutoreleasePool * atp = (GAutoreleasePool*)GAutoreleasePool::createObject();

	TestClass* t = (TestClass*)GClassInfo::createObject("TestClassB");
	t->print();
    t->autorelease();

    atp->release();

	return 0;
}