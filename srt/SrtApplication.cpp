#include "SrtApplication.h"
#include "JobScheduler/JobScheduler.h"
#include "Graphic/Image.h"
#include "Graphic/Material.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Ray.h"
#include "Math/Random.h"
#include "Math/RayHitTest.h"
#include "Math/Halton.h"
#include "Scene/Scene.h"
#include "Scene/Sphere.h"
#include "Scene/Quad.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Scene/SceneTraceResult.h"
#include "Graphic/WMOutputDevice.h"
#include "Lighting/LightingUtils.h"
#include "RenderJobRT.h"
#include "RenderJobSimple.h"
#include "RenderJobPathTracing.h"
#include <assert.h>
#include <cmath>

namespace srt
{
	static constexpr uint32_t kJobSchedulerThreadCount = 6;
	static constexpr uint32_t kWidthJobsCount = 16;
	static constexpr uint32_t kHeightJobsCount = 8;
	
	const char * SrtApplication::ms_renderModeName[ (size_t)RenderMode::kRenderModeCount ] =
	{
		{ "Simple" },
		{ "Ray Tracing" },
		{ "Path Tracing" }
	};

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	SrtApplication::SrtApplication( const AppContext & context )
	: Application( context )
	{

		m_jobScheduler = new JobScheduler{ kJobSchedulerThreadCount };

		m_scene = new Scene;

		Material *	metal = new Material{ "Metal" };
					metal->SetAlbedo( Vec3{ 0.8f, 0.6f, 0.2f } ).SetRoughness( 0.01f ).SetMetalness( 1.0f );

		Material *	grayPlastic = new Material{ "Gray Plastic" };
					grayPlastic->SetAlbedo( Vec3{ 0.8f, 0.8f, 0.8f } ).SetRoughness( 0.2f ).SetMetalness( 0.0f );

		Material *	greenPlastic = new Material{ "Green Plastic" };
					greenPlastic->SetAlbedo( Vec3{ 0.4f, 0.8f, 0.4f } ).SetRoughness( 0.2f ).SetMetalness( 0.0f );

		Material *	redPlastic = new Material{ "Red Plastic" };
					redPlastic->SetAlbedo( Vec3{ 0.8f, 0.2f, 0.2f } ).SetRoughness( 0.2f ).SetMetalness( 0.0f );

		Material *	bluePlastic = new Material{ "Blue Plastic" };
					bluePlastic->SetAlbedo( Vec3{ 0.2f, 0.2f, 0.8f } ).SetRoughness( 0.2f ).SetMetalness( 0.0f );

		Material *	ground = new Material{ "Ground" };
					ground->SetAlbedo( Vec3{ 0.8f, 0.4f, 0.2f } ).SetRoughness( 0.7f ).SetMetalness( 0.0f );

		Material *	glass = new Material{ "Glass" };
					glass->SetAlbedo( Vec3{ 0.5f, 0.5f, 0.5f } ).SetRoughness( 0.2f ).SetMetalness( 0.0f ).SetIOR( 2.2f );

		Material *	emissive = new Material{ "Emissive" };
					emissive->SetAlbedo( Vec3{ 0.0f, 0.0f, 0.0f } ).SetEmissive( Vec3{ 1.0f, 1.0f, 1.0f } );

		m_scene->AddObject( new Sphere{ "Sphere", Vec3{ -0.5f, 0.25f, -1.0f }, 0.25f, *grayPlastic } );
		m_scene->AddObject( new Sphere{ "Moving Sphere", Vec3{ 0.5f, 0.3f, -1.0f }, 0.3f, *metal } );
		//m_scene->AddObject( new Sphere{ "Ground", Vec3{ 0.0f, -80.5f, -1.0f }, 80.0f, *ground  } );
		m_scene->AddObject( new Quad{ "Ground", Vec3{ -40.0f, 0.0f, 40.0f }, Vec3{ 0.0f, 1.0f, 0.0 }, 80.0f, 80.0f, *ground } );
		m_scene->AddObject( new Sphere{ "Glass Sphere", Vec3{ 2.0f, 1.0f, -1.0f }, 0.5f, *glass } );

		m_scene->AddObject( new Quad{ "Left Wall", Vec3{ -1.0f, 0.0f, -0.5f }, Vec3{ 1.0f, 0.0f, 0.0 }, 2.0f, 2.5f, *redPlastic } );
		m_scene->AddObject( new Quad{ "Right Wall", Vec3{ 1.0f, 0.0f, -0.5f }, Vec3{ 1.0f, 0.0f, 0.0 }, 2.0f, 2.5f, *greenPlastic } );
		m_scene->AddObject( new Quad{ "Top Wall", Vec3{ -1.0f, 2.5f, -2.5f }, Vec3{ 0.0f, -1.0f, 0.0 }, 2.0f, 2.0f, *grayPlastic } );
		m_scene->AddObject( new Quad{ "Bottom Wall", Vec3{ -1.0f, 0.0f, -0.5f }, Vec3{ 0.0f, 1.0f, 0.0 }, 2.0f, 2.0f, *grayPlastic } );
		m_scene->AddObject( new Quad{ "Back Wall", Vec3{ -1.0f, 0.0f, -2.5f }, Vec3{ 0.0f, 0.0f, 1.0 }, 2.0f, 2.5f, *grayPlastic } );
		m_scene->AddObject( new Quad{ "Emissive Light", Vec3{ -0.25f, 2.45f, -1.5f }, Vec3{ 0.0f, -1.0f, 0.0 }, 0.5f, 0.5f, *emissive } );

		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ -1.0f, 2.0f, -3.0f }, Vec3{ 1.0f, -1.0f, 1.0f }, Vec3{ 5.0f, 5.0f, 5.0f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 2.0f, 1.5f, 2.0f }, Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ 5.0f, 5.0f, 4.0f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 4.0f, 4.0f, -2.0f }, Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ 5.0f, 5.0f, 5.0f } } );

		m_scene->AddCamera( new Camera{ Vec3{ 0.0f, 1.0f, 1.0f }, Vec3{ 0.0f, 1.0f, -1.0f }, 70.0f, (float)context.width / (float)context.height } );

		m_backBuffer = new Image( context.width, context.height, PixelFormat::kBGRA8_UInt );
		m_result = new Image( context.width, context.height, PixelFormat::kBGRA8_UInt );

		m_outputDev = new WMOutputDevice( this->GetWindowHandle() );
	}	

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	SrtApplication::~SrtApplication()
	{
		delete m_scene;
		delete m_backBuffer;
		delete m_result;
		delete m_outputDev;
		delete m_jobScheduler;
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
				if( GetKeyState( KeyCode::kPageUp ).justPressed )
				{
					m_pickResult.material->SetMetalness( m_pickResult.material->GetMetalness( ) + 0.05f );
				}
				else if( GetKeyState( KeyCode::kPageDown ).justPressed )
				{
					m_pickResult.material->SetMetalness( m_pickResult.material->GetMetalness( ) - 0.05f );
				}
			}
			else if( GetKeyState( KeyCode::kR ).pressed )
			{
				if( GetKeyState( KeyCode::kPageUp ).justPressed )
				{
					m_pickResult.material->SetRoughness( m_pickResult.material->GetRoughness( ) + 0.05f );
				}
				else if( GetKeyState( KeyCode::kPageDown ).justPressed )
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
		m_jobScheduler->ResetThreadStat();
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void SrtApplication::FrameEnd( const float frameDuration )
	{
		m_outputDev->BlitImage( *m_backBuffer );
		
		m_outputDev->SetTextColor( 0, 0, 0 );
		m_outputDev->SetTextPosition( 10, 10 );
		m_outputDev->PushText( "FrameDuration: %.4f ms (%u fps)", frameDuration * 1000.0f, ( uint32_t )( 1000.0f / ( frameDuration * 1000.0f ) ) );

		m_outputDev->PushText( "Render mode: %s", ms_renderModeName[ (size_t)m_renderMode ] );

	#if defined( SRT_JOBSCHEDULER_STATS )
		const size_t threadCount = m_jobScheduler->GetThreadCount( );
		for( size_t threadIdx = 0; threadIdx < threadCount; ++threadIdx )
		{
			m_outputDev->PushText( "Thread%zu jobs: %u", threadIdx, m_jobScheduler->GetThreadStat( threadIdx ).jobProcessed );
		}
	#endif

		m_outputDev->PushText( "Focal: %.02f�", m_scene->GetCamera( 0 )->GetFOV( ) );
		m_outputDev->PushText( "Rays: %u", m_rayCount );
		m_outputDev->PushText( "Samples: %u", m_sampleCount );

		//const MousePos & mousePos = GetMousePos( );
		//m_outputDev->PushText( "Mouse: %d, %d", mousePos.x, mousePos.y );

		//const MousePos & mousePosDelta = GetMousePosDelta( );
		//m_outputDev->PushText( "MouseDelta: %d, %d", mousePosDelta.x, mousePosDelta.y );

		if( m_pickResult.hitResult.hitTime >= 0.0f )
		{
			m_outputDev->PushText( "HIT", m_pickResult.material->GetName( ) );
			m_outputDev->PushText( "  Object %s", m_pickResult.object->GetName( ) );
			m_outputDev->PushText( "  Material %s", m_pickResult.material->GetName( ) );
			m_outputDev->PushText( "    Position  : %.3f, %.3f, %.3f", m_pickResult.hitResult.position.X(), m_pickResult.hitResult.position.Y(), m_pickResult.hitResult.position.Z() );
			m_outputDev->PushText( "    Albedo    : %.3f, %.3f, %.3f", m_pickResult.material->GetAlbedo().X(), m_pickResult.material->GetAlbedo().Y(), m_pickResult.material->GetAlbedo().Z() );

			if( GetKeyState( KeyCode::kR ).pressed )
				m_outputDev->SetTextColor( 255, 0, 0 );
			else
				m_outputDev->SetTextColor( 0, 0, 0 );
			m_outputDev->PushText( "    Roughness: %.2f", m_pickResult.material->GetRoughness() );

			if( GetKeyState( KeyCode::kM ).pressed )
				m_outputDev->SetTextColor( 255, 0, 0 );
			else
				m_outputDev->SetTextColor( 0, 0, 0 );
			m_outputDev->PushText( "    Metalness : %.2f", m_pickResult.material->GetMetalness() );

			m_outputDev->SetTextColor( 0, 0, 0 );
			m_outputDev->PushText( "    IOR       : %.2f", m_pickResult.material->GetIOR() );

			const Vec3 f0 = ComputeF0( m_pickResult.material->GetAlbedo(), m_pickResult.material->GetMetalness() );
			m_outputDev->PushText( "    F0        : %.2f, %.2f, %.2f", f0.X(), f0.Y(), f0.Z() );
		}

		m_outputDev->Present( );
		++m_frameIndex;
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

		// Camera
		bool cameraChanged = false;
		if( GetKeyState( KeyCode::kMouseMiddleButton).pressed )
		{
			constexpr float moveDT { 0.004f };
			const float dx = (float)( GetMousePosDelta().x ) * moveDT;
			const float dy = (float)( GetMousePosDelta().y ) * moveDT;

			Vec3 right, front, up;
			camera->BuildBasis( right, front, up );

			if( GetKeyState( KeyCode::kShift ).pressed )
			{
				// Camera panning
				const Vec3 camMove = right * -dx + up * dy;
				camera->SetPosition( camera->GetPosition( ) + camMove );
				camera->SetLookAt( camera->GetLookAt( ) + camMove );
			}
			else if( GetKeyState( KeyCode::kControl ).pressed )
			{
				// Camera zoom in/out: stop the zoom if too near to lookAt position
				const Vec3 camMove = front * -dy;
				if( Length( camera->GetLookAt() - ( camera->GetPosition() + camMove ) ) > 0.01f )
				{
					camera->SetPosition( camera->GetPosition( ) + camMove );
				}
			}
			else
			{
				// Camera rotation
				const Vec3 camMove = right * -dx + up * dy;
				camera->SetPosition( camera->GetPosition( ) + camMove );
			}
			m_frameIndex = 0;
			cameraChanged = true;
		}

		if( GetKeyState( KeyCode::kC ).justPressed && ( m_pickResult.hitResult.hitTime > 0.0f ) )
		{
			// Center camera on picked position
			const Vec3 camDir { camera->GetLookAt() - camera->GetPosition() };
			const Vec3 newLookAt { m_pickResult.hitResult.position };
			const Vec3 newCamPos { newLookAt - camDir };
			camera->SetPosition( newCamPos );
			camera->SetLookAt( newLookAt );
			m_frameIndex = 0;
		}

		// Rendering mode
		if( GetKeyState( KeyCode::kF1 ).justPressed )
		{
			m_renderMode = RenderMode::kSimple;
			m_frameIndex = 0;
		}
		else if( GetKeyState( KeyCode::kF2 ).justPressed )
		{
			m_renderMode = RenderMode::kPBR;
			m_frameIndex = 0;
		}
		else if( GetKeyState( KeyCode::kF3 ).justPressed )
		{
			m_renderMode = RenderMode::kFullRT;
			m_frameIndex = 0;
		}

		// Number of rays
		if( GetKeyState( KeyCode::kAdd ).justPressed )
		{
			if( m_rayCount < 32 )
				m_rayCount++;
		}
		else if( GetKeyState( KeyCode::kSubtract ).justPressed )
		{
			if( m_rayCount > 1 )
				m_rayCount--;
		}

		// Number of samples
		if( GetKeyState( KeyCode::kMultiply ).justPressed )
		{
			if( m_sampleCount < 50 )
				m_sampleCount++;
		}
		else if( GetKeyState( KeyCode::kDivide ).justPressed )
		{
			if( m_sampleCount > 1 )
				m_sampleCount--;
		}

		UpdateEditMode( );

		if( m_frameIndex == 0 )
		{
			m_result->ClearMipSurface( 0, Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
		}

		dt = m_isPaused ? 0.0f : dt;

		// Let's move some scene object
		// -----------------------------
		static float t = 0.0f;
		t += dt;
		const float cs = cosf( t );
		const float si = sinf( t );

		SceneObject * obj = m_scene->GetObject( 1 );
		Vec3 objPos = Vec3{ 0.0f, 0.5f, -1.0f } + Vec3{ cs * 0.8f, cs * 0.8f, si * 1.0f };
//		obj->SetPosition( objPos );

//		Light * light = m_scene->GetLight( 0 );
//		Vec3 lightPos = light->GetPosition( );
//		lightPos = lightPos + Vec3{ -cs * 0.5f, 0.0f, 0.0f };
//		light->SetPosition( lightPos );


		// Render!
		// -------
		
		const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
		const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;

		// Compute remaining pixel to process if job count is not multiple of width or height
		const uint32_t lastPixWidth = bbWidth % kWidthJobsCount;
		const uint32_t lastPixHeight = bbHeight % kHeightJobsCount;

		RenderJob::Context context;
		context.backBuffer = m_backBuffer;
		context.scene = m_scene;
		context.camera = camera;
		context.prevResult = m_result;
		context.curResult = m_result;
		context.sampleCount = m_sampleCount;
		context.rayCount = m_rayCount;
		context.frameIndex = m_frameIndex;

		Halton halton( context.sampleCount );
		context.halton = &halton;

		size_t			jobIdx = 0;

		for( uint32_t heightJob = 0; heightJob < kHeightJobsCount; ++heightJob )
		{
			context.y = ( bbHeight / kHeightJobsCount ) * heightJob;
			context.height = ( bbHeight / kHeightJobsCount );
			context.height += ( heightJob == kHeightJobsCount - 1 ) ? lastPixHeight : 0;

			for( uint32_t widthJob = 0; widthJob < kWidthJobsCount; ++widthJob )
			{
				context.x = ( bbWidth / kWidthJobsCount ) * widthJob;
				context.width = ( bbWidth / kWidthJobsCount );
				context.width += ( widthJob == kWidthJobsCount - 1 ) ? lastPixWidth : 0;

				RenderJob * job { nullptr };
				if( m_renderMode==RenderMode::kSimple || cameraChanged )
				{
					job = m_freeAllAllocator.Allocate< RenderJobSimple >( );
				}
				else if( m_renderMode==RenderMode::kPBR )
				{
					job = m_freeAllAllocator.Allocate< RenderJobRayTracing >( );
				}
				else if( m_renderMode==RenderMode::kFullRT )
				{
					job = m_freeAllAllocator.Allocate< RenderJobPathTracing >( );
				}

				assert( job!=nullptr );

				job->SetContext( context );
				m_jobScheduler->PushJob( job );
				++jobIdx;
			}
		}

		m_jobScheduler->WaitForJobs( );

		// Pick
		// ----
		if( GetKeyState( KeyCode::kMouseLeftButton ).justPressed )
		{
			const float nx = (float)GetMousePos().x / (float)bbWidth;
			const float ny = (float)GetMousePos().y / (float)bbHeight;
			const Ray ray = camera->GenerateRay( nx, ny );

			SceneTraceResult	traceResult;
			m_scene->TraceRay( ray, 0.01f, FLT_MAX, traceResult );
			m_pickResult = traceResult;
		}

		m_freeAllAllocator.FreeAll();
	}


}
