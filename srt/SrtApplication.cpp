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
#include "Graphic/WMOutputDevice.h"
#include "Lighting/LightingUtils.h"
#include "RenderJobPBR.h"
#include "RenderJobSimple.h"
#include "RenderJobFullRT.h"

namespace srt
{
	static constexpr uint32_t kJobSchedulerThreadCount = 6;
	static constexpr uint32_t kWidthJobsCount = 16;
	static constexpr uint32_t kHeightJobsCount = 8;

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	SrtApplication::SrtApplication( const AppContext & context )
	: Application( context )
	{

		m_jobScheduler = new JobScheduler{ kJobSchedulerThreadCount };

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

	#if defined( SRT_JOBSCHEDULER_STATS )
		const size_t threadCount = m_jobScheduler->GetThreadCount( );
		for( size_t threadIdx = 0; threadIdx < threadCount; ++threadIdx )
		{
			m_outputDev->PushText( "Thread%zu jobs: %u", threadIdx, m_jobScheduler->GetThreadStat( threadIdx ).jobProcessed );
		}
	#endif

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

		if( GetKeyState( KeyCode::kF1 ).justPressed )
		{
			m_renderMode = RenderMode::kSimple;
		}
		else if( GetKeyState( KeyCode::kF2 ).justPressed )
		{
			m_renderMode = RenderMode::kPBR;
		}
		else if( GetKeyState( KeyCode::kF3 ).justPressed )
		{
			m_renderMode = RenderMode::kFullRT;
		}

		UpdateEditMode( );


		dt = m_isPaused ? 0.0f : dt;

		// Let's move some scene object
		// -----------------------------
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


		// Render!
		// -------
		
		// TODO: rewrite this!
		RenderJobPBR	jobsPBR[ kWidthJobsCount * kHeightJobsCount ];
		RenderJobSimple	jobsSimple[ kWidthJobsCount * kHeightJobsCount ];
		RenderJobFullRT	jobsFullRT[ kWidthJobsCount * kHeightJobsCount ];
		RenderJob *		jobs = nullptr;

		switch( m_renderMode )
		{
			case RenderMode::kSimple:
				jobs = jobsSimple;
				break;

			case RenderMode::kPBR:
				jobs = jobsPBR;
				break;

			case RenderMode::kFullRT:
				jobs = jobsFullRT;
				break;

			default:
				return;
		}

		size_t			jobIdx = 0;

		const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
		const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;

		RenderJob::Context context{ m_backBuffer, m_scene, camera };
		context.width = ( bbWidth / kWidthJobsCount );
		context.height = ( bbHeight / kHeightJobsCount );
		context.sampleCount = 1;

		for( uint32_t heightJob = 0; heightJob < kHeightJobsCount; ++heightJob )
		{
			context.y = ( bbHeight / kHeightJobsCount ) * heightJob;
			for( uint32_t widthJob = 0; widthJob < kWidthJobsCount; ++widthJob )
			{
				context.x = ( bbWidth / kWidthJobsCount ) * widthJob;
				jobs[ jobIdx ].SetContext( context );
				m_jobScheduler->PushJob( &jobs[ jobIdx ] );
				++jobIdx;
			}
		}

		m_jobScheduler->WaitForJobs( );

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
