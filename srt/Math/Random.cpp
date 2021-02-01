#include "Random.h"
#include <random>

namespace srt
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( )
	{
		static std::uniform_real_distribution< float >	distribution( 0.0f, 1.0f );
		static std::mt19937 generator;

		return distribution( generator );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( float min, float max )
	{
		return min + ( max - min ) * RandomFloat( );
	}
}
