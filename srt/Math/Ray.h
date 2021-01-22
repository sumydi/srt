#ifndef SRT_RAY_H
#define SRT_RAY_H

#include "Vector3.h"

namespace srt
{
	class Ray
	{
	public:
		explicit Ray( const Vec3 & origin, const Vec3 & direction )
		: m_origin( origin )
		, m_direction( direction )
		{
		
		}

		Vec3	m_origin;
		Vec3	m_direction;
	};


}
#endif
