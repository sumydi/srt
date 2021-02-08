#ifndef SRT_LIGTHGINTUTILS_H
#define SRT_LIGTHGINTUTILS_H

#include "Math/Vector3.h"

namespace srt
{

	struct SceneTraceResult;
	class Light;

	// ============================================================================
	//	Informations about a light source. It can be a light but also informations
	//	about indirect lighting
	// ============================================================================
	struct LightSource
	{
		Vec3	direction;			// Light direction
		Vec3	radiance;
	};

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	void InitLightSource( const SceneTraceResult & result, const Light & light, LightSource & lightSource );

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	Vec3 ComputeBRDF( const SceneTraceResult & result, const LightSource & lightSource );

}

#endif


