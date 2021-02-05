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
	inline Vec3 FresnelSchlick( float cosTheta, const Vec3 & F0 )
	{
		return F0 + ( 1.0f - F0 ) *	 powf( 1.0f - cosTheta, 5.0f );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	inline float DistributionGGX( const float NdH, const float roughness )
	{
		const float a = roughness * roughness;
		const float a2 = a * a;
		const float d = ( NdH * a2 - NdH ) * NdH + 1.0f;
		return a2 / ( kPI * d * d );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	float GeometrySchlickGGX( float NdV, float roughness )
	{
		const float r = ( roughness + 1.0f );
		const float k = ( r * r ) / 8.0f;

		return NdV / ( NdV * ( 1.0f - k ) + k );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	float GeometrySmith( float NdV, float NdL, float roughness )
	{
		float ggx2  = GeometrySchlickGGX( NdV, roughness );
		float ggx1  = GeometrySchlickGGX( NdL, roughness );

		return ggx1 * ggx2;
	}

	// ------------------------------------------------------------------------
	//
	// Refs:
	//	- https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
	//	- https://gist.github.com/galek/53557375251e1a942dfa
	//	- https://learnopengl.com/PBR/Lighting
	//
	//
	// ------------------------------------------------------------------------
	Vec3 Light::ComputeLighting( const Vec3 & pos, const Vec3 & normal, const Material & mat ) const
	{
		const Vec3 L	= Normalize( m_position - pos );
		const Vec3 V	= Normalize( -pos );				// because our camera is always on the world's origin currently
		const Vec3 H	= Normalize( L + V );
		const float NdL = std::max( 0.0f, Dot( normal, L ) );
		const float HdV	= std::max( 0.01f, Dot( H, V ) );
		const float NdH	= std::max( 0.0f, Dot( normal, H ) );
		const float NdV	= std::max( 0.01f, Dot( normal, V ) );

		// Fresnel schlick approx
		const Vec3 F0	= Lerp( Vec3( 0.04f ), mat.GetAlbedo(), mat.GetMetalness() );	// can be computed constant across all lights (and even material if no textures)
		const Vec3 F	= FresnelSchlick( HdV, F0 );

		// Specular microfacet BRDF
		const float NDF	= DistributionGGX( NdH, mat.GetRoughness() );
		const float G	= GeometrySmith( NdV, NdL, mat.GetRoughness() );

		const Vec3 specularBRDF = ( NDF * G * F ) / std::max( 0.0001f, ( 4.0f * NdV * NdL ) );
		
		// Simple Lambertian diffuse
		const Vec3 kD = ( Vec3( 1.0f ) - F ) * ( 1.0f - mat.GetMetalness( ) );
		const Vec3 diffuseBRDF = kD * mat.GetAlbedo( );


		return ( diffuseBRDF + specularBRDF ) * m_color * NdL;
	}

}