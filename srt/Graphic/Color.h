#ifndef SRT_COLOR_H
#define SRT_COLOR_H

#include "Math/Vector3.h"

namespace srt
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline uint32_t MakeRGB( uint8_t r, uint8_t g, uint8_t b )
	{
		return ( r << 16 ) | ( g << 8 ) | ( b );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline uint32_t MakeRGB( const Vec3 & v )
	{
		const uint8_t r = (uint8_t)( v.X() * 255.0f );
		const uint8_t g = (uint8_t)( v.Y() * 255.0f );
		const uint8_t b = (uint8_t)( v.Z() * 255.0f );

		return MakeRGB( r, g, b );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline Vec3 LinearTosRGB( const Vec3 & v )
	{
		Vec3 tmp { sqrtf( v.X() ), sqrtf( v.Y() ), sqrtf( v.Z() ) };
		return tmp;

	}

}

#endif
