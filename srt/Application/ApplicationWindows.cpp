#include "Application.h"

#if defined( SRT_PLATFORM_WINDOWS )
#include "resource.h"
#include "InputDevice.h"

namespace srt
{

	struct KeyConvert
	{
		WPARAM		wndKeyCode;
		KeyCode		keyCode;
	};

	// Only lists special keys
	static KeyConvert g_keyConvert[] = 
	{
		{ VK_ESCAPE,	KeyCode::kEscape },

		{ VK_PRIOR,		KeyCode::kPageUp },
		{ VK_NEXT,		KeyCode::kPageDown },
		{ VK_END,		KeyCode::kEnd },
		{ VK_HOME,		KeyCode::kHome },

		{ VK_LEFT,		KeyCode::kLeft },
		{ VK_UP,		KeyCode::kUp },
		{ VK_RIGHT,		KeyCode::kRight },
		{ VK_DOWN,		KeyCode::kDown },
		{ VK_SPACE,		KeyCode::kSpace },

		{ VK_ADD,		KeyCode::kAdd },
		{ VK_SUBTRACT,	KeyCode::kSubtract },
		{ VK_MULTIPLY,	KeyCode::kMultiply },
		{ VK_DIVIDE,	KeyCode::kDivide }
	};
	constexpr size_t kKeyConvertSize = sizeof( g_keyConvert ) / sizeof( KeyConvert );

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	static KeyCode ConvertToKeyCode( WPARAM wParam )
	{
		KeyCode keyCode = KeyCode::kUnknow;

		if( wParam >= 0x30 && wParam <= 0x39 )
		{
			// 0 - 9
			keyCode = (KeyCode)( ( wParam - 0x30 ) + (WPARAM)KeyCode::k0 );
		}
		else if( wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9 )
		{
			// Numpad 0 - 9
			keyCode = (KeyCode)( ( wParam - VK_NUMPAD0 ) + (WPARAM)KeyCode::k0 );
		}
		else if( wParam >= VK_F1 && wParam < VK_F10 )
		{
			// F1 - F10
			keyCode = (KeyCode)( ( wParam - VK_F1 ) + (WPARAM)KeyCode::kF1 );
		}
		else if( wParam >= 0x41 && wParam <= 0x5a )
		{
			// A - Z
			keyCode = (KeyCode)( ( wParam - 0x41 ) + (WPARAM)KeyCode::kA );
		}
		else
		{
			// Others
			for( size_t i = 0; i < kKeyConvertSize; ++i )
			{
				if( g_keyConvert[ i ].wndKeyCode==wParam )
				{
					keyCode = g_keyConvert[ i ].keyCode;
					break;
				}
			}
		}

		return keyCode;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Application * app = reinterpret_cast< Application * >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		switch (message)
		{
			case WM_COMMAND:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

			case WM_KEYDOWN:
			{
				const KeyCode keyCode = ConvertToKeyCode( wParam );
				app->OnKeyDown( keyCode );
			}
			break;

			case WM_KEYUP:
			{
				const KeyCode keyCode = ConvertToKeyCode( wParam );
				app->OnKeyUp( keyCode );
			}
			break;

			case WM_MOUSEMOVE:
			{
				POINTS  points = MAKEPOINTS( lParam );
				MousePos pos { points.x, points.y };
				app->OnMouseMove( pos );

			}
			break;

			case WM_LBUTTONDOWN:
			{
				app->OnKeyDown( KeyCode::kMouseLeftButton );
			}
			break;

			case WM_LBUTTONUP:
			{
				app->OnKeyUp( KeyCode::kMouseLeftButton );
			}
			break;

			case WM_MBUTTONDOWN:
			{
				app->OnKeyDown( KeyCode::kMouseMiddleButton );
			}
			break;

			case WM_MBUTTONUP:
			{
				app->OnKeyUp( KeyCode::kMouseMiddleButton );
			}
			break;

			case WM_RBUTTONDOWN:
			{
				app->OnKeyDown( KeyCode::kMouseRightButton );
			}
			break;

			case WM_RBUTTONUP:
			{
				app->OnKeyUp( KeyCode::kMouseRightButton );
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

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::InitPlatform( const AppContext & context )
	{
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
		SetWindowLongPtr( m_hWnd, GWLP_USERDATA, (LONG_PTR)this );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::TermPlatform( )
	{
	
	}
}


#endif
