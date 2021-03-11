#include "SrtApplication.h"
#include "JobScheduler/JobScheduler.h"
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
#include "Scene/SceneTraceResult.h"
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

		m_jobScheduler = new JobScheduler{ 4 };

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
		delete m_jobScheduler;
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
	void SrtApplication::UpdateEditMode( )
	{
		//
		if( m_pickResult.material!=nullptr )
		{
			if( GetKeyState( KeyCode::kM ).pressed  )
			{
				if( GetKeyState( KeyCode::kAdd ).pressed )
				{
					m_pickResult.material->SetMetalness( m_pickResult.material->GetMetalness( ) + 0.05f );
				}
				else if( GetKeyState( KeyCode::kSubtract ).pressed )
				{
					m_pickResult.material->SetMetalness( m_pickResult.material->GetMetalness( ) - 0.05f );
				}
			}
			else if( GetKeyState( KeyCode::kR ).pressed )
			{
				if( GetKeyState( KeyCode::kAdd ).pressed )
				{
					m_pickResult.material->SetRoughness( m_pickResult.material->GetRoughness( ) + 0.05f );
				}
				else if( GetKeyState( KeyCode::kSubtract ).pressed )
				{
					m_pickResult.material->SetRoughness( m_pickResult.material->GetRoughness( ) - 0.05f );
				}
			}
		}
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
		
		m_outputDev->SetTextColor( 0, 0, 0 );
		m_outputDev->SetTextPosition( 10, 10 );
		m_outputDev->PushText( "FrameDuration: %.4f ms (%u fps)", frameDuration * 1000.0f, ( uint32_t )( 1000.0f / ( frameDuration * 1000.0f ) ) );
		m_outputDev->PushText( "Focal: %.02f°", m_scene->GetCamera( 0 )->GetFOV( ) );

		const MousePos & mousePos = GetMousePos( );
		m_outputDev->PushText( "Mouse: %d, %d", mousePos.posX, mousePos.posY );

		if( m_pickResult.hitResult.hitTime >= 0.0f )
		{
			m_outputDev->PushText( "HIT", m_pickResult.material->GetName( ) );
			m_outputDev->PushText( "  Object %s", m_pickResult.object->GetName( ) );
			m_outputDev->PushText( "  Material %s", m_pickResult.material->GetName( ) );
			m_outputDev->PushText( "    Albedo    : %.3f, %.3f, %.3f", m_pickResult.material->GetAlbedo().X(), m_pickResult.material->GetAlbedo().Y(), m_pickResult.material->GetAlbedo().Z() );

			if( GetKeyState( KeyCode::kR ).pressed )
				m_outputDev->SetTextColor( 255, 0, 0 );
			else
				m_outputDev->SetTextColor( 0, 0, 0 );
			m_outputDev->PushText( "    Rougness  : %.2f", m_pickResult.material->GetRoughness() );

			if( GetKeyState( KeyCode::kM ).pressed )
				m_outputDev->SetTextColor( 255, 0, 0 );
			else
				m_outputDev->SetTextColor( 0, 0, 0 );
			m_outputDev->PushText( "    Metalness : %.2f", m_pickResult.material->GetMetalness() );

			m_outputDev->SetTextColor( 0, 0, 0 );
			const Vec3 f0 = ComputeF0( m_pickResult.material->GetAlbedo(), m_pickResult.material->GetMetalness() );
			m_outputDev->PushText( "    F0        : %.2f, %.2f, %.2f", f0.X(), f0.Y(), f0.Z() );
		}

		m_outputDev->Present( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::FrameUpdate( float dt )
	{
		Camera * camera = m_scene->GetCamera( 0 );

		if( GetKeyState( KeyCode::kP ).justPressed )
		{
			m_isPaused = !m_isPaused;
		}

		if( GetKeyState( KeyCode::kDown ).pressed )
		{
			camera->SetPosition( camera->GetPosition( ) + Vec3( 0.0f, 0.0f, 0.2f ) * dt );
		}

		if( GetKeyState( KeyCode::kUp ).pressed )
		{
			camera->SetPosition( camera->GetPosition( ) - Vec3( 0.0f, 0.0f, 0.2f ) * dt );
		}

		UpdateEditMode( );


		dt = m_isPaused ? 0.0f : dt;

		// Do the real math!
		// -----------------
		uint8_t * surf = reinterpret_cast< uint8_t * >( m_backBuffer->LockMipSurface( 0 ) );

		const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
		const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;

		static float t = 0.0f;
		t += dt;
		const float cs = cosf( t );
		const float si = sinf( t );

		SceneObject * obj = m_scene->GetObject( 1 );
		Vec3 objPos = Vec3{ 0.0f, 0.5f, -1.0f } + Vec3{ cs * 0.8f, cs * 0.3f, si * 1.0f };
		obj->SetPosition( objPos );

//		Light * light = m_scene->GetLight( 0 );
//		Vec3 lightPos = light->GetPosition( );
//		lightPos = lightPos + Vec3{ -cs * 0.5f, 0.0f, 0.0f };
//		light->SetPosition( lightPos );

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

		// Pick
		// ----
		if( GetKeyState( KeyCode::kMouseLeftButton ).justPressed )
		{
			const float nx = (float)GetMousePos().posX / (float)bbWidth;
			const float ny = (float)GetMousePos().posY / (float)bbHeight;
			const Ray ray = camera->GenerateRay( nx, ny );

			SceneTraceResult	traceResult;
			m_scene->TraceRay( ray, 0.01f, FLT_MAX, traceResult );
			m_pickResult = traceResult;
		}
	}


}
