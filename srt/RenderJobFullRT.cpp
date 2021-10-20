#include "RenderJobFullRT.h"
#include "Math/Ray.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/SceneTraceResult.h"

namespace srt
{

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
static Vec3 ComputeColor( const Scene & scene, const Ray & ray )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

	SceneTraceResult primaryResult;
	scene.TraceRay( ray, 0.001f, FLT_MAX, primaryResult );

	if( primaryResult.hitResult.hitTime >= 0.0f )
	{
		resultColor = Vec3{ 1.0f, 0.0f, 0.0f };
	}
	else
	{
		// hit nothing
		resultColor = Vec3{ 0.5f, 0.5f, 0.5f };
	}

	return resultColor;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RenderJobFullRT::Execute( )
{
	const float surfWidth = static_cast< float >( m_context.image->GetMipDesc( 0 ).width );
	const float surfHeight = static_cast< float >( m_context.image->GetMipDesc( 0 ).height );
	const uint32_t surfPitch = m_context.image->GetMipDesc( 0 ).pitch;
	uint8_t * surfPixels = reinterpret_cast< uint8_t * >( m_context.image->LockMipSurface( 0 ) );

	for( uint32_t y = 0; y < m_context.height; ++y )
	{
		uint32_t * line = reinterpret_cast< uint32_t * >( surfPixels + ( m_context.y + y ) * surfPitch ) + m_context.x;
		for( uint32_t x = 0; x < m_context.width; ++x )
		{
			Vec3 resultColor;
			const float nx = ( (float)( x + m_context.x ) / surfWidth );
			const float ny = ( (float)( y + m_context.y ) / surfHeight );

			// make a ray from the origin to the current normalized pixel
			const Ray ray = m_context.camera->GenerateRay( nx, ny );

			resultColor = ComputeColor( *m_context.scene, ray );

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
