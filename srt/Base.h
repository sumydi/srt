#ifndef SRT_BASE_H
#define SRT_BASE_H

#include <cstdint>
#include "resource.h"


#if defined( _WIN32 )
#define SRT_PLATFORM_WINDOWS
#endif


#if defined( SRT_PLATFORM_WINDOWS )
#include <Windows.h>
#endif

#endif