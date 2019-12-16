#ifndef SRT_GRAPHIC_TYPES_H
#define SRT_GRAPHIC_TYPES_H

#include <stdint.h>

namespace srt
{
	//
	enum class PixelFormat : uint32_t
	{
		kX8R8G8B8_UInt = 0,
		kA8R8G8B8_UInt,

		kCount
	};
}

#endif
