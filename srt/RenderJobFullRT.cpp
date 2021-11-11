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

static constexpr uint32_t kRayCount = 8;

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
static Vec3 ComputeColor( const Scene & scene, const Ray & ray, uint32_t rayIdx )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

	if( rayIdx == 0 )
	{
		return resultColor;
	}

	SceneTraceResult hit;
	scene.TraceRay( ray, 0.001f, FLT_MAX, hit );

	if( hit.hitResult.hitTime >= 0.0f )
	{
		Vec3 target = hit.hitResult.position + hit.hitResult.normal + Normalize( RandomInUnitSphere() );
		resultColor =  0.5f * ComputeColor( scene, Ray{ hit.hitResult.position, Normalize( target - hit.hitResult.position ) }, rayIdx - 1 );
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

				resultColor += ComputeColor( *m_context.scene, ray, kRayCount );
			}
			resultColor /= (float)m_context.sampleCount;

			// Basic tone mapping
			//resultColor = resultColor / ( resultColor + 1.0f );

			resultColor = LinearTosRGB( resultColor );
			const uint32_t color = MakeRGB( resultColor );

			*line = color;
			++line;
		}
	}

	m_context.image->UnlockMipSurface( 0 );
}
}
