#include "LightingUtils.h"
#include "Scene/Scene.h"
#include "Scene/Light.h"
#include "Graphic/Material.h"

namespace srt
{
	// ========================================================================
	//
	// LightSource immplementation
	//
	// ========================================================================

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	LightSource::LightSource( const SceneTraceResult & result, const Light & light )
	{
		if( light.GetType()==Light::Type::kOmni )
		{
			const float lightDist		= Length( result.hitResult.position - light.GetPosition() );
			this->radiance				= light.GetColor() * ( 1.0f / ( lightDist * lightDist ) );
			this->direction				= Normalize( result.hitResult.position - light.GetPosition() );
		}
		else if( light.GetType()==Light::Type::kDirectionnal )
		{
			this->radiance	= light.GetColor();
			this->direction	= light.GetDirection();
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	LightSource::LightSource( const Vec3 & lightDir, const Vec3 & radiance )
	: direction( lightDir )
	, radiance( radiance )
	{
	
	}

	// ========================================================================
	//
	// Lighting equations... hard math goes here
	//
	// ========================================================================

	// ------------------------------------------------------------------------
	// Computes fresnel term.
	//
	//	cosTheta	: dot product between light & view vetors
	//	F0			: reflective value when directly looking at the surface
	// ------------------------------------------------------------------------
	inline Vec3 FresnelSchlick( float cosTheta, const Vec3 & f0 )
	{
		return f0 + ( 1.0f - f0 ) * powf( 1.0f - cosTheta, 5.0f );
	}

	// ------------------------------------------------------------------------
	//
	//	GGX Normal Distrtibution Function.
	//
	//	NdH			: dot product between normal and reflective (half) vector 
	//	roughness	: roughness...
	// 
	//
	// ------------------------------------------------------------------------
	inline float DistributionGGX( const float NdH, const float roughness )
	{
		const float a = roughness * roughness;
		const float a2 = a * a;
		const float d = ( NdH * a2 - NdH ) * NdH + 1.0f;
		return a2 / ( kPI * d * d );
	}

	// ------------------------------------------------------------------------
	//	Slick/GGX geometry shadowing.
	//
	//	dot	: can be dot normal & light (geom obstruction) or normal & view ( geom shadowing)
	//	k	: roughness remaping (depending of direct light or IBKL)
	//
	// ------------------------------------------------------------------------
	inline float GeometrySchlickGGX( float dot, float k )
	{
		return dot / ( dot * ( 1.0f - k ) + k );
	}

	// ------------------------------------------------------------------------
	//	
	// ------------------------------------------------------------------------
	inline float GeometrySmith( float NdV, float NdL, float roughness )
	{
		const float r  = ( roughness + 1.0f );
		const float k = ( r * r ) / 8.0f;
		const float ggx1  = GeometrySchlickGGX( NdL, k );
		const float ggx2  = GeometrySchlickGGX( NdV, k );

		return ggx1 * ggx2;
	}

	// ------------------------------------------------------------------------
	//	
	// ------------------------------------------------------------------------
	inline float GeometrySchlick( float NdV, float NdL, float roughness )
	{
		const float k  = roughness * 0.5f;
		const float ggx1  = NdV  / ( NdV * ( 1.0f - k ) + k);
		const float ggx2  = NdL  / ( NdL * ( 1.0f - k ) + k);

		return ggx1 * ggx2;
	}

	// ------------------------------------------------------------------------
	//	
	// Refs:
	//	- https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl
	//	- https://gist.github.com/galek/53557375251e1a942dfa
	//	- https://learnopengl.com/PBR/Lighting
	//
	// ------------------------------------------------------------------------
	Vec3 ComputeBRDF( const Vec3 & viewPosition, const SceneTraceResult & result, const LightSource & lightSource )
	{

		// some usefull constants used later
		const Vec3 L	= -lightSource.direction;
		const Vec3 V	= Normalize( viewPosition - result.hitResult.position );	// [constant across all lights]
		const Vec3 H	= Normalize( L + V );
		const float NdL = std::max( 0.0f, Dot( result.hitResult.normal, L ) );
		const float HdV	= std::max( 0.01f, Dot( H, V ) );
		const float NdH	= std::max( 0.0f, Dot( result.hitResult.normal, H ) );
		const float NdV	= std::max( 0.01f, Dot( result.hitResult.normal, V ) );				// [constant across all lights]

		// Fresnel schlick approx
		const Vec3 f0	= Lerp( Vec3( 0.04f ), result.material->GetAlbedo(), result.material->GetMetalness() );	// can be computed constant across all lights (and even material if no textures)
		const Vec3 f	= FresnelSchlick( HdV, f0 );

		// Specular microfacet BRDF
		const float d	= DistributionGGX( NdH, result.material->GetRoughness() );
		//const float g	= GeometrySmith( NdV, NdL, result.material->GetRoughness() );
		const float g	= GeometrySchlick( NdV, NdL, result.material->GetRoughness() );

		const Vec3 specularBRDF = ( d * g * f ) / std::max( 0.0001f, ( 4.0f * NdV * NdL ) );

		// Simple Lambertian diffuse
		const Vec3 kD = ( Vec3( 1.0f ) - f ) * ( 1.0f - result.material->GetMetalness( ) );
		const Vec3 diffuseBRDF = ( kD * result.material->GetAlbedo( ) ) / kPI;

		return ( diffuseBRDF + specularBRDF ) * lightSource.radiance * NdL;
	}
}
