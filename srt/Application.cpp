// github test
#include "Application.h"
#include "resource.h"
#include <iostream>
#include <chrono>

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
#include "Lighting/LightingUtils.h"

#if defined (SRT_PLATFORM_WINDOWS )
	#include "Graphic/DIBDevice.h"
#endif

static constexpr uint32_t kSampleCount = 1;
static constexpr uint32_t kRayCount = 3;


namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
#if defined (SRT_PLATFORM_WINDOWS )
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_COMMAND:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
			}
			break;

			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
#endif

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::Application( const AppContext & context )
	: m_backBuffer{nullptr }
	, m_outputDev{ nullptr }
	{

		m_scene = new Scene;

		Material * mat1 = new Material{ Vec3{ 1.0f, 0.2f, 0.2f }, 0.4f, 0.3f };
		m_scene->AddObject( new Sphere{ Vec3{ 0.0f, 0.0f, -1.0f }, 0.5f, *mat1 } );

		Material * mat2 = new Material{ Vec3{ 0.4f, 1.0f, 0.2f }, 0.2f, 0.8f };
		m_scene->AddObject( new Sphere{ Vec3{ -1.0f, 0.0f, -1.0f }, 0.2f, *mat2 } );

		Material * mat3 = new Material{ Vec3{ 0.2f, 0.2f, 1.0f }, 0.7f, 0.0f };
		m_scene->AddObject( new Sphere{ Vec3{ 0.0f, -80.5f, -1.0f }, 80.0f, *mat3 } );

		m_scene->AddLight( new Light{ Light::Type::kDirectionnal, Vec3{ 0, 0.0f, 0.0f }, Vec3{ 1.0f, -1.0f, 1.0f }, Vec3{ 1.0f, 0.8f, 0.4f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 4.0f, 4.0f, 4.0f }, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ 5.0f, 5.0f, 4.0f } } );
		m_scene->AddLight( new Light{ Light::Type::kOmni, Vec3{ 4.0f, 4.0f, -4.0f }, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ 5.0f, 5.0f, 5.0f } } );

		m_backBuffer = new Image( context.width, context.height, PixelFormat::kBGRA8_UInt );

	#if defined( SRT_PLATFORM_WINDOWS )

		m_hInstance = context.hInstance;

		// Window class
		WNDCLASSEXW wcex{};

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style          = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc    = WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = context.hInstance;
		wcex.hIcon          = LoadIcon(context.hInstance, MAKEINTRESOURCE(IDI_SRT));
		wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName   = NULL;
		wcex.lpszClassName  = L"SRTWindowClass";
		wcex.hIconSm        = LoadIcon(NULL, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);

		// Compute the window size
		constexpr DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		RECT wndRect;
		SetRect( &wndRect, 0, 0, context.width, context.height);
		AdjustWindowRect( &wndRect, wndStyle, FALSE );

		// Create the window
		const int wndWidth = wndRect.right - wndRect.left;
		const int wndHeight = wndRect.bottom - wndRect.top;
		const int wndX = ( GetSystemMetrics( SM_CXSCREEN ) - wndWidth ) / 2;
		const int wndY = ( GetSystemMetrics( SM_CYSCREEN ) - wndHeight) / 2;
		m_hWnd = CreateWindow( L"SRTWindowClass", L"SRT", wndStyle, wndX, wndY, wndWidth, wndHeight, NULL, NULL, GetModuleHandle( NULL ), NULL );

		m_outputDev = new DIBDevice( m_hWnd );
	#endif
	}	

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::~Application()
	{
		delete m_scene;
		delete m_backBuffer;
		delete m_outputDev;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::Run( )
	{
		bool canContinue = true;
		float frameTime = 0.016f; // 16 ms by default

		while( canContinue )
		{
	#if defined( SRT_PLATFORM_WINDOWS )
			MSG msg;
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				if( LOWORD( msg.message )==WM_QUIT )
				{
					canContinue = false;
				}
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
	#endif
			auto startFrame = std::chrono::high_resolution_clock::now( );
			Update( frameTime );
			auto endFrame = std::chrono::high_resolution_clock::now( );

			frameTime = (float)( std::chrono::duration< double, std::milli >( endFrame - startFrame ).count() / 1000.0 );

			m_outputDev->OutputText( 10, 10, "FrameTime: %.4f ms (%u fps)", frameTime * 1000.0f, ( uint32_t )( 1000.0f / ( frameTime * 1000.0f ) ) );
			m_outputDev->Present( );
		}
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
	static Vec3 ComputeColor( const Scene & scene, const Ray& ray, uint32_t rayIdx )
	{
		Vec3 resultColor{ 0.0f, 0.0f, 0.0f };

		if( rayIdx < kRayCount )
		{
			SceneTraceResult result;
			scene.TraceRay( ray, 0.001f, FLT_MAX, result );

			if( result.hitResult.hitTime > 0.0f )
			{
				// Hit an object: compute lightings for all lights
				for( size_t lightIdx = 0; lightIdx < scene.GetLightCount(); ++lightIdx )
				{
					Light * light = scene.GetLight( lightIdx );

					// shadow ray
					Ray shadowRay{ result.hitResult.position, Normalize( light->GetPosition() - result.hitResult.position ) };
					SceneTraceResult shadowResult;
					scene.TraceRay( shadowRay, 0.001f, FLT_MAX, shadowResult );
					if( shadowResult.hitResult.hitTime < 0.0f )
					{
						LightSource	lightSource;
						InitLightSource( result, *light, lightSource );
						resultColor += ComputeBRDF( result, lightSource );
					}
				}

				// GI diffuse
				/*
				Vec3 target = result.hitResult.position + result.hitResult.normal + RandomUnitVector( );
				resultColor += 0.5f * ComputeColor( scene, Ray{ result.hitResult.position, Normalize( target - result.hitResult.position ) }, rayIdx + 1 );
				*/
				//resultColor = Clamp( resultColor, 0.0f, 1.0f );
				
			}
			else
			{
				// hit nothing: sky
				const float t = 0.5f * ( ray.Direction().Y() + 1.0f );
				resultColor = ( 1.0f - t ) * Vec3( 1.0f, 1.0f, 1.0f ) + t * Vec3( 0.5f, 0.7f, 1.0f );
			}

		}

		return resultColor;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::Update( float dt )
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

		// Do the real math!
		uint8_t * surf = reinterpret_cast< uint8_t * >( m_backBuffer->LockMipSurface( 0 ) );

		const uint32_t bbWidth = m_backBuffer->GetMipDesc( 0 ).width;
		const uint32_t bbHeight = m_backBuffer->GetMipDesc( 0 ).height;
		const float aspectRatio = (float)bbWidth / (float)bbHeight;

		static float t = 0.0f;
		t += dt;
		const float cs = cos( t );
		const float si = sin( t );

		SceneObject * obj = m_scene->GetObject( 1 );
		Vec3 objPos = Vec3( 0.0f, 0.0f, -1.0f ) + Vec3( cs * 0.7f, cs * 0.3f, si * 1.0f );
		obj->SetPosition( objPos );

		Light * light = m_scene->GetLight( 0 );
		Vec3 lightPos = light->GetPosition( );
		lightPos = lightPos + Vec3( -cs * 0.5f, 0.0f, 0.0f );
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
					// note: y is reverted so 1.0 is the top of the image and -1.0 is the bottom
					const float nx = ( ( (float)x + RandomFloat() * jitteringFactor ) / (float)bbWidth ) * 2.0f * aspectRatio - aspectRatio;
					const float ny = -( ( ( (float)y + RandomFloat() * jitteringFactor ) / (float)bbHeight ) * 2.0f - 1.0f );

					// make a ray from the origin to the current normalized pixel
					const Ray ray{ Vec3{ 0.0f, 0.0f, 1.0f }, Normalize( Vec3{ nx, ny, -1.0f } ) };

					resultColor += ComputeColor( *m_scene, ray, 0 );
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

		m_outputDev->BlitImage( *m_backBuffer );
	}
}
