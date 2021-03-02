#ifndef SRT_APPLICATION_H
#define SRT_APPLICATION_H

// ============================================================================
//
//	This is the platform specific implementation of an application
//
// ============================================================================
#include "Base.h"
#include "InputDevice.h"
#include <chrono>

namespace srt
{
	// ============================================================================
	//
	//	Application context: holds all informations like output width & height,
	//	windows instance, ....
	//
	// ============================================================================
	struct AppContext
	{
		uint32_t	width;
		uint32_t	height;
		AppHandle	appHandle;	
	};

	// ============================================================================
	//
	//
	//
	// ============================================================================
	class Application
	{
	public:

		Application( const AppContext & context );
		virtual ~Application( );

		void				Run( );
		WindowHandle		GetWindowHandle( ) const { return m_hWnd; }

		const KeyState &	GetKeyState( KeyCode key ) const;

		const MousePos &	GetMousePos( ) const { return m_mousePos; }

	protected:

		//
		virtual void	FrameStart( ) { };

		// dt is the delta time between the previous frame and the current frame in second
		virtual void	FrameUpdate( const float dt ) = 0;

		// frameDuration is the time taken to process the current frame in second
		virtual void	FrameEnd( const float frameDuration ) { };

		void	OnKeyDown( KeyCode key );
		void	OnKeyUp( KeyCode key );

		void	OnMouseMove( const MousePos & pos );

	private:
		Application( ) = delete;
		Application operator = ( const Application & other ) = delete;

		void	InitPlatform( const AppContext & context );
		void	TermPlatform( );

		AppHandle		m_hApp;
		WindowHandle	m_hWnd;

		uint8_t			m_keyPressed[ (size_t)KeyCode::kCount ];
		KeyState		m_keyState[ (size_t)KeyCode::kCount ];
		MousePos		m_mousePos;

		#if defined( SRT_PLATFORM_WINDOWS )
			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		#endif
	};
}
#endif
