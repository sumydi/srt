#include "Light.h"
#include "Math/MathUtils.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Light::Light( const Vec3 & pos, const Vec3 & color )
	: m_position{ pos }
	, m_color{ color }
	{
	
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Vec3 Light::ComputeLighting( const Vec3 & pos, const Vec3 & normal )
	{
		Vec3 dir = ( pos - m_position );
		const float dist = Length( dir );	// used later for attenuation

		dir /= dist;

		const float dot = Clamp( Dot( dir, normal ), 0.0f, 1.0f );
		return m_color * dot;
	}

}