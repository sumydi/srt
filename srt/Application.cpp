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

#if defined (SRT_PLATFORM_WINDOWS )
	#include "Graphic/DIBDevice.h"
#endif

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

		Material * mat1 = new Material{ Vec3{ 1.0f, 0.0f, 0.0f }, 0.5f, 0.0f };
		m_scene->AddObject( new Sphere{ Vec3{ 0.0f, 0.0f, -1.0f }, 0.5f, *mat1 } );
		m_scene->AddObject( new Sphere{ Vec3{ -1.0f, 0.0f, -1.0f }, 0.2f, *mat1 } );

		Material * mat2 = new Material{ Vec3{ 0.0f, 0.0f, 1.0f }, 0.5f, 0.0f };
		m_scene->AddObject( new Sphere{ Vec3{ 0.0f, -100.5f, -1.0f }, 100.0f, *mat2 } );

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
	
		/*
		Vec3	p;
		do
		{
			p = 2.0f * Vec3( (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX ) - Vec3( 1.0f, 1.0f, 1.0f );
		}while( Dot( p, p ) >= 1.0f );
		return p;
		*/
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

		if( rayIdx < 4 )
		{
			SceneTraceResult result;
			scene.TraceRay( ray, 0.001f, FLT_MAX, result );


			if( result.hitResult.hitTime > 0.0f )
			{
				// hits an object
				//resultColor = 0.5f * Vec3{ result.hitResult.normal.X() + 1.0f, result.hitResult.normal.Y() + 1.0f, result.hitResult.normal.Z() + 1.0f };

				Vec3 target = result.hitResult.position + result.hitResult.normal + RandomUnitVector( );
				resultColor = 0.5f * ComputeColor( scene, Ray{ result.hitResult.position, Normalize( target - result.hitResult.position ) }, rayIdx + 1 );
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
		const float cs = cos( t ) * 0.6f * dt;

		SceneObject * obj = m_scene->GetObject( 1 );
		Vec3 objPos = obj->GetPosition() ;
		objPos = objPos + Vec3( 0.0f, cs, 0.0f );
		obj->SetPosition( objPos );

		for( uint32_t y = 0; y < bbHeight; ++y )
		{
			uint32_t *line = reinterpret_cast< uint32_t * >( surf + y * m_backBuffer->GetMipDesc( 0 ).pitch );
			for( uint32_t x = 0; x < bbWidth; ++x )
			{
				// transform coordinates to "normalized" coordinates
				// note: y is reverted so 1.0 is the top of the image and -1.0 is the bottom
				const float nx = ( (float)x / (float)bbWidth ) * 2.0f * aspectRatio - aspectRatio;
				const float ny = -( ( (float)y / (float)bbHeight ) * 2.0f - 1.0f );

				// make a ray from the origin to the current normalized pixel
				const Ray ray{ Vec3{ 0.0f, 0.0f, 1.0f }, Normalize( Vec3{ nx, ny, -1.0f } ) };

				Vec3 resultColor = ComputeColor( *m_scene, ray, 0 );

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

		m_outputDev->Present( *m_backBuffer );
	}
}
