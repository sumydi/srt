#ifndef SRT_BASE_H
#define SRT_BASE_H

#include <stdint.h>

#if defined( _WIN32 )
	#define SRT_PLATFORM_WINDOWS
#elif defined( __linux__ )
	#define SRT_PLATFORM_LINUX
#endif


#if defined( SRT_PLATFORM_WINDOWS )
	#define NOMINMAX
	#include <Windows.h>
#endif

#if defined( SRT_PLATFORM_LINUX )
	#include <gtk/gtk.h>
#endif

namespace srt
{
	#if defined( SRT_PLATFORM_WINDOWS )
		using AppHandle = HINSTANCE;
		using WindowHandle = HWND;
	#endif

	#if defined( SRT_PLATFORM_LINUX )
		using AppHandle = GtkApplication *;
		using WindowHandle = GtkWidget *;
	#endif
}


#endif
