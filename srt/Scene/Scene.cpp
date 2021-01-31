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
	void Scene::TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const
	{
		SceneTraceContext	tmpCtx { context };
		SceneTraceResult	tmpResult;

		for( auto & it : m_objects )
		{
			it->TraceRay( tmpCtx, ray, tmpResult );
			if( tmpResult.hitResult.hitTime >= tmpCtx.tMin )
			{
				tmpCtx.tMax = tmpResult.hitResult.hitTime;
				result = tmpResult;
			}
		}
	}
	

}
