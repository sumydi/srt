#ifndef SRT_SCENETRACERESULT_H
#define SRT_SCENETRACERESULT_H

#include "Math/RayHitTest.h"

namespace srt
{
	class SceneObject;
	class Material;

	// ============================================================================
	//
	//	Results of a ray trace into a scene
	//
	// ============================================================================
	struct SceneTraceResult
	{
		RayHitResult		hitResult;				// Geometric result
		const SceneObject *	object { nullptr };
		const Material *	material { nullptr };
	};

}

#endif
