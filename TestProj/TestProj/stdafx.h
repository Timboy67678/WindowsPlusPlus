// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <functional>
#include <thread>

#include "resource.h"

#include "..\..\winplusplus.hpp"
#include "..\..\dialog.hpp"
#include "..\..\window.hpp"

using namespace wpp;

#pragma comment(lib, "winplusplus.lib")
#pragma comment(lib, "comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32'\
 name='Microsoft.Windows.Common-Controls'\
 version='6.0.0.0'\
 processorArchitecture='*'\
 publicKeyToken='6595b64144ccf1df'\
 language='*'\"")
