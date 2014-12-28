#ifndef __GAUTO_RELEASE_POOL__
#define __GAUTO_RELEASE_POOL__

#include <list>

#include "gobject.h"

class GAutoreleasePool : public GObject
{
	DECLARE_GDYNAMIC_CLASS(GAutoreleasePool)
public:
    GAutoreleasePool(void);
    virtual ~GAutoreleasePool(void);

private:
    void drain(void);
    void add(GObject* obj);

    void pushTls(void);
    void popTls(void);
private:
    typedef std::list<GObject*> GObjectList;
    GObjectList         m_list;
    GAutoreleasePool*   m_prev;

public:
    static void addObject(GObject* obj);
    static void drainObject();

    // 其实这里
    // 1. 应该有个临界区啥的
    // 2. Tls系列函数可以包装个对象，方便跨平台
    static unsigned int _tlsKey;
};

#endif

