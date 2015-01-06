#ifndef __GTYPES_H__
#define __GTYPES_H__

#include "./gtype.h"

class GSize;
class GPoint;
class GRect;

class GAPI GSize : public gSize
{
public:
    GSize();
    GSize(int x, int y);
    GSize(const gSize& size);
    bool operator ==(gSize size) const;
    bool operator !=(gSize size) const;
    void operator +=(gSize size);
    void operator -=(gSize size);
    void SetSize(int x, int y);
    GSize operator +(gSize size) const;
    GSize operator -(gSize size) const;
    GSize operator -() const;
    GPoint operator +(gPoint pt) const;
    GPoint operator -(gPoint pt) const;
    GRect operator +(const gRect* pRect) const;
    GRect operator -(const gRect* pRect) const;
};

class GAPI GPoint : public gPoint
{
public:
    GPoint();
    GPoint(int ptX, int ptY);
    GPoint(const gPoint& pt);
    void Offset(int xOffset, int yOffset);
    void Offset(gPoint pt);
    void Offset(gSize size);
    bool operator ==(gPoint pt) const;
    bool operator !=(gPoint pt) const;
    void operator +=(gSize size);
    void operator -=(gSize size);
    void operator +=(gPoint pt);
    void operator -=(gPoint pt);
    void SetPoint(int ptX, int ptY);
    GPoint operator +(gSize size) const;
    GPoint operator -(gSize size) const;
    GPoint operator -() const;
    GPoint operator +(gPoint pt) const;
    GSize operator -(gPoint pt) const;
    GRect operator +(const gRect* pRect) const;
    GRect operator -(const gRect* pRect) const;
};

class GAPI GRect : public gRect
{
public:
    GRect();
    GRect(int l, int t, int r, int b);
    GRect(const gRect& srcRect);
    GRect(const gRect* pSrcRect);
    GRect(gPoint pt, gSize size);
    GRect(gPoint topLeft, gPoint bottomRight);
    int Width() const;
    int Height() const;
    GSize Size() const;
    GPoint& TopLeft();
    GPoint& BottomRight();
    const GPoint& TopLeft() const;
    const GPoint& BottomRight() const;
    GPoint CenterPoint() const;
    operator gRect*();
    operator const gRect*() const;
    bool IsRectEmpty() const;
    bool IsRectNull() const;
    bool PtInRect(gPoint pt) const;
    void SetRect(int x1, int y1, int x2, int y2);
    void SetRect(gPoint topLeft, gPoint bottomRight);
    void SetRectEmpty();
    void CopyRect(const gRect* pSrcRect);
    bool EqualRect(const gRect* pRect) const;
    void InflateRect(int x, int y);
    void InflateRect(gSize size);
    void InflateRect(const gRect* pRect);
    void InflateRect(int l, int t, int r, int b);
    void DeflateRect(int x, int y);
    void DeflateRect(gSize size);
    void DeflateRect(const gRect* pRect);
    void DeflateRect(int l, int t, int r, int b);
    void OffsetRect(int x, int y);
    void OffsetRect(gSize size);
    void OffsetRect(gPoint pt);
    void NormalizeRect();
    void MoveToY(int y);
    void MoveToX(int x);
    void MoveToXY(int x, int y);
    void MoveToXY(gPoint pt);
    bool IntersectRect(const gRect* pRect1, const gRect* pRect2);
    bool UnionRect(const gRect* pRect1, const gRect* pRect2);
    bool SubtractRect(const gRect* pRect1, const gRect* pRect2);

    void operator =(const gRect& srcRect);
    bool operator ==(const gRect& rect) const;
    bool operator !=(const gRect& rect) const;
    void operator +=(gPoint pt);
    void operator +=(gSize size);
    void operator +=(const gRect* pRect);
    void operator -=(gPoint pt);
    void operator -=(gSize size);
    void operator -=(const gRect* pRect);
    void operator &=(const gRect& rect);
    void operator |=(const gRect& rect);
    GRect operator +(gPoint pt) const;
    GRect operator -(gPoint pt) const;
    GRect operator +(const gRect* pRect) const;
    GRect operator +(gSize size) const;
    GRect operator -(gSize size) const;
    GRect operator -(const gRect* lpRect) const;
    GRect operator &(const gRect& rect) const;
    GRect operator |(const gRect& rect) const;
};

//GSize

inline GSize::GSize() 
{
    cx = 0;
    cy = 0;
}

inline GSize::GSize(int x, int y)
{
    cx = x;
    cy = y;
}

inline GSize::GSize(const gSize& size)
{
    *(gSize*)this = size;
}

inline bool GSize::operator ==(gSize size) const
{
    return (cx == size.cx && cy == size.cy);
}

inline bool GSize::operator !=(gSize size) const
{
    return (cx != size.cx || cy != size.cy);
}

inline void GSize::operator +=(gSize size)
{
    cx += size.cx;
    cy += size.cy;
}

inline void GSize::operator -=(gSize size)
{
    cx -= size.cx;
    cy -= size.cy;
}

inline void GSize::SetSize(int x, int y)
{
    cx = x;
    cy = y;
}

inline GSize GSize::operator +(gSize size) const
{
    return GSize(cx + size.cx, cy + size.cy);
}

inline GSize GSize::operator -(gSize size) const
{
    return GSize(cx - size.cx, cy - size.cy);
}

inline GSize GSize::operator -() const
{
    return GSize(-cx, -cy);
}

inline GPoint GSize::operator +(gPoint pt) const
{
    return GPoint(cx + pt.x, cy + pt.y);
}

inline GPoint GSize::operator -(gPoint pt) const
{
    return GPoint(cx - pt.x, cy - pt.y);
}

inline GRect GSize::operator +(const gRect* pRect) const
{
    return GRect(pRect) + *this;
}

inline GRect GSize::operator -(const gRect* pRect) const
{
    return GRect(pRect) - *this;
}

//GPoint

inline GPoint::GPoint()
{
    x = 0;
    y = 0;
}

inline GPoint::GPoint(int ptX, int ptY)
{
    x = ptX;
    y = ptY;
}

inline GPoint::GPoint(const gPoint& pt)
{
    *(gPoint*)this = pt;
}

inline void GPoint::Offset(int xOffset, int yOffset)
{
    x += xOffset;
    y += yOffset;
}

inline void GPoint::Offset(gPoint pt)
{
    x += pt.x;
    y += pt.y;
}

inline void GPoint::Offset(gSize size)
{
    x += size.cx;
    y += size.cy;
}

inline bool GPoint::operator ==(gPoint pt) const
{
    return (x == pt.x && y == pt.y);
}

inline bool GPoint::operator !=(gPoint pt) const
{
    return (x != pt.x || y != pt.y);
}

inline void GPoint::operator +=(gSize size)
{
    x += size.cx;
    y += size.cy;
}

inline void GPoint::operator -=(gSize size)
{
    x -= size.cx;
    y -= size.cy;
}

inline void GPoint::operator +=(gPoint pt)
{
    x += pt.x;
    y += pt.y;
}

inline void GPoint::operator -=(gPoint pt)
{
    x -= pt.x;
    y -= pt.y;
}

inline void GPoint::SetPoint(int ptX, int ptY)
{
    x = ptX;
    y = ptY;
}

inline GPoint GPoint::operator +(gSize size) const
{
    return GPoint(x + size.cx, y + size.cy);
}

inline GPoint GPoint::operator -(gSize size) const
{
    return GPoint(x - size.cx, y - size.cy);
}

inline GPoint GPoint::operator -() const
{
    return GPoint(-x, -y);
}

inline GPoint GPoint::operator +(gPoint pt) const
{
    return GPoint(x + pt.x, y + pt.y);
}

inline GSize GPoint::operator -(gPoint pt) const
{
    return GSize(x - pt.x, y - pt.y);
}

inline GRect GPoint::operator +(const gRect* pRect) const
{
    return GRect(pRect) + *this;
}

inline GRect GPoint::operator -(const gRect* pRect) const
{
    return GRect(pRect) - *this;
}

//GRect

inline GRect::GRect()
{
    left = 0;
    top = 0;
    right = 0;
    bottom = 0;
}

inline GRect::GRect(int l, int t, int r, int b)
{
    left = l;
    top = t;
    right = r;
    bottom = b;
}

inline GRect::GRect(const gRect& srGRect)
{
    *(gRect*)this = srGRect;
}

inline GRect::GRect(const gRect* pSrGRect)
{
    *(gRect*)this = *pSrGRect;
}

inline GRect::GRect(gPoint pt, gSize size)
{
    right = (left = pt.x) + size.cx;
    bottom = (top = pt.y) + size.cy;
}

inline GRect::GRect(gPoint topLeft, gPoint bottomRight)
{
    left = topLeft.x;
    top = topLeft.y;
    right = bottomRight.x;
    bottom = bottomRight.y;
}

inline int GRect::Width() const
{
    return right - left;
}

inline int GRect::Height() const
{
    return bottom - top;
}

inline GSize GRect::Size() const
{
    return GSize(right - left, bottom - top);
}

inline GPoint& GRect::TopLeft()
{
    return *((GPoint*)this);
}

inline GPoint& GRect::BottomRight()
{
    return *((GPoint*)this + 1);
}

inline const GPoint& GRect::TopLeft() const
{
    return *((GPoint*)this);
}

inline const GPoint& GRect::BottomRight() const
{
    return *((GPoint*)this + 1);
}

inline GPoint GRect::CenterPoint() const
{
    return GPoint((left + right) / 2, (top + bottom) / 2);
}

inline GRect::operator gRect*()
{
    return this;
}

inline GRect::operator const gRect*() const
{
    return this;
}

inline bool GRect::IsRectEmpty() const
{
    return (left == right || top == bottom);
}

inline bool GRect::IsRectNull() const
{
    return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

inline bool GRect::PtInRect(gPoint pt) const
{
    return (pt.x >= left && pt.x < right && pt.y >= top && pt.y < bottom);
}


inline void GRect::SetRect(int x1, int y1, int x2, int y2)
{
    left = x1;
    top = y1;
    right = x2;
    bottom = y2;
}

inline void GRect::SetRect(gPoint topLeft, gPoint bottomRight)
{
    SetRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline void GRect::SetRectEmpty()
{
    left = 0;
    top = 0;
    right = 0;
    bottom = 0;
}

inline void GRect::CopyRect(const gRect* pSrGRect)
{
    *(gRect*)this = *pSrGRect;
}

inline bool GRect::EqualRect(const gRect* pRect) const
{
    return (left == pRect->left && top == pRect->top && right == pRect->right && bottom == pRect->bottom);
}

inline void GRect::InflateRect(int x, int y)
{
    left -= x;
    top -= y;
    right += x;
    bottom += y;
}

inline void GRect::InflateRect(gSize size)
{
    InflateRect(size.cx, size.cy);
}

inline void GRect::InflateRect(const gRect* pRect)
{
    left -= pRect->left;
    top -= pRect->top;
    right += pRect->right;
    bottom += pRect->bottom;
}

inline void GRect::InflateRect(int l, int t, int r, int b)
{
    left -= l;
    top -= t;
    right += r;
    bottom += b;
}

inline void GRect::DeflateRect(int x, int y)
{
    InflateRect(-x, -y);
}

inline void GRect::DeflateRect(gSize size)
{
    InflateRect(-size.cx, -size.cy);
}

inline void GRect::DeflateRect(const gRect* lpRect)
{
    left += lpRect->left;
    top += lpRect->top;
    right -= lpRect->right;
    bottom -= lpRect->bottom;
}

inline void GRect::DeflateRect(int l, int t, int r, int b)
{
    left += l;
    top += t;
    right -= r;
    bottom -= b;
}

inline void GRect::OffsetRect(int x, int y)
{
    left += x;
    top += y;
    right += x;
    bottom += y;
}

inline void GRect::OffsetRect(gSize size)
{
    OffsetRect(size.cx, size.cy);
}

inline void GRect::OffsetRect(gPoint point)
{
    OffsetRect(point.x, point.y);
}

inline void GRect::NormalizeRect()
{
    int nTemp;
    if (left > right)
    {
        nTemp = left;
        left = right;
        right = nTemp;
    }
    if (top > bottom)
    {
        nTemp = top;
        top = bottom;
        bottom = nTemp;
    }
}

inline void GRect::MoveToY(int y)
{
    bottom = Height() + y;
    top = y;
}

inline void GRect::MoveToX(int x)
{
    right = Width() + x;
    left = x;
}

inline void GRect::MoveToXY(int x, int y)
{
    MoveToX(x);
    MoveToY(y);
}

inline void GRect::MoveToXY(gPoint pt)
{
    MoveToX(pt.x);
    MoveToY(pt.y);
}

inline bool GRect::IntersectRect(const gRect* pRect1, const gRect* pRect2)
{
    int maxLeft = max(pRect1->left, pRect2->left);
    int maxTop = max(pRect1->top, pRect2->top);
    int minRight = min(pRect1->right, pRect2->right);
    int minBottom = min(pRect1->bottom, pRect2->bottom);
    if (maxLeft < minRight && maxTop < minBottom)
    {
        left = maxLeft;
        top = maxTop;
        right = minRight;
        bottom = minBottom;
        return true;
    }
    else
    {
        SetRectEmpty();
        return false;
    }
}

inline bool GRect::UnionRect(const gRect* pRect1, const gRect* pRect2)
{
    if (pRect1->left == pRect1->right || pRect1->top == pRect1->bottom)
    {
        if (pRect2->left == pRect2->right || pRect2->top == pRect2->bottom)
        {
            SetRectEmpty();
            return false;
        }
        else
        {
            *(gRect*)this = *pRect2;
            return true;
        }
    }
    else if (pRect2->left == pRect2->right || pRect2->top == pRect2->bottom)
    {
        if (pRect1->left == pRect1->right || pRect1->top == pRect1->bottom)
        {
            SetRectEmpty();
            return false;
        }
        else
        {
            *(gRect*)this = *pRect1;
            return true;
        }
    }
    int minLeft = min(pRect1->left, pRect2->left);
    int minTop = min(pRect1->top, pRect2->top);
    int maxRight = max(pRect1->right, pRect2->right);
    int maxBottom = max(pRect1->bottom, pRect2->bottom);
    if (minLeft < maxRight && minTop < maxBottom)
    {
        left = minLeft;
        top = minTop;
        right = maxRight;
        bottom = maxBottom;
        return true;
    }
    else
    {
        SetRectEmpty();
        return false;
    }
}


inline bool GRect::SubtractRect(const gRect* pRect1, const gRect* pRect2)
{
    if (pRect1->left == pRect1->right || pRect1->top == pRect1->bottom)
    {
        SetRectEmpty();
        return false;
    }
    // w&o one of four borders of pRect2 crosses pRect1 subtraction suc
    int nCount = 0;
    // topleft-topright
    if (pRect2->top > pRect1->top && pRect2->top < pRect1->bottom
        && pRect2->left <= pRect1->left && pRect2->right >= pRect1->right)
    {
        nCount++;
    }
    // bottomleft-bottomright
    if (pRect2->bottom > pRect1->top && pRect2->bottom < pRect1->bottom
        && pRect2->left <= pRect1->left && pRect2->right >= pRect1->right)
    {
        nCount++;
    }
    // lefttop-leftbottom
    if (pRect2->left > pRect1->left && pRect2->left < pRect1->right
        && pRect2->top <= pRect1->top && pRect2->bottom >= pRect1->bottom)
    {
        nCount++;
    }
    // righttop-rightbottom
    if (pRect2->right > pRect1->left && pRect2->right < pRect1->right
        && pRect2->top <= pRect1->top && pRect2->bottom >= pRect1->bottom)
    {
        nCount++;
    }

    if (nCount == 0)
    {
        if (pRect2->left <= pRect1->left && pRect2->right >= pRect1->right
            && pRect2->top <= pRect1->top && pRect2->bottom >= pRect1->bottom)
        {
            SetRectEmpty();
            return false;
        }
        else
        {
            *(gRect*)this = *pRect1;
            return true;
        }
    }
    else if (nCount == 1)
    {
        // topleft-topright
        if (pRect2->top > pRect1->top && pRect2->top < pRect1->bottom
            && pRect2->left <= pRect1->left && pRect2->right >= pRect1->right)
        {
            *(gRect*)this = *pRect1;
            bottom = pRect2->top;
        }
        // bottomleft-bottomright
        else if (pRect2->bottom > pRect1->top && pRect2->bottom < pRect1->bottom
            && pRect2->left <= pRect1->left && pRect2->right >= pRect1->right)
        {
            *(gRect*)this = *pRect1;
            top = pRect2->bottom;
        }
        // lefttop-leftbottom
        else if (pRect2->left > pRect1->left && pRect2->left < pRect1->right
            && pRect2->top <= pRect1->top && pRect2->bottom >= pRect1->bottom)
        {
            *(gRect*)this = *pRect1;
            right = pRect2->left;
        }
        // righttop-rightbottom
        else if (pRect2->right > pRect1->left && pRect2->right < pRect1->right
            && pRect2->top <= pRect1->top && pRect2->bottom >= pRect1->bottom)
        {
            *(gRect*)this = *pRect1;
            left = pRect2->right;
        }
        return true;
    }
    else    // nCount > 1
    {
        *(gRect*)this = *pRect1;
        return true;
    }
}


inline void GRect::operator =(const gRect& srGRect)
{
    CopyRect(&srGRect);
}

inline bool GRect::operator ==(const gRect& rect) const
{
    return EqualRect(&rect);
}

inline bool GRect::operator !=(const gRect& rect) const
{
    return !EqualRect(&rect);
}

inline void GRect::operator +=(gPoint point)
{
    OffsetRect(point.x, point.y);
}

inline void GRect::operator +=(gSize size)
{
    OffsetRect(size.cx, size.cy);
}

inline void GRect::operator +=(const gRect* pRect)
{
    InflateRect(pRect);
}

inline void GRect::operator -=(gPoint point)
{
    OffsetRect(-point.x, -point.y);
}

inline void GRect::operator -=(gSize size)
{
    OffsetRect(-size.cx, -size.cy);
}

inline void GRect::operator -=(const gRect* pRect)
{
    DeflateRect(pRect);
}

inline void GRect::operator &=(const gRect& rect)
{
    IntersectRect(this, &rect);
}

inline void GRect::operator |=(const gRect& rect)
{
    UnionRect(this, &rect);
}

inline GRect GRect::operator +(gPoint pt) const
{
    GRect rect(*this);
    rect.OffsetRect(pt.x, pt.y);
    return rect;
}

inline GRect GRect::operator -(gPoint pt) const
{
    GRect rect(*this);
    rect.OffsetRect(-pt.x, -pt.y);
    return rect;
}

inline GRect GRect::operator +(const gRect* pRect) const
{
    GRect rect(this);
    rect.InflateRect(pRect);
    return rect;
}

inline GRect GRect::operator +(gSize size) const
{
    GRect rect(*this);
    rect.OffsetRect(size.cx, size.cy);
    return rect;
}

inline GRect GRect::operator -(gSize size) const
{
    GRect rect(*this);
    rect.OffsetRect(-size.cx, -size.cy);
    return rect;
}

inline GRect GRect::operator -(const gRect* pRect) const
{
    GRect rect(this);
    rect.DeflateRect(pRect);
    return rect;
}

inline GRect GRect::operator &(const gRect& rect2) const
{
    GRect rect;
    rect.IntersectRect(this, &rect2);
    return rect;
}

inline GRect GRect::operator |(const gRect& rect2) const
{
    GRect rect;
    rect.UnionRect(this, &rect2);
    return rect;
}

#endif/*__GTYPES_H__*/