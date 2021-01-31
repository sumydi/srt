#ifndef SRT_SCENE_H
#define SRT_SCENE_H

#include <vector>
#include <memory>
#include "Math/RayHitTest.h"

namespace srt
{
	class SceneObject;
	class Ray;
	class Material;

	// ============================================================================
	//
	//	Context of a ray trace (constants across a ray trace)
	//
	// ============================================================================
	struct SceneTraceContext
	{
		uint32_t	maxRayCount { 1 };
		uint32_t	curRayIdx { 0 };
		float		tMin { 0.001f };	// Current min time of ray trace
		float		tMax { FLT_MAX };	// Current max time of ray trace
	};

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

		void AddObject( SceneObject * object );
		SceneObject * GetObject( size_t idx ) { return m_objects[ idx ].get(); }

		void TraceRay( const SceneTraceContext & context, const Ray & ray, SceneTraceResult & result ) const;

	private:
		Scene( const Scene & other ) = delete;
		Scene & operator = ( const Scene & other ) = delete;

		using ObjectContainer = std::vector< std::shared_ptr< SceneObject > >;

		ObjectContainer		m_objects;
	
	};
}

 #endif
