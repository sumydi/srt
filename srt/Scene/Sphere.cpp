#include "Sphere.h"
#include "Scene/Scene.h"
#include "Graphic/Material.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Sphere::Sphere( const Vec3 & position, const float radius, const Material & material )
	: m_radius{ radius }
	{
		m_position = position;
		m_material = std::make_shared< Material >( material );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Sphere::TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const
	{
		RaySphereHit( ray, m_position, m_radius, context.tMin, context.tMax, result.hitResult );
		if( result.hitResult.hitTime >= 0.0f )
		{
			result.material = m_material.get();
		}
	}
}

