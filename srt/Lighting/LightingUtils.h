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

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	Vec3 ComputeBRDF( const Vec3 & viewPosition, const SceneTraceResult & result, const LightSource & lightSource );

}

#endif


