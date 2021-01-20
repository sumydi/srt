#include "Application.h"
#include "resource.h"
#include <iostream>
#include <chrono>

#include "Graphic/Image.h"
#include "Math/Vector3.h"

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
		delete m_backBuffer;
		delete m_outputDev;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::Run( )
	{
		bool canContinue = true;

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
			Update( );
			auto endFrame = std::chrono::high_resolution_clock::now( );
			const float frameTime = std::chrono::duration< float, std::milli >( endFrame - startFrame ).count();

		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::Update( )
	{
		Vec3 vx{ 10.0f, 0.0f, 0.0f };

		const float len = vx.Length();
		Vec3 vxn = Normalize( vx );
		const float newLen = vxn.Length();

		Vec3 vy{ 0.0f, 5.0f, 0.0f };
		Vec3 vyn = Normalize( vy );

		Vec3 vz = Cross( vxn, vyn );

		constexpr uint32_t color = ( 255 << 16 );
		uint32_t * surf = reinterpret_cast< uint32_t * >( m_backBuffer->LockMipSurface( 0 ) );

		for( uint32_t y = 0; y < m_backBuffer->GetMipDesc( 0 ).height; ++y )
		{
			uint32_t *line = surf + ( y * (uint32_t)m_backBuffer->GetMipDesc( 0 ).width );
			for( uint32_t x = 0; x < m_backBuffer->GetMipDesc( 0 ).width; ++x )
			{
				*line = color;
				++line;
			}
		}

		m_backBuffer->UnlockMipSurface( 0 );

		m_outputDev->Present( *m_backBuffer );
	}
}
