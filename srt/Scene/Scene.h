#ifndef SRT_SCENE_H
#define SRT_SCENE_H

#include <vector>
#include <memory>
#include "Math/RayHitTest.h"

namespace srt
{
	class SceneObject;
	class Ray;

	// ============================================================================
	//
	//	Context of a ray trace (constants across a ray trace)
	//
	// ============================================================================
	struct SceneTraceContext
	{
		float	tMin;	// Current time min of ray trace
		float	tMax;	// Current time max of ray trace
	};

	// ============================================================================
	//
	//	Results of a ray trace into a scene
	//
	// ============================================================================
	struct SceneTraceResult
	{
		RayHitResult	hitResult;	// Geometric result
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

		void AddObject( SceneObject * object );

		void TraceRay( const Ray & ray, SceneTraceResult & result ) const;

	private:
		Scene( const Scene & other ) = delete;
		Scene & operator = ( const Scene & other ) = delete;

		using ObjectContainer = std::vector< std::shared_ptr< SceneObject > >;

		ObjectContainer		m_objects;
	
	};
}

 #endif
