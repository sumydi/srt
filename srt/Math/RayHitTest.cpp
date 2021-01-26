#include "RayHitTest.h"
#include "Ray.h"
#include "Sphere.h"

namespace srt
{

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RaySphereHit( const Ray & ray, const Sphere & sphere, RayHitResult & result )
	{
		const Vec3 oc = ray.Origin() - sphere.Center();
		const float a = Dot( ray.Direction(), ray.Direction() );
		const float b = 2.0f * Dot( oc, ray.Direction() );
		const float c = Dot( oc, oc ) - sphere.Radius() * sphere.Radius();
		const float discriminant = b * b - 4.0f * a * c;

		if( discriminant < 0.0f )
		{
			result.hitTime = -1.0f;
		}
		else
		{
			result.hitTime = ( -b - sqrtf( discriminant ) ) / ( 2.0f * a );
			result.normal = Normalize( ( ray.Origin() + result.hitTime * ray.Direction() ) - sphere.Center() );
		}
	}
}
