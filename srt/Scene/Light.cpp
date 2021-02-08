#include "Light.h"
#include "Math/MathUtils.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Light::Light( Type type, const Vec3 & pos, const Vec3 & dir, const Vec3 & color )
	: m_type{ type }
	, m_position{ pos }
	, m_color{ color }
	{
		m_direction = Normalize( dir );

		if( type==Type::kDirectionnal )
		{
			// a directionnal light has no real position: make it far away
			m_position = -m_direction * 9999999.9f;
		}
	}
}