#ifndef SRT_BASE_H
#define SRT_BASE_H

#include <stdint.h>

#if defined( _WIN32 )
	#define SRT_PLATFORM_WINDOWS
#endif


#if defined( SRT_PLATFORM_WINDOWS )
	#define NOMINMAX
	#include <Windows.h>
#endif

namespace srt
{
	#if defined( SRT_PLATFORM_WINDOWS )
		using AppHandle = HINSTANCE;
		using WindowHandle = HWND;
	#endif

}


#endif
