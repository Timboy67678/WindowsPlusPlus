#ifndef __WINPLUSPLUS_H__
#define __WINPLUSPLUS_H__

/*
 *	Big thanks to DarthTon's Xenos Injector for his pseudo win32++ impl, helped a ton (especially mapping message values to appropriate callbacks with help from his bound member thunking)
 *	Also a big thanks to the creator of WTL:: For his functions that i started implementing but half way through just ended up copying his impl
 *	
 *	TODO: Add a window timer que and abstract add/remove methods!
 */

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#include <Windows.h>
#endif //_WINDOWS_
#include <tchar.h>
#include <richedit.h>
#include <richole.h>
#include <windowsx.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

#include <string>
#include <thread>
#include <map>

namespace std 
{ 
#ifdef _UNICODE
	using tstring = wstring;

	template <typename Type> std::tstring to_tstring( Type t )
	{ return std::to_wstring( t ); }
#else
	using tstring = string;

	template <typename Type> std::tstring to_tstring( Type t )
	{ return std::to_string( t ); }
#endif
}

#include "Thunk.hpp"
#include "Interfaces.hpp"
#include "Dialog.hpp"

#endif //__WINPLUSPLUS_H__
