#ifndef SRT_SPHERE_H
#define SRT_SPHERE_H

#include "SceneObject.h"

namespace srt
{

	class Sphere final : public SceneObject
	{
	public:
		Sphere( const Vec3 & position, const float radius );

		void TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const final;

	private:
		float	m_radius { 1.0f };
	};

}

#endif
