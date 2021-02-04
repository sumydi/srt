#ifndef SRT_MATHUTILS_H
#define SRT_MATHUTILS_H

#include <algorithm>
#include <numbers>
namespace srt
{
	constexpr float kPI = 3.141592653589793f;

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	inline float Clamp( float value, float min, float max )
	{
		return std::max( std::min( value, max ), min );
	}
}

#endif
