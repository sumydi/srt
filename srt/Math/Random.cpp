#include "Random.h"
#include <random>

namespace srt
{
	static uint32_t RandomInt()
	{
		static uint32_t g_value = (uint32_t)rand();

		g_value = 1664525 * g_value + 1013904223;
		return g_value;
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( )
	{
	
		static std::uniform_real_distribution< float >	distribution( 0.0f, 1.0f );
		static std::mt19937 generator;
		//return distribution( generator );
		
		return (float)RandomInt() / (float)UINT32_MAX;
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( float min, float max )
	{
		return min + ( max - min ) * RandomFloat( );
	}
}
