#include "Quad.h"
#include "Scene/Scene.h"
#include "Scene/ScenetraceResult.h"
#include "Graphic/Material.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Quad::Quad( const char * name, const Vec3 & position, const Vec3 & normal, const float width, const float height, const Material & material )
	: SceneObject( name )
	{
		m_material = std::make_shared< Material >( material );
		m_position = position;
		m_normal = normal;

		Vec3 yAxis = Cross( normal, Vec3{ 1.0f, 0.0f, 0.0 } );
		if( Length( yAxis ) < 0.001f )
		{
			yAxis = Cross( normal, Vec3{ 0.0f, 0.0f, -1.0f } );
		}
		Vec3 xAxis = Cross( yAxis, normal );
		m_width = xAxis * width;
		m_height = yAxis * height;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Quad::TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const
	{
		RayQuadHit( ray, m_position, m_normal, m_width, m_height, tMin, tMax, result.hitResult );
		if( result.hitResult.hitTime >= 0.0f )
		{
			result.material = m_material.get();
		}
	}
}
