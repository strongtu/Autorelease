#include "./windib.h"

DIB::DIB(void) : m_hDC(nullptr),
    m_hDIB(nullptr),
    m_lpByte(nullptr)
{
}

DIB::~DIB(void)
{
    if (m_hDIB)
    {
        DeleteObject(m_hDIB);
    }
    if (m_hDC)
    {
        DeleteDC(m_hDC);
    }
}

void DIB::Rebuild(int cx, int cy)
{
    CreateDevice();

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));

    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = cx;
    bmi.bmiHeader.biHeight = -cy;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biSizeImage = cx * cy * 4;

    m_lpByte = nullptr;
    m_hDIB = ::CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, (void**)&m_lpByte, nullptr, 0);

    HGDIOBJ hOldObject = ::SelectObject(m_hDC, m_hDIB);
    ::DeleteObject(hOldObject);
}

byte* DIB::GetData()
{
    return m_lpByte;
}

void DIB::CreateDevice()
{
    if (!m_hDC)
    {
        m_hDC = ::CreateCompatibleDC(nullptr);
    }
}