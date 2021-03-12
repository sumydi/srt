#ifndef SRT_GRAPHIC_TYPES_H
#define SRT_GRAPHIC_TYPES_H

#include "Base.h"

namespace srt
{
	// Pixel formats supported by srt
	enum class PixelFormat : uint32_t
	{
		kBGRX8_UInt = 0,
		kBGRA8_UInt,

		kCount
	};


	// A pixel surface (a rectangular grid of pixels)
	struct PixelSurface
	{
		struct Desc
		{
			uint16_t	width{ 0 };
			uint16_t	height{ 0 };
			uint32_t	pitch{ 0 };
		};

		Desc		desc;
		void *		surface{ nullptr };
	};
}

#endif
