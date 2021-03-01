#include "Scene.h"
#include "Math/Ray.h"
#include "SceneObject.h"
#include "Light.h"
#include "Camera.h"

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
	void Scene::AddLight( Light * light )
	{
		m_lights.push_back( std::shared_ptr< Light >( light ) );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Scene::AddCamera( Camera * camera )
	{
		m_cameras.push_back( std::shared_ptr< Camera >( camera ) );
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
				result.object = it.get();
			}
		}
	}
}
