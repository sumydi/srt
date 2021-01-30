#ifndef SRT_SCENEOBJECT_H
#define SRT_SCENEOBJECT_H

#include "Math/Vector3.h"

namespace srt
{
	class Ray;
	struct SceneTraceContext;
	struct SceneTraceResult;

	class SceneObject
	{
	public:
		SceneObject() = default;
		virtual ~SceneObject() = default;

		virtual void TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const = 0;

	protected:
		Vec3		m_position;	// TODO: replace with a matrix

	private:

	};
}

#endif
