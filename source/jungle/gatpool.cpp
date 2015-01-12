#include "stdafx.h"
#include "gatpool.h"

IMPLEMENT_GDYNAMIC_CLASS(GAutoreleasePool, GObject)

unsigned int GAutoreleasePool::_tlsKey = 0;

GAutoreleasePool::GAutoreleasePool(void)
: m_prev(NULL)
{
    pushTls();
}

GAutoreleasePool::~GAutoreleasePool(void)
{
    popTls();
    drain();
}

void GAutoreleasePool::drain(void)
{
    for(GObjectList::iterator iter = m_list.begin(); iter != m_list.end(); ++iter)
    {
        (*iter)->release();
    }
}

void GAutoreleasePool::add(GObject* obj)
{
    if (!obj) return;
    m_list.push_front(obj);
}

void GAutoreleasePool::pushTls(void)
{
    if (_tlsKey == 0)
    {
        _tlsKey = TlsAlloc();
        TlsSetValue(_tlsKey, this);
    }
    else
    {
        GAutoreleasePool* prev = static_cast<GAutoreleasePool*>(TlsGetValue(_tlsKey));
        m_prev = prev;
        TlsSetValue(_tlsKey, this);
    }
}

void GAutoreleasePool::popTls(void)
{
    if (_tlsKey == 0)
    {
        // not possible !
    }
    else
    {
        TlsSetValue(_tlsKey, m_prev);
        m_prev = NULL;
    }
}

void GAutoreleasePool::addObject(GObject* obj)
{
    if (_tlsKey == 0)
    {
        // not possible !
    }
    else
    {
        GAutoreleasePool* pool = static_cast<GAutoreleasePool*>(TlsGetValue(_tlsKey));
        pool->add(obj);
    }
}

void GAutoreleasePool::drainObject()
{
    if (_tlsKey == 0)
    {
        // not possible !
    }
    else
    {
        GAutoreleasePool* pool = static_cast<GAutoreleasePool*>(TlsGetValue(_tlsKey));
        pool->drain();
    }
}