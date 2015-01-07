#ifndef __GSTACKLAYOUT_H__
#define __GSTACKLAYOUT_H__

#include "gview.h"

class GAPI GStackLayout : public GView
{
    DECLARE_GDYNAMIC_CLASS(GStackLayout)

public:
    GStackLayout();
    virtual ~GStackLayout();

    virtual void onResize(const GSize& szOld, const GSize& szNew);

protected:
    void layout(void);
};

#endif