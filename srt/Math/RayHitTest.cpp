#include "RayHitTest.h"
#include "Ray.h"

namespace srt
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Vec3 & sphereCenter, const float sphereRadius, float tMin, float tMax, RayHitResult & result )
	{
		const Vec3 oc = ray.Origin() - sphereCenter;
		const float a = Dot( ray.Direction(), ray.Direction() );
		const float b = Dot( oc, ray.Direction() );
		const float c = Dot( oc, oc ) - sphereRadius * sphereRadius;
		const float discriminant = b * b - a * c;

		result.hitTime	= -1.0f;

		if( discriminant >= 0.0f )
		{
			const float sqrDiscriminant = sqrtf( discriminant );

			float root = ( -b - sqrDiscriminant ) / a;

			if( root < tMin || root > tMax )
			{
				root = ( -b + sqrDiscriminant ) / a;
				if( root < tMin || root > tMax )
				{
					return;
				}
			}

			result.hitTime = root;
			result.position = ray.Origin( ) + ray.Direction( ) * result.hitTime;
			const Vec3 normal = ( result.position - sphereCenter ) / sphereRadius;
			result.SetNormal( ray, normal );
		}
	}
}
