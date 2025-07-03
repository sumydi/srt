#include "RenderJobPathTracing.h"
#include "Math/Ray.h"
#include "Math/Halton.h"
#include "Math/Random.h"
#include "Lighting/LightingUtils.h"
#include "Graphic/Color.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneTraceResult.h"
#include "Graphic/Color.h"
#include "Graphic/ToneMapping.h"

namespace srt
{
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
bool RenderJobPathTracing::Scatter( const Ray & ray, const SceneTraceResult & traceResult, Vec3 & scattered, Vec3 & attenuation )
{
	if( traceResult.material->GetMetalness() > 0.9f )
	{
		// Metal material
		scattered = Reflect( Normalize( ray.Direction( ) ), traceResult.hitResult.normal );
		scattered += traceResult.material->GetRoughness( ) * RandomInHemiSphere( m_rndGenerator, traceResult.hitResult.normal );
		attenuation = traceResult.material->GetAlbedo( );

		// Check if the scattered vector does not go inside the surface
		return ( Dot( scattered, traceResult.hitResult.normal ) > 0.0f );
	}
	else
	{
		if( traceResult.material->GetIOR( ) <= 1.0f )
		{
			// Lambertian material
			scattered = traceResult.hitResult.normal + RandomUnitVector( m_rndGenerator );

			// Checks for near zero length scaterred vector
			if( SquaredLength( scattered ) < 0.0001f )
			{
				scattered = traceResult.hitResult.normal;
			}
			attenuation = traceResult.material->GetAlbedo( );
		}
		else
		{
			// Glass or other dieletric material
			attenuation = Vec3{ 1.0f, 1.0f, 1.0f };

			// The refraction ratio dependent if we are going in the volume or out of the volume
			const float refRatio = traceResult.hitResult.frontFace ? ( 1.0f / traceResult.material->GetIOR() ) : traceResult.material->GetIOR();

			const Vec3 unitDir = Normalize( ray.Direction() );

			const float cosTheta = Dot( -unitDir, traceResult.hitResult.normal );
			const float sinTheta = sqrtf( 1.0f - cosTheta * cosTheta );
			const bool canNotRefract = ( refRatio * sinTheta > 1.0f );
			const float f0 = ( 1.0f - refRatio ) / ( 1.0f + refRatio );

			if( canNotRefract /* || FresnelSchlick(cosTheta, f0 * f0) > RandomFloat(m_rndGenerator) */)
			{
				scattered = Reflect( unitDir, traceResult.hitResult.normal );
			}
			else
			{
				scattered = Refract( unitDir, traceResult.hitResult.normal, refRatio );
			}
		}
	}
	return true;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
Vec3 RenderJobPathTracing::ComputeColor( const Ray & initialRay )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };
	Vec3 throughput{ 1.0f, 1.0f, 1.0f };
	Ray ray = initialRay;

	for( uint32_t rayIdx = 0; rayIdx <= m_context.rayCount; ++rayIdx )
	{
		SceneTraceResult hit;
		m_context.scene->TraceRay( ray, 0.001f, FLT_MAX, hit );

		if( hit.hitResult.hitTime >= 0.0f )
		{
			Vec3	scattered;
			Vec3	attenuation;
			if( Scatter( ray, hit, scattered, attenuation ) )
			{
				// TODO: manage emissive by adding it to resultColor
				throughput *= attenuation;

				ray = Ray{ hit.hitResult.position, scattered };
			}
		}
		else
		{
			// Hit nothing
			const float t = 0.5f * ray.Direction().Y() + 1.0f;
			resultColor += Lerp( Vec3{ 0.8f, 0.0f, 0.8f }, Vec3{ 0.5f, 0.7f, 1.0f }, t ) * throughput;
			break;
		}
	}	
	return resultColor;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RenderJobPathTracing::Execute( )
{
	// do not apply jitterring on the camera when samplecount==1 to avoid wobling picture
	const float jitteringFactor = m_context.sampleCount > 1 ? 1.0f : 0.0f;

	const float surfWidth = static_cast< float >( m_context.backBuffer->GetMipDesc( 0 ).width - 1 );
	const float surfHeight = static_cast< float >( m_context.backBuffer->GetMipDesc( 0 ).height - 1 );
	const uint32_t surfPitch = m_context.backBuffer->GetMipDesc( 0 ).pitch;
	uint8_t * surfBB = reinterpret_cast< uint8_t * >( m_context.backBuffer->LockMipSurface( 0 ) );
	uint8_t * surfPrev = reinterpret_cast< uint8_t * >( m_context.prevResult->LockMipSurface( 0 ) );
	uint8_t * surfCur = reinterpret_cast< uint8_t * >( m_context.curResult->LockMipSurface( 0 ) );

	for( uint32_t y = 0; y < m_context.height; ++y )
	{
		uint32_t * lineBB = reinterpret_cast< uint32_t * >( surfBB + ( m_context.y + y ) * surfPitch ) + m_context.x;
		uint32_t * linePrev = reinterpret_cast< uint32_t * >( surfPrev + ( m_context.y + y ) * surfPitch ) + m_context.x;
		uint32_t * lineCur = reinterpret_cast< uint32_t * >( surfCur + ( m_context.y + y ) * surfPitch ) + m_context.x;
		for( uint32_t x = 0; x < m_context.width; ++x )
		{
			Vec3 resultColor{ 0.0f, 0.0f, 0.0f };
			for( uint32_t sampleIdx = 0; sampleIdx < m_context.sampleCount; ++sampleIdx )
			{
				// transform coordinates to "normalized" coordinates
				//Vec2 jitter = m_context.halton->GetValue( sampleIdx );
				const Vec2 jitter = Vec2{ RandomFloat( m_rndGenerator ), RandomFloat( m_rndGenerator ) } - 0.5f;// = m_context.halton->GetValue( sampleIdx );
				const float nx = ( ( (float)( x + m_context.x ) + jitter.X() * jitteringFactor ) / surfWidth );
				const float ny = ( ( (float)( y + m_context.y ) + jitter.Y() * jitteringFactor ) / surfHeight );

				// make a ray from the origin to the current normalized pixel
				const Ray ray = m_context.camera->GenerateRay( nx, ny );

				resultColor += ComputeColor( ray );
			}
			resultColor /= (float)m_context.sampleCount;

			// Acucmulate results
			Vec3 prevColor = FromRGB( *linePrev );
			resultColor = Lerp( prevColor, resultColor, 1.0f / (float)( m_context.frameIndex + 1 ) );
			*lineCur = MakeRGB( resultColor );

			// Tone mapping
			resultColor = ACESFilm( resultColor );

			resultColor = LinearTosRGB( resultColor );
			const uint32_t color = MakeRGB( resultColor );

			*lineBB = color;
			++lineBB;
			++linePrev;
			++lineCur;
		}
	}

	m_context.backBuffer->UnlockMipSurface( 0 );
}
}
