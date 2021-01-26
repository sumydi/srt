#ifndef SRT_RAYHITTEST_H
#define SRT_RAYHITTEST_H

//#include "Ray.h"

namespace srt
{
	// ============================================================================
	//
	//	Result of a hit function.
	//
	// ============================================================================
	struct RayHitResult
	{
		float	hitTime;
		
	};

	class Ray;
	class Sphere;

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Sphere & sphere, RayHitResult & result );

}

#endif
