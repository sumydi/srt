#include "Sphere.h"
#include "Scene/Scene.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Sphere::Sphere( const Vec3 & position, const float radius )
	: m_radius{ radius }
	{
		m_position = position;
	
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Sphere::TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const
	{
		RaySphereHit( ray, m_position, m_radius, context.tMin, context.tMax, result.hitResult );
	}
}

