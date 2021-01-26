#include "RayHitTest.h"
#include "Ray.h"
#include "Sphere.h"

namespace srt
{

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Sphere & sphere, RayHitResult & result )
	{
		const Vec3 oc = ray.GetOrigin() - sphere.GetCenter();
		const float a = Dot( ray.GetDirection(), ray.GetDirection() );
		const float b = 2.0f * Dot( oc, ray.GetDirection() );
		const float c = Dot( oc, oc ) - sphere.GetRadius() * sphere.GetRadius();
		const float discriminant = b * b - 4.0f * a * c;

		if( discriminant < 0.0f )
		{
			result.hitTime = -1.0f;
		}
		else
		{
			result.hitTime = ( -b - sqrtf( discriminant ) ) / ( 2.0f * a );
		}
	}

}
