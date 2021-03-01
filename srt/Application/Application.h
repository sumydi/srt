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

		void			Run( );
		WindowHandle	GetWindowHandle( ) const { return m_hWnd; }

	protected:

		//
		virtual void	FrameStart( ) { };

		// dt is the delta time between the previous frame and the current frame in second
		virtual void	FrameUpdate( const float dt ) = 0;

		// frameDuration is the time taken to process the current frame in second
		virtual void	FrameEnd( const float frameDuration ) { };

		virtual void	OnKeyUp( KeyCode key ) = 0;
		virtual void	OnKeyDown( KeyCode key ) = 0;

		virtual void	OnMouseMove( const MousePos & pos ) = 0;

	private:
		Application( ) = delete;
		Application operator = ( const Application & other ) = delete;

		void	InitPlatform( const AppContext & context );
		void	TermPlatform( );

	#if defined( SRT_PLATFORM_WINDOWS )
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	#endif
		AppHandle		m_hApp;
		WindowHandle	m_hWnd;
	};
}
#endif
