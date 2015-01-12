#ifndef __GARRAY_H__
#define __GARRAY_H__

#include "gobject.h"
#include "gdatatype.h"

class GAPI GArray : public GObject
{
    DECLARE_GDYNAMIC_CLASS(GArray)

public:
    GArray(void);
    virtual ~GArray(void);

    int          serialize(byte* outBuf, int len) const;
    const byte*  unserialize(const byte* inbuf, int len);
};

#endif