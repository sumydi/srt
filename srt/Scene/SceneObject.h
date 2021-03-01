#ifndef SRT_SCENEOBJECT_H
#define SRT_SCENEOBJECT_H

#include "Math/Vector3.h"
#include <string>

namespace srt
{
	class Ray;
	struct SceneTraceResult;

	class SceneObject
	{
	public:
		SceneObject( const char * name ) : m_name{ name } { };
		virtual ~SceneObject() = default;

		virtual void	TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const = 0;

		const char *	GetName( ) const { return m_name.c_str(); }

		const Vec3 &	GetPosition( ) { return m_position; }
		void			SetPosition( const Vec3 & pos ) { m_position = pos; }


	protected:
		std::string	m_name;
		Vec3		m_position;	// TODO: replace with a matrix

	private:

	};
}

#endif
