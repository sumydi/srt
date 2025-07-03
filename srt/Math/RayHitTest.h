#ifndef SRT_RAYHITTEST_H
#define SRT_RAYHITTEST_H

#include "Ray.h"

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
		Vec3	position;
		Vec3	normal;
		bool	frontFace { true };

		void SetNormal( const Ray & ray, const Vec3 & computedNormal )
		{
			frontFace = Dot( ray.Direction(), computedNormal ) < 0.0f;
			normal = frontFace ? computedNormal : -computedNormal;
		}
	};

	class Ray;

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Vec3 & sphereCenter, const float sphereRadius, float tMin, float tMax, RayHitResult & result );

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RayQuadHit( const Ray & ray, const Vec3 & quadPos, const Vec3 & quadNormal, const Vec3 & quadWidth, const Vec3 & quadHeight, float tMin, float tMax, RayHitResult & result );


}

#endif
