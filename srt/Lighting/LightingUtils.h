#ifndef SRT_LIGTHGINTUTILS_H
#define SRT_LIGTHGINTUTILS_H

#include "Math/Vector3.h"

namespace srt
{

	struct SceneTraceResult;
	class Light;

	Vec3 ComputeBRDF( const SceneTraceResult & result, const Light & light );

}

#endif


