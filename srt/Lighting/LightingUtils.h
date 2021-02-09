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
		Vec3	direction;			// Light direction
		Vec3	radiance;
	};

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	Vec3 ComputeBRDF( const SceneTraceResult & result, const LightSource & lightSource );

}

#endif


