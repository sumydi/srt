#include "Scene.h"
#include "Math/Ray.h"
#include "Scene/SceneObject.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Scene::AddObject( SceneObject * object )
	{
		m_objects.push_back( std::shared_ptr< SceneObject >( object ) );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Scene::TraceRay( const Ray & ray, SceneTraceResult & result ) const
	{
		SceneTraceContext context;
		context.tMin = 0.001f;
		context.tMax = FLT_MAX;

		SceneTraceResult	tmpResult;
		for( auto & it : m_objects )
		{
			it->TraceRay( context, ray, tmpResult );
			if( tmpResult.hitResult.hitTime >= context.tMin )
			{
				context.tMax = tmpResult.hitResult.hitTime;
				result = tmpResult;
			}
		}
	}
	

}
