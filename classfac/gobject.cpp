#include "stdafx.h"
#include "gobject.h"

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

int GObject::retain(void)
{
    return ++m_ref;
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
