#include "Application.h"

#if defined( SRT_PLATFORM_WINDOWS )
	#include "resource.h"
#endif

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
#if defined( SRT_PLATFORM_WINDOWS )
	LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_COMMAND:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

			case WM_KEYDOWN:
				break;

			case WM_KEYUP:
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
	{
	#if defined( SRT_PLATFORM_WINDOWS )
		m_hApp = context.appHandle;

		// Window class
		WNDCLASSEXW wcex{};

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style          = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc    = WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = context.appHandle;
		wcex.hIcon          = LoadIcon(context.appHandle, MAKEINTRESOURCE(IDI_SRT));
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
	#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::~Application()
	{
	
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::Run( )
	{
		bool canContinue = true;
		float frameDuration = 0.016f; // 16 ms by default

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

			const auto startFrameTime = std::chrono::high_resolution_clock::now( );
			FrameStart( );
			FrameUpdate( frameDuration );
			const auto endFrameTime = std::chrono::high_resolution_clock::now( );
			frameDuration = (float)( std::chrono::duration< double, std::milli >( endFrameTime - startFrameTime ).count() / 1000.0 );
			FrameEnd( frameDuration );
		}
	}

}
