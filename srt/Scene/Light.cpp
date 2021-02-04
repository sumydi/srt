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
	//
	// Refs:
	//	- https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
	//	- https://gist.github.com/galek/53557375251e1a942dfa
	//
	//
	// ------------------------------------------------------------------------
	Vec3 Light::ComputeLighting( const Vec3 & pos, const Vec3 & normal, const Material & mat ) const
	{
		const Vec3 L	= Normalize( m_position - pos );
		const Vec3 V	= Normalize( -pos );
		const Vec3 H	= Normalize( L + V );
		const float LdN = std::max( 0.0f, Dot( L, normal ) );
		const float HdV	= std::max( 0.0f, Dot( H, V ) );

		// Fresnel schlick approx
		const Vec3 F0 = Lerp( Vec3( 0.04f ), mat.GetAlbedo(), mat.GetMetalness() );	// can be computed in material...
		const Vec3 fresnel = Lerp( F0, Vec3( 1.0f ), powf( 1.0f - HdV, 5.0f ) );

		// Simple Lambertian diffuse
		const Vec3 kd = Lerp( Vec3( 1.0f ) - fresnel, Vec3( 0.0f ), mat.GetMetalness() );
		const Vec3 diffuseBRDF = kd * mat.GetAlbedo( );

		return diffuseBRDF * m_color * LdN;
	}

}