#include "RenderJobFullRT.h"
#include "Math/Ray.h"
#include "Math/Halton.h"
#include "Math/Random.h"
#include "Graphic/Color.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneTraceResult.h"
#include "Graphic/Color.h"

namespace srt
{
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
bool RenderJobFullRT::Scatter( const Ray & ray, const SceneTraceResult & traceResult, Vec3 & scattered, Vec3 & attenuation )
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

			const float refRatio = traceResult.hitResult.frontFace ? ( 1.0f / traceResult.material->GetIOR() ) : traceResult.material->GetIOR();

			const Vec3 unitDir = Normalize( ray.Direction() );
			const Vec3 refracted = Refract(unitDir, traceResult.hitResult.normal, refRatio );

			scattered = refracted;
		}
	}
	return true;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
Vec3 RenderJobFullRT::ComputeColor( const Ray & ray, uint32_t rayIdx )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

	if( rayIdx == 0 )
	{
		return resultColor;
	}

	SceneTraceResult hit;
	m_context.scene->TraceRay( ray, 0.001f, FLT_MAX, hit );

	if( hit.hitResult.hitTime >= 0.0f )
	{
		// Uses Lambertian distribution
		Vec3	scattered;
		Vec3	attenuation;
		if( Scatter( ray, hit, scattered, attenuation ) )
		{
			resultColor = attenuation * ComputeColor( Ray{ hit.hitResult.position, scattered }, rayIdx - 1);
		}
	}
	else
	{
		// hit nothing
		const float t = 0.5f * ray.Direction().Y() + 1.0f;
		resultColor = Lerp( Vec3{ 1.0f, 1.0f, 1.0f }, Vec3{ 0.5f, 0.7f, 1.0f }, t );
	}
	
	return resultColor;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RenderJobFullRT::Execute( )
{
	// do not apply jitterring on the camera when kSamplecount==1 to avoid wobling picture
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
			//resultColor = resultColor / ( resultColor + 1.0f );

			//resultColor = LinearTosRGB( resultColor );
			const uint32_t color = MakeRGB( resultColor );

			*line = color;
			++line;
		}
	}

	m_context.image->UnlockMipSurface( 0 );
}
}
