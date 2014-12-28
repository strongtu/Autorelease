#ifndef __GOBJECT__H__
#define __GOBJECT__H__

#include "classinfo.h"

// 对象基类
class GObject
{
	DECLARE_GDYNAMIC_CLASS(GObject)
protected:
	GObject();
	virtual ~GObject();

public:
    GObject* retain(void);
    GObject* autorelease(void);
    int release(void);

private:
    int m_ref;
};

#endif