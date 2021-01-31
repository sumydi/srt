#ifndef SRT_SPHERE_H
#define SRT_SPHERE_H

#include "SceneObject.h"
#include <memory>

namespace srt
{
	class Material;

	class Sphere final : public SceneObject
	{
	public:
		Sphere( const Vec3 & position, const float radius, const Material & material );

		void TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const final;

	private:
		float						m_radius { 1.0f };
		std::shared_ptr< Material >	m_material;
	};

}

#endif
