#include "RenderJobSimple.h"
#include "Math/Ray.h"
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
static Vec3 ComputeColor( const Scene & scene, const Ray & ray )
{
	Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

	SceneTraceResult primaryResult;
	scene.TraceRay( ray, 0.001f, FLT_MAX, primaryResult );

	if( primaryResult.hitResult.hitTime >= 0.0f )
	{
		resultColor = primaryResult.material->GetAlbedo();
	}
	else
	{
		// hit nothing: sky
		const float t = 0.5f * ray.Direction().Y() + 1.0f;
		resultColor = Lerp( Vec3{ 1.0f, 1.0f, 1.0f }, Vec3{ 0.5f, 0.7f, 1.0f }, t );
	}

	return resultColor;
}


// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
void RenderJobSimple::Execute( )
{
	const float surfWidth = static_cast< float >( m_context.backBuffer->GetMipDesc( 0 ).width - 1 );
	const float surfHeight = static_cast< float >( m_context.backBuffer->GetMipDesc( 0 ).height - 1 );
	const uint32_t surfPitch = m_context.backBuffer->GetMipDesc( 0 ).pitch;
	uint8_t * surfPixels = reinterpret_cast< uint8_t * >( m_context.backBuffer->LockMipSurface( 0 ) );

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

			*line = MakeRGB( resultColor );
			++line;
		}
	}

	m_context.backBuffer->UnlockMipSurface( 0 );
}
}
