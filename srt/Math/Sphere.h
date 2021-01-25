#ifndef SRT_SPHERE_H
#define SRT_SPHERE_H

#include "Vector3.h"

namespace srt
{
	class Sphere
	{
	public:
		Sphere() = default;

		explicit Sphere( const Vec3 & center, const float radius )
		: m_center{ center }
		, m_radius{ radius }
		{
		}

		inline Sphere & operator = ( const Sphere & other );

	private:
		Vec3	m_center;
		float	m_radius { 1.0f };
	
	};

	inline Sphere & Sphere::operator = ( const Sphere & other )
	{
		m_center = other.m_center;
		m_radius = other.m_radius;
	}

}

#endif
