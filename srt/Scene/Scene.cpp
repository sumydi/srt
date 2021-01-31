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
	void Scene::TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const
	{
		SceneTraceResult	tmpResult;

		for( auto & it : m_objects )
		{
			it->TraceRay( ray, tMin, tMax, tmpResult );
			if( tmpResult.hitResult.hitTime >= tMin )
			{
				tMax = tmpResult.hitResult.hitTime;
				result = tmpResult;
			}
		}
	}
	

}
