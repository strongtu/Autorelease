#ifndef __GOBJECT__H__
#define __GOBJECT__H__

#include "gclassinfo.h"

// 对象基类
class GAPI GObject
{
	DECLARE_GDYNAMIC_CLASS(GObject)
protected:
	GObject();
	virtual ~GObject();

public:
    GObject* retain(void);
    GObject* autorelease(void);
    int release(void);

    static void*  allocBuffer(int size);
    static void   freeBuffer(void* p);
private:
    int m_ref;
};

#endif