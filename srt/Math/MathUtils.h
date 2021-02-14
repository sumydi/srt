#ifndef SRT_MATHUTILS_H
#define SRT_MATHUTILS_H

#include <algorithm>

namespace srt
{
	constexpr float kPI = 3.141592653589793f;

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	inline float Clamp( const float value, const float min, const float max )
	{
		return std::max( std::min( value, max ), min );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	inline float DegToRad( const float v )
	{
		return ( v / 180.0f ) * kPI;
	}
}

#endif
