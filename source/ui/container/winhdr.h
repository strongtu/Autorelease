#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT                   
#define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlstr.h>
#include <wtl/atlapp.h>
#include <wtl/atlcrack.h>
#include <wtl/atlmisc.h>