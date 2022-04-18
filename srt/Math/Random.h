#ifndef SRT_RANDOM_H
#define SRT_RANDOM_H

#include "Vector3.h"

namespace srt
{

	// ============================================================================
	//
	//	Standard pseudo random number generator.
	// 
	// ============================================================================
	class StandardRandom
	{
	public:
		static constexpr int32_t kMaxValue = RAND_MAX;

		StandardRandom() = default;
		StandardRandom( int32_t seed );

		int32_t Rand( ) { return std::rand(); }

	private:
	};

	// ============================================================================
	//
	//	Fast pseudo random number generator.
	// 
	//  NOTE
	//  std::mt19937 generator is "slow", replaced by this generator
	//  allowed to have around 2x speedup
	//  BUT I'm aware that this is a bad PRNG
	// ============================================================================
	class FastRandom
	{
	public:
		static constexpr int32_t kMaxValue = 0x7fffffff;

		FastRandom( ) : m_value{ 1 } { }
		FastRandom( int32_t seed ) : m_value{ seed } { }

		int32_t	Rand( );

	private:
		int32_t		m_value;
	};

	using RandomGenerator = FastRandom;

	// ------------------------------------------------------------------------
	// Returns a random number between [0; 1(
	// ------------------------------------------------------------------------
	float	RandomFloat( RandomGenerator & generator );

	// ------------------------------------------------------------------------
	// Returns a random number between [min; max(
	// ------------------------------------------------------------------------
	float	RandomFloat( RandomGenerator & generator, float min, float max );

	// ------------------------------------------------------------------------
	// Returns a random position in an unit sphere
	// ------------------------------------------------------------------------
	Vec3	RandomInUnitSphere( RandomGenerator & generator );

	// ------------------------------------------------------------------------
	// Returns a random position in a hemi sphere
	// ------------------------------------------------------------------------
	Vec3	RandomInHemiSphere( RandomGenerator & generator, const Vec3 & normal );

	// ------------------------------------------------------------------------
	// Returns a random unit vector
	// ------------------------------------------------------------------------
	Vec3	RandomUnitVector( RandomGenerator & generator );
}

#endif

