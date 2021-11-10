#ifndef SRT_RANDOM_H
#define SRT_RANDOM_H

#include "Vector3.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// Returns a random number between [0; 1(
	// ------------------------------------------------------------------------
	float	RandomFloat( );

	// ------------------------------------------------------------------------
	// Returns a random number between [min; max(
	// ------------------------------------------------------------------------
	float	RandomFloat( float min, float max );

	// ------------------------------------------------------------------------
	// Returns a random position in an unit sphere
	// ------------------------------------------------------------------------
	Vec3 RandomInUnitSphere( );
}

#endif

