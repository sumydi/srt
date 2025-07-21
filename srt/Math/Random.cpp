#include "Random.h"
#include <random>
#include <chrono>

namespace srt
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	StandardRandom::StandardRandom( uint32_t seed )
	{
		std::srand( seed );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	FastRandom::FastRandom()
	{
		// use standard random value as seed
		m_value = static_cast< uint32_t >( std::rand() );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	uint32_t FastRandom::Rand( )
	{
		m_value = (m_value ^ 61) ^ (m_value >> 16);
		m_value *= 9;
		m_value = m_value ^ (m_value >> 4);
		m_value *= 0x27d4eb2d;
		m_value = m_value ^ (m_value >> 15);
		return m_value;
	
	//	m_value = 1664525 * m_value + 1013904223;
	//	return m_value;
	}

	// ----------------------------------------------------------------------------
	// returns a random number in the range [0;1]
	// ----------------------------------------------------------------------------
	float RandomFloat( RandomGenerator & generator )
	{
		return (float)generator.Rand() / ( (float)RandomGenerator::kMaxValue );
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
		//return Normalize( RandomInUnitSphere( generator ) );
		float z = RandomFloat( generator, 0.0f, 1.0f ) * 2.0f - 1.0f;
		float a = RandomFloat( generator, 0.0f, 1.0f ) * 3.14159265359f * 2.0f;
		float r = sqrt(1.0f - z * z);
		float x = r * cos(a);
		float y = r * sin(a);
		return Vec3( x, y, z );
	}
}
