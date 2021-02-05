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
	inline float D_GGX( const float roughness, const float NdH )
	{
		const float m = roughness * roughness;
		const float m2 = m * m;
		const float d = ( NdH * m2 - NdH ) * NdH + 1.0f;
		return m2 / ( kPI * d * d );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	inline Vec3 BlinnSpecular( const float NdH, const Vec3 & specColor, const float roughness )
	{
		const float k = 1.999f / ( roughness * roughness );

		return std::min( 1.0f, 3.0f * 0.0398f * k ) * powf( NdH, std::min( 10000.0f, k ) ) * specColor;
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
		const float NdL = std::max( 0.001f, Dot( normal, L ) );
		const float HdV	= std::max( 0.001f, Dot( H, V ) );
		const float NdH	= std::max( 0.001f, Dot( normal, H ) );

		// Fresnel schlick approx
		const Vec3 F0 = Lerp( Vec3( 0.04f ), mat.GetAlbedo(), mat.GetMetalness() );	// can be computed in material if constant across the material
		const Vec3 fresnel = Lerp( F0, Vec3( 1.0f ), powf( 1.01f - HdV, 5.0f ) );

		// Blinn specular microfacet BRDF
		const Vec3 specularBRDF = BlinnSpecular( NdH, fresnel, mat.GetRoughness() );

		// Simple Lambertian diffuse
		//const Vec3 kd = Lerp( Vec3( 1.0f ) - fresnel, Vec3( 0.0f ), mat.GetMetalness() );
		//const Vec3 diffuseBRDF = kd * mat.GetAlbedo( );
		const Vec3 diffuseBRDF = ( 1.0f - fresnel ) * mat.GetAlbedo( );


		return ( diffuseBRDF + specularBRDF ) * m_color * NdL;
	}

}