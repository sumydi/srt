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
		float	hitTime { - 1.0f };
		Vec3	normal;
		
	};

	class Ray;
	class Sphere;

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Vec3 & sphereCenter, const float sphereRadius, float tMin, float tMax, RayHitResult & result );

}

#endif
