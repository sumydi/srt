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

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void RayQuadHit( const Ray & ray, const Vec3 & quadPos, const Vec3 & quadNormal, const Vec3 & quadWidth, const Vec3 & quadHeight, float tMin, float tMax, RayHitResult & result  )
	{
		result.hitTime	= -1.0f;

		const float nDotR = Dot( quadNormal, ray.Direction() ); 

		if( std::abs( nDotR ) < 0.00001f )
		{
			return;
		}

		const float t = Dot( quadPos - ray.Origin(), quadNormal ) / nDotR;
		if( t < tMin || t > tMax )
		{
			return;
		}

		const Vec3 hitPoint = ray.Origin() + ray.Direction() * t;
		const Vec3 hitPointOffset = hitPoint - quadPos;
		const float widthDot = Dot( hitPointOffset, quadWidth );
		const float heightDot = Dot( hitPointOffset, quadHeight );
		if( widthDot >= 0.0f && widthDot < Dot( quadWidth, quadWidth ) &&
			heightDot >= 0.0f && heightDot < Dot( quadHeight, quadHeight ) )
		{
			result.hitTime = t;
			result.position = hitPoint;
			result.SetNormal( ray, quadNormal );
		}
	}
}
