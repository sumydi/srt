#ifndef SRT_QUAD_H
#define SRT_QUAD_H

#include "SceneObject.h"
#include "Math/Vector3.h"
#include <memory>

namespace srt
{
class Material;

class Quad final : public SceneObject
{
public:
	Quad( const char * name, const Vec3 & position, const Vec3 & normal, const float width, const float height, const Material & material );

	void TraceRay( const Ray & ray, float tMin, float tMax, SceneTraceResult & result ) const final;

private:
	Vec3		m_position;
	Vec3		m_normal;
	Vec3		m_width;
	Vec3		m_height;

	std::shared_ptr< Material >	m_material;
};

}

#endif
