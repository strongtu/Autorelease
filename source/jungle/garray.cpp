#include "stdafx.h"
#include "garray.h"
#include "gdataserialize.h"

IMPLEMENT_GDYNAMIC_CLASS(GArray, GObject);

GArray::GArray(void)
{
}

GArray::~GArray(void)
{
}

int GArray::serialize(byte* outBuf, int len) const
{
    return 0;
}

const byte* GArray::unserialize(const byte* inbuf, int len)
{
    return inbuf;
}