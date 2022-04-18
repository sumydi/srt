#include "Random.h"
#include <random>

namespace srt
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	StandardRandom::StandardRandom( int32_t seed )
	{
		std::srand( (unsigned int )seed );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	int32_t FastRandom::Rand( )
	{
		m_value = 1664525 * m_value + 1013904223;
		return m_value;
	}

	// ----------------------------------------------------------------------------
	// returns a random number in the range [0;1)
	// ----------------------------------------------------------------------------
	float RandomFloat( RandomGenerator & generator )
	{
	
		return (float)generator.Rand() / ( (float)RandomGenerator::kMaxValue + 1.0f );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( RandomGenerator & generator, float min, float max )
	{
		return min + ( max - min ) * RandomFloat( generator );
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Vec3 RandomInUnitSphere( RandomGenerator & generator )
	{
		Vec3 v{ 10.0f, 10.0f, 10.0f };

		while( SquaredLength( v ) >= 1.0f )
		{
			const float x = RandomFloat( generator, -1.0f, 1.0f );
			const float y = RandomFloat( generator, -1.0f, 1.0f );
			const float z = RandomFloat( generator, -1.0f, 1.0f );

			v = Vec3{ x,y,z };
		}

		return v;
		/*
		const float x = RandomFloat( generator, -1.0f, 1.0f );
		const float y = RandomFloat( generator, -1.0f, 1.0f );
		const float z = RandomFloat( generator, -1.0f, 1.0f );

		Vec3 v{ x, y, z };
		v = Normalize( v );
		const float t = std::cbrt( RandomFloat( generator ) );	// use cube root for more uniform distribution
		v *= t;

		return v;
		*/
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Vec3 RandomInHemiSphere( RandomGenerator & generator, const Vec3 & normal )
	{
		Vec3 inUnitSphere = RandomInUnitSphere( generator );
		if( Dot( inUnitSphere, normal ) > 0.0f )
		{
			return inUnitSphere;
		}

		return -inUnitSphere;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Vec3 RandomUnitVector( RandomGenerator& generator )
	{
		return Normalize( RandomInUnitSphere( generator ) );
	}
}
