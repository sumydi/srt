#include "Light.h"
#include "Math/MathUtils.h"
#include "Graphic/Material.h"

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
	Vec3 Light::ComputeLighting( const Vec3 & pos, const Vec3 & normal, const Material & mat ) const
	{
		Vec3 dir = ( m_position - pos );
		const float dist = Length( dir );	// used later for attenuation
		const float att = Clamp( 10.0f / dist , 0.0f, 1.0f );
		dir /= dist;

		const float dot = Clamp( Dot( dir, normal ), 0.0f, 1.0f );
		return m_color * mat.GetDiffuse() * dot * att;
	}

}