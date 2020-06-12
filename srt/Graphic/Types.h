#ifndef SRT_GRAPHIC_TYPES_H
#define SRT_GRAPHIC_TYPES_H

#include <stdint.h>

namespace srt
{
	// Pixel formats supported by srt
	enum class PixelFormat : uint32_t
	{
		kX8R8G8B8_UInt = 0,
		kA8R8G8B8_UInt,

		kCount
	};


	// A pixel surface (a rectangular grid of pixels)
	struct PixelSurface
	{
		struct Desc
		{
			uint16_t	width;
			uint16_t	height;
			uint32_t	pitch;
		};

		Desc		desc;
		void *		surface;
	};
}

#endif
