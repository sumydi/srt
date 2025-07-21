#ifndef SRT_COLOR_H
#define SRT_COLOR_H

#include "Math/Vector3.h"
#include "Math/Vector4.h"

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
	inline uint32_t MakeRGBA( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
	{
		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline uint32_t MakeRGB( const Vec3 & v )
	{
		const uint8_t r = (uint8_t)( std::min( v.X() * 255.0f, 255.0f ) );
		const uint8_t g = (uint8_t)( std::min( v.Y() * 255.0f, 255.0f ) );
		const uint8_t b = (uint8_t)( std::min( v.Z() * 255.0f, 255.0f ) );

		return MakeRGB( r, g, b );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline Vec3 FromRGB( const uint32_t c )
	{
		const float r = (float)( ( c >> 16 ) & 0xFF ) / 255.0f;
		const float g = (float)( ( c >> 8 ) & 0xFF ) / 255.0f;
		const float b = (float)( c & 0xFF ) / 255.0f;

		return Vec3( r, g, b );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline Vec4 FromRGBA( const uint32_t c )
	{
		const float a = (float)( ( c >> 24 ) & 0xFF ) / 255.0f;
		const float r = (float)( ( c >> 16 ) & 0xFF ) / 255.0f;
		const float g = (float)( ( c >> 8 ) & 0xFF ) / 255.0f;
		const float b = (float)( c & 0xFF ) / 255.0f;
		
		return Vec4( r, g, b, a );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline uint32_t MakeRGBA( const Vec4 & v )
	{
		const uint8_t r = (uint8_t)( v.X() * 255.0f );
		const uint8_t g = (uint8_t)( v.Y() * 255.0f );
		const uint8_t b = (uint8_t)( v.Z() * 255.0f );
		const uint8_t a = (uint8_t)( v.W() * 255.0f );

		return MakeRGBA( r, g, b, a );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	inline Vec3 LinearTosRGB( const Vec3 & v )
	{
		// note: sqrt( x ) = pow( x, 1 / 2 )
		Vec3 tmp { sqrtf( v.X() ), sqrtf( v.Y() ), sqrtf( v.Z() ) };
		return tmp;
	}

}

#endif
