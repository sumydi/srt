#ifndef SRT_SCENE_H
#define SRT_SCENE_H

#include <vector>
#include <memory>
#include "Math/RayHitTest.h"

namespace srt
{
	class SceneObject;
	class Light;
	class Ray;
	class Material;
	class Camera;

	// ============================================================================
	//
	//	Results of a ray trace into a scene
	//
	// ============================================================================
	struct SceneTraceResult
	{
		RayHitResult	hitResult;				// Geometric result
		Material *		material { nullptr };
	};

	// ============================================================================
	//
	//	A scene
	//
	// ============================================================================
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		void			AddObject( SceneObject * object );
		size_t			GetObjectCount( ) const { return m_objects.size(); }
		SceneObject *	GetObject( size_t idx ) const { return m_objects[ idx ].get(); }

		void			AddLight( Light * light );
		size_t			GetLightCount( ) const { return m_lights.size(); }
		Light *			GetLight( size_t idx ) const { return m_lights[ idx ].get(); }

		void			AddCamera( Camera * camera );
		size_t			GetCameraCount( ) const { return m_cameras.size(); }
		Camera *		GetCamera( size_t idx ) const { return m_cameras[ idx ].get(); }

		void			TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const;

	private:
		Scene( const Scene & other ) = delete;
		Scene & operator = ( const Scene & other ) = delete;

		using ObjectContainer = std::vector< std::shared_ptr< SceneObject > >;
		ObjectContainer		m_objects;

		using LightContainer = std::vector< std::shared_ptr< Light > >;
		LightContainer		m_lights;

		using CameraContainer = std::vector< std::shared_ptr< Camera > >;
		CameraContainer		m_cameras;
	
	};
}

 #endif
