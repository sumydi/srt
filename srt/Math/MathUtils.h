#ifndef SRT_MATHUTILS_H
#define SRT_MATHUTILS_H

#include <algorithm>

namespace srt
{
	float Clamp( float value, float min, float max )
	{
		return std::max( std::min( value, max ), min );
	}

}

#endif