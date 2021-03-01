#include "SrtApplication.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Ray.h"
#include "Math/Random.h"
#include "Math/RayHitTest.h"
#include "Scene/Scene.h"
#include "Scene/Sphere.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Lighting/LightingUtils.h"
#include "Graphic/WMOutputDevice.h"

static constexpr uint32_t kSampleCount = 1;
static constexpr uint32_t kRayCount = 2;

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	SrtApplication::SrtApplication( const AppContext & context )
	: Application( context )
	{
		m_scene = new Scene;

		Material * mat1 = new Material{ "White Metal", Vec3{ 1.0f, 1.0f, 1.0f }, 0.5f, 1.0f };
		m_scene->AddObject( new Sphere{ "Sphere0", Vec3{ 0.0f, 0.0f, -1.0f }, 0.5f, *mat1 } );

		Material * mat2 = new Material{ "Red", Vec3{ 1.0f, 0.2f, 0.2f }, 0.2f, 0.0f };
		m_scene->AddObject( new Sphere{ "Sphere1", Vec3{ -1.0f, 0.0f, -1.0f }, 0.2f, *mat2 } );

		Material * mat3 = new Material{ "Blue", Vec3{ 0.2f, 0.2f, 1.0f }, 0.7f, 0.0f };
		m_scene->AddObject( new Sphere{ "Sphere2", Vec3{ 0.0f, -80.5f, -1.0f }, 80.0f, *mat3 } );

		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ -1.0f, 2.0f, -3.0f }, Vec3{ 1.0f, -1.0f, 1.0f }, Vec3{ 5.0f, 5.0f, 5.0f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 2.0f, 1.5f, 2.0f }, Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ 5.0f, 5.0f, 4.0f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 4.0f, 4.0f, -2.0f }, Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ 5.0f, 5.0f, 5.0f } } );

		m_scene->AddCamera( new Camera{ Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ 0.0f, 0.0f, -1.0f }, 70.0f, (float)context.width / (float)context.height } );

		m_backBuffer = new Image( context.width, context.height, PixelFormat::kBGRA8_UInt );

		m_outputDev = new WMOutputDevice( this->GetWindowHandle() );
	}	

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	SrtApplication::~SrtApplication()
	{
		delete m_scene;
		delete m_backBuffer;
		delete m_outputDev;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::OnKeyDown( KeyCode key )
	{
		if( key==KeyCode::kMouseLeftButton )
		{
			m_mouseLeft = true;
		}
		else if( key==KeyCode::kMouseRightButton )
		{
			m_mouseRight = true;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::OnKeyUp( KeyCode key )
	{
		if( key==KeyCode::kP )
		{
			m_isPaused = !m_isPaused;
		}
		else if( key==KeyCode::kAdd )
		{
			Camera * camera = m_scene->GetCamera( 0 );
			camera->SetFOV( camera->GetFOV( ) + 1.0f );
		}
		else if( key==KeyCode::kSubtract )
		{
			Camera * camera = m_scene->GetCamera( 0 );
			camera->SetFOV( camera->GetFOV( ) - 1.0f );
		}
		else if( key==KeyCode::kDown )
		{
			Camera * camera = m_scene->GetCamera( 0 );
			camera->SetPosition( camera->GetPosition( ) + Vec3( 0.0f, 0.0f, 0.2f ) );
		}
		else if( key==KeyCode::kUp )
		{
			Camera * camera = m_scene->GetCamera( 0 );
			camera->SetPosition( camera->GetPosition( ) - Vec3( 0.0f, 0.0f, 0.2f ) );
		}
		else if( key==KeyCode::kMouseLeftButton )
		{
			m_mouseLeft = false;
		}
		else if( key==KeyCode::kMouseRightButton )
		{
			m_mouseRight = false;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::OnMouseMove( const MousePos & pos )
	{
		m_mousePos = pos;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	static Vec3 RandomInUnitSphere( )
	{
		const float x = RandomFloat( -1.0f, 1.0f );
		const float y = RandomFloat( -1.0f, 1.0f );
		const float z = RandomFloat( -1.0f, 1.0f );

		Vec3 v{ x, y, z };
		v = Normalize( v );
		const float t = RandomFloat( );
		v *= t;

		return v;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	static Vec3 RandomUnitVector( )
	{
		const float x = RandomFloat( -1.0f, 1.0f );
		const float y = RandomFloat( -1.0f, 1.0f );
		const float z = RandomFloat( -1.0f, 1.0f );

		Vec3 v{ x, y, z };
		v = Normalize( v );
		return v;
	}

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
				/*
				Vec3 target = primaryResult.hitResult.position + primaryResult.hitResult.normal + RandomUnitVector( );
				resultColor += 0.5f * ComputeColor( scene, Ray{ primaryResult.hitResult.position, Normalize( target - primaryResult.hitResult.position ) }, rayIdx + 1 );
				*/
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
	void SrtApplication::FrameStart( )
	{
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::FrameEnd( const float frameDuration )
	{
		m_outputDev->BlitImage( *m_backBuffer );
		
		int y = 10;
		m_outputDev->OutputText( 10, y, "FrameDuration: %.4f ms (%u fps)", frameDuration * 1000.0f, ( uint32_t )( 1000.0f / ( frameDuration * 1000.0f ) ) );

		y += 12;
		m_outputDev->OutputText( 10, y, "Focal: %.02f°", m_scene->GetCamera( 0 )->GetFOV( ) );
		y += 12;

		m_outputDev->OutputText( 10, y, "Mouse: %d, %d", m_mousePos.posX, m_mousePos.posY );
		y += 12;

		// Picking
		// -------
		if( m_mouseLeft )
		{
			Camera * camera = m_scene->GetCamera( 0 );

			const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
			const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;


			const float nx = (float)m_mousePos.posX / (float)m_backBuffer->GetMipDesc( 0 ).width;
			const float ny = (float)m_mousePos.posY / (float)m_backBuffer->GetMipDesc( 0 ).height;
			const Ray ray = camera->GenerateRay( nx, ny );

			SceneTraceResult	traceResult;
			m_scene->TraceRay( ray, 0.01f, FLT_MAX, traceResult );

			if( traceResult.hitResult.hitTime >= 0.0f )
			{
				m_outputDev->OutputText( 10, y , "HIT: %s", traceResult.material->GetName( ) );
			}
		}

		m_outputDev->Present( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::FrameUpdate( float dt )
	{
		// Unit test vectors
		{
			Vec3 vx{ 10.0f, 0.0f, 0.0f };

			const float len = Length( vx );
			Vec3 vxn = Normalize( vx );
			const float newLen = Length( vxn );

			Vec3 vy{ 0.0f, 5.0f, 0.0f };
			Vec3 vyn = Normalize( vy );

			Vec3 vz = Cross( vxn, vyn );

			Vec4 fcolor{ 1.0f, .0f, .0f, 1.0f };
		}

		dt = m_isPaused ? 0.0f : dt;

		// Do the real math!
		// -----------------
		uint8_t * surf = reinterpret_cast< uint8_t * >( m_backBuffer->LockMipSurface( 0 ) );

		const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
		const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;
		const float aspectRatio = (float)bbWidth / (float)bbHeight;

		static float t = 0.0f;
		t += dt;
		const float cs = cosf( t );
		const float si = sinf( t );

		SceneObject * obj = m_scene->GetObject( 1 );
		Vec3 objPos = Vec3{ 0.0f, 0.5f, -1.0f } + Vec3{ cs * 0.8f, cs * 0.3f, si * 1.0f };
		obj->SetPosition( objPos );

		Light * light = m_scene->GetLight( 0 );
		Vec3 lightPos = light->GetPosition( );
		lightPos = lightPos + Vec3{ -cs * 0.5f, 0.0f, 0.0f };
//		light->SetPosition( lightPos );

		Camera * camera = m_scene->GetCamera( 0 );

		// do not apply jitterring on the camera when kSamplecount==1 to avoid wobling picture
		const float jitteringFactor = kSampleCount > 1 ? 1.0f : 0.0f;

		for( uint32_t y = 0; y < bbHeight; ++y )
		{
			uint32_t *line = reinterpret_cast< uint32_t * >( surf + y * m_backBuffer->GetMipDesc( 0 ).pitch );
			for( uint32_t x = 0; x < bbWidth; ++x )
			{
				Vec3 resultColor{ 0.0f, 0.0f, 0.0f };
				for( uint32_t sampleIdx = 0; sampleIdx < kSampleCount; ++sampleIdx )
				{
					// transform coordinates to "normalized" coordinates
					const float nx = ( ( (float)x + RandomFloat() * jitteringFactor ) / (float)bbWidth );
					const float ny = ( ( (float)y + RandomFloat() * jitteringFactor ) / (float)bbHeight );

					// make a ray from the origin to the current normalized pixel
					const Ray ray = camera->GenerateRay( nx, ny );

					resultColor += ComputeColor( *m_scene, ray, kRayCount );
				}
				resultColor /= (float)kSampleCount;

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

		m_backBuffer->UnlockMipSurface( 0 );
	}


}
