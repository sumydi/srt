#include "Random.h"
#include <random>

namespace srt
{
	// ----------------------------------------------------------------------------
	// returns a random number in the range [0;1)
	// ----------------------------------------------------------------------------
	float RandomFloat( )
	{
		// NOTE
		//  std::mt19937 generator is "slow", replaced by std::rand()
		//  allowed to have around 2x speedup
		//  BUT I'm aware that std::rand() is a bad PRNG
		//

		//static std::uniform_real_distribution< float >	distribution( 0.0f, 1.0f );
		//static std::mt19937 generator;
		//return distribution( generator );
		
		return (float)std::rand() / ( (float)RAND_MAX + 1.0f );
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	float RandomFloat( float min, float max )
	{
		return min + ( max - min ) * RandomFloat( );
	}
}
