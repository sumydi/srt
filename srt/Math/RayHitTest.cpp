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
		const float b = 2.0f * Dot( oc, ray.Direction() );
		const float c = Dot( oc, oc ) - sphereRadius * sphereRadius;
		const float discriminant = b * b - 4.0f * a * c;

		result.hitTime = -1.0f;

		if( discriminant >= 0.0f )
		{
			const float sqrDiscriminant = sqrtf( discriminant );

			const float r1 = ( -b - sqrDiscriminant ) / ( 2.0f * a );
			if( r1 > tMin && r1 < tMax )
			{
				result.hitTime = r1;
				result.normal = Normalize( ( ray.Origin() + result.hitTime * ray.Direction() ) - sphereCenter );
			}
			else
			{
				const float r2 = ( -b + sqrDiscriminant ) / ( 2.0f * a );
				if( r2 > tMin && r2 < tMax )
				{
					result.hitTime = r2;
					result.normal = -Normalize( ( ray.Origin() + result.hitTime * ray.Direction() ) - sphereCenter );
				}
			}
			
		}
	}
}
