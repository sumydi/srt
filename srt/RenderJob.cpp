#include "RenderJob.h"
#include "Math/Random.h"
#include "Math/Ray.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/SceneTraceResult.h"
#include "Lighting/LightingUtils.h"

namespace srt
{

	static constexpr uint32_t kRayCount = 2;

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	static Vec3 ComputeColor( const Scene & scene, const Ray & ray, uint32_t rayIdx )
	{
		Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

		if( rayIdx > 0 )
		{
			SceneTraceResult primaryResult;
			scene.TraceRay( ray, 0.001f, FLT_MAX, primaryResult );

			if( primaryResult.hitResult.hitTime >= 0.0f )
			{
				// Hit an object: compute lightings for all lights
				for( size_t lightIdx = 0; lightIdx < scene.GetLightCount(); ++lightIdx )
				{
					Light * light = scene.GetLight( lightIdx );

					// shadow ray
					Ray shadowRay{ primaryResult.hitResult.position, Normalize( light->GetPosition() - primaryResult.hitResult.position ) };
					SceneTraceResult shadowResult;
					scene.TraceRay( shadowRay, 0.01f, FLT_MAX, shadowResult );
					if( shadowResult.hitResult.hitTime < 0.0f )
					{
						// Direct lighting
						LightSource	lightSource{ primaryResult, *light };
						resultColor += ComputeBRDF( ray.Origin(), primaryResult, lightSource );
					}
				}

				//
				if( primaryResult.material->GetMetalness() > 0.8f )
				{
					const Vec3 reflect = Reflect( ray.Direction(), primaryResult.hitResult.normal );
					const Ray reflectRay { primaryResult.hitResult.position, reflect };
					const Vec3 indirectColor = ComputeColor( scene, reflectRay, rayIdx - 1 );
					LightSource lightSource{ -reflectRay.Direction(), indirectColor };
					resultColor += ComputeBRDF( reflectRay.Origin(), primaryResult, lightSource );
				}

				// GI diffuse
				// Vec3 target = primaryResult.hitResult.position + primaryResult.hitResult.normal + RandomUnitVector( );
				// resultColor += 0.5f * ComputeColor( scene, Ray{ primaryResult.hitResult.position, Normalize( target - primaryResult.hitResult.position ) }, rayIdx + 1 );

				//resultColor = Clamp( resultColor, 0.0f, 1.0f );

			}
			else
			{
				// hit nothing: sky
				const float t = 0.5f * ( ray.Direction().Y() + 1.0f );
				resultColor = ( 1.0f - t ) * Vec3{ 1.0f, 1.0f, 1.0f } + t * Vec3{ 0.5f, 0.7f, 1.0f };
				resultColor *= 10.0f;
			}
		}

		return resultColor;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void RenderJob::Execute( )
	{
		// do not apply jitterring on the camera when kSamplecount==1 to avoid wobling picture
		const float jitteringFactor = m_context.sampleCount > 1 ? 1.0f : 0.0f;

		const float surfWidth = static_cast< float >( m_context.image->GetMipDesc( 0 ).width );
		const float surfHeight = static_cast< float >( m_context.image->GetMipDesc( 0 ).height );
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
					const float nx = ( ( (float)( x + m_context.x ) + RandomFloat() * jitteringFactor ) / surfWidth );
					const float ny = ( ( (float)( y + m_context.y ) + RandomFloat() * jitteringFactor ) / surfHeight );

					// make a ray from the origin to the current normalized pixel
					const Ray ray = m_context.camera->GenerateRay( nx, ny );

					resultColor += ComputeColor( *m_context.scene, ray, kRayCount );
				}
				resultColor /= (float)m_context.sampleCount;

				// Basic tone mapping
				resultColor = resultColor / ( resultColor + 1.0f );

				// convert from (normalized) linear to sRGB
				const uint32_t r = (uint32_t)( sqrtf( resultColor.X() ) * 255.0f );
				const uint32_t g = (uint32_t)( sqrtf( resultColor.Y() ) * 255.0f );
				const uint32_t b = (uint32_t)( sqrtf( resultColor.Z() ) * 255.0f );

				const uint32_t color = ( r << 16 ) | ( g << 8 ) | ( b );

				*line = color;
				++line;
			}
		}

		m_context.image->UnlockMipSurface( 0 );

	}
}
