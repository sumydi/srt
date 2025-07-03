#ifndef SRT_RAY_H
#define SRT_RAY_H

#include "Vector3.h"

namespace srt
{
	class Ray
	{
	public:
		Ray() = default;

		explicit Ray( const Vec3 & origin, const Vec3 & direction )
		: m_origin( origin )
		, m_direction( direction )
		{
		
		}

		inline Ray & operator = ( const Ray & other );

		const Vec3 & Origin( ) const { return m_origin; }
		const Vec3 & Direction( ) const { return m_direction; }

	private:
		Vec3	m_origin;
		Vec3	m_direction;
	};

	inline Ray & Ray::operator = ( const Ray & other )
	{
		m_origin = other.m_origin;
		m_direction = other.m_direction;
		return *this;
	}

}
#endif
