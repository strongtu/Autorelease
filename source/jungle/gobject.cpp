#include "stdafx.h"
#include "gobject.h"
#include "gatpool.h"
#include "tcmalloc\tcmalloc.h"

GClassInfo GObject::_classInfo("GObject", NULL,  sizeof(GObject), GObject::createObject); 

GClassInfo* GObject::getClassInfo() const
{ 
	return &GObject::_classInfo; 
}

GObject* GObject::createObject()						
{
    void* p = allocBuffer(sizeof(GObject));
	return new (p) GObject; 
}

GObject::GObject()
: m_ref(1)
{
}

GObject::~GObject()
{
}

GObject* GObject::retain(void)
{
    ++m_ref;
    return this;
}

GObject* GObject::autorelease(void)
{
    GAutoreleasePool::addObject(this);
    return this;
}

int GObject::release(void)
{
    int ref = --m_ref;
    if (ref <= 0)
    {
        this->~GObject();
        freeBuffer(this);
    }
    return ref;
}

void* GObject::allocBuffer(int size)
{
    return tc_malloc(size);
}

void GObject::freeBuffer(void* p)
{
    tc_free(p);
}