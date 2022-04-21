#ifndef SRT_LIGTHGINTUTILS_H
#define SRT_LIGTHGINTUTILS_H

#include "Math/Vector3.h"

namespace srt
{

	struct SceneTraceResult;
	class Light;

	// ============================================================================
	//	Informations about a light source. It can be a light but also informations
	//	about indirect lighting.
	//	It holds all constant variable usefull to compute lighting.
	//	You can see it as a local context of lighting.
	// ============================================================================
	struct LightSource
	{
		LightSource( const SceneTraceResult & result, const Light & light );
		LightSource( const Vec3 & lightDir, const Vec3 & radiance );
		Vec3	direction;			// Light direction
		Vec3	radiance;
	};

	// ----------------------------------------------------------------------------
	// F0 is the fresnel reflectance at 0 degrees.
	//
	//	metalRef	: metal reflectance color.
	//	metalness	: 0 = full dieletric, 1 = full metal
	// ----------------------------------------------------------------------------
	inline Vec3 ComputeF0( const Vec3 & metalRef, float metalness )
	{
		return Lerp( Vec3( 0.04f ), metalRef, metalness );
	}

	// ------------------------------------------------------------------------
	// Computes fresnel term using Schlick's approximation.
	//
	//	cosTheta	: dot product between light & view vetors
	//	F0			: reflective value when directly looking at the surface
	// ------------------------------------------------------------------------
	template< typename T >
	T FresnelSchlick( float cosTheta, const T & f0 )
	{
		return f0 + ( 1.0f - f0 ) * powf( 1.0f - cosTheta, 5.0f );
	}

	// ------------------------------------------------------------------------
	// Computes a refracted vector.
	// 
	//  v			: incident direction.
	//	n			: normal.
	//	refRatio	: refraction ratio. It's the ratio of the ior of incoming matter on the ior of outcoming matter. 
	// ------------------------------------------------------------------------
	inline Vec3 Refract( const Vec3 v, const Vec3& n, const float refRatio )
	{
		const float cosTheta = std::min( Dot( -v, n ), 1.0f );
		const Vec3 outPerp =  refRatio * ( v + cosTheta * n );
		const Vec3 outParallel = -sqrtf( fabsf( 1.0f - SquaredLength( outPerp ) ) ) * n;
		return outPerp + outParallel;
	}


	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	Vec3 ComputeBRDF( const Vec3 & viewPosition, const SceneTraceResult & result, const LightSource & lightSource );

}

#endif


