#include "gobject.h"
#include "gatpool.h"

GClassInfo GObject::_classInfo("GObject", NULL,  sizeof(GObject), GObject::createObject); 

GClassInfo* GObject::getClassInfo() const
{ 
	return &GObject::_classInfo; 
}

GObject* GObject::createObject()						
{
	return new GObject; 
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
        delete this;
    }
    return ref;
}
