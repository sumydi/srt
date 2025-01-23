#include "RenderJobRT.h"
#include "Math/Random.h"
#include "Math/Ray.h"
#include "Math/Halton.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/SceneTraceResult.h"
#include "Lighting/LightingUtils.h"
#include "Graphic/Color.h"

namespace srt
{

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
Vec3 RenderJobRayTracing::ComputeColor( const Ray & ray, uint32_t rayIdx )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

	if( rayIdx > 0 )
	{
		SceneTraceResult traceResult;
		m_context.scene->TraceRay( ray, 0.001f, FLT_MAX, traceResult );

		if( traceResult.hitResult.hitTime >= 0.0f )
		{
			// Hit an object: compute lightings for all lights
			for( size_t lightIdx = 0; lightIdx < m_context.scene->GetLightCount(); ++lightIdx )
			{
				Light * light = m_context.scene->GetLight( lightIdx );

				// shadow ray
				Ray shadowRay{ traceResult.hitResult.position, Normalize( light->GetPosition() - traceResult.hitResult.position ) };
				SceneTraceResult shadowResult;
				m_context.scene->TraceRay( shadowRay, 0.01f, FLT_MAX, shadowResult );
				if( shadowResult.hitResult.hitTime < 0.0f )
				{
					// Direct lighting
					LightSource	lightSource{ traceResult, *light };
					resultColor += ComputeBRDF( ray.Origin(), traceResult, lightSource );
				}
			}

			if( traceResult.material->GetMetalness() > 0.8f )
			{

				const Vec3 reflect = Reflect( ray.Direction(), traceResult.hitResult.normal ) + RandomUnitVector( m_rndGenerator ) * traceResult.material->GetRoughness();
				const Ray reflectRay { traceResult.hitResult.position, reflect };
				resultColor += ComputeColor( reflectRay, rayIdx - 1 );;
			}
			else
			{
				if( traceResult.material->GetIOR( ) > 1.0f )
				{
					// Glass material
					// The refraction ratio dependent if we are going in the volume or out of the volume
					const float refRatio = traceResult.hitResult.frontFace ? ( 1.0f / traceResult.material->GetIOR() ) : traceResult.material->GetIOR();

					const Vec3 unitDir = Normalize( ray.Direction() );

					const float cosTheta = Dot( -unitDir, traceResult.hitResult.normal );
					const float sinTheta = sqrtf( 1.0f - cosTheta * cosTheta );
					const bool canNotRefract = ( refRatio * sinTheta > 1.0f );
					const float f0 = ( 1.0f - refRatio ) / ( 1.0f + refRatio );

					Vec3 scattered;

					if( canNotRefract /* || FresnelSchlick(cosTheta, f0 * f0) > RandomFloat(m_rndGenerator) */)
					{
						scattered = Reflect( unitDir, traceResult.hitResult.normal );
					}
					else
					{
						scattered = Refract( unitDir, traceResult.hitResult.normal, refRatio );
					}
					resultColor += ComputeColor( Ray{ traceResult.hitResult.position, scattered }, rayIdx - 1);
				}
			}

		}
		else
		{
			// hit nothing: sky
			const float t = 0.5f * ray.Direction().Y() + 1.0f;
			resultColor = Lerp( Vec3{ 1.0f, 1.0f, 1.0f }, Vec3{ 0.5f, 0.7f, 1.0f }, t );
		}
	}

	return resultColor;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RenderJobRayTracing::Execute( )
{
	// do not apply jitterring on the camera when samplecount==1 to avoid wobling picture
	const float jitteringFactor = m_context.sampleCount > 1 ? 1.0f : 0.0f;

	const float surfWidth = static_cast< float >( m_context.image->GetMipDesc( 0 ).width - 1 );
	const float surfHeight = static_cast< float >( m_context.image->GetMipDesc( 0 ).height - 1 );
	const uint32_t surfPitch = m_context.image->GetMipDesc( 0 ).pitch;
	uint8_t * surfPixels = reinterpret_cast< uint8_t * >( m_context.image->LockMipSurface( 0 ) );

	for( uint32_t y = 0; y < m_context.height; ++y )
	{
		uint32_t * line = reinterpret_cast< uint32_t * >( surfPixels + ( m_context.y + y ) * surfPitch ) + m_context.x;
		for( uint32_t x = 0; x < m_context.width; ++x )
		{
			Vec3 resultColor{ 0.0f, 0.0f, 0.0f };
			for( uint32_t sampleIdx = 0; sampleIdx < m_context.sampleCount; ++sampleIdx )
			{
				// transform coordinates to "normalized" coordinates
				Vec2 jitter = m_context.halton->GetValue( sampleIdx );
				const float nx = ( ( (float)( x + m_context.x ) + jitter.X() * jitteringFactor ) / surfWidth );
				const float ny = ( ( (float)( y + m_context.y ) + jitter.Y() * jitteringFactor ) / surfHeight );

				// make a ray from the origin to the current normalized pixel
				const Ray ray = m_context.camera->GenerateRay( nx, ny );

				resultColor += ComputeColor( ray, m_context.rayCount );
			}
			resultColor /= (float)m_context.sampleCount;

			// Basic tone mapping
			resultColor = resultColor / ( resultColor + 1.0f );

			// convert from (normalized) linear to sRGB
			Vec3 srgbColor = LinearTosRGB( resultColor );
			*line = MakeRGB( srgbColor );
			++line;
		}
	}

	m_context.image->UnlockMipSurface( 0 );

}
}
