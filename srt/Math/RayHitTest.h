#ifndef SRT_RAYHITTEST_H
#define SRT_RAYHITTEST_H

#include "Vector3.h"

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
		Vec3	normal;
		
	};

	class Ray;
	class Sphere;

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Sphere & sphere, float tMin, float tMax, RayHitResult & result );

}

#endif
