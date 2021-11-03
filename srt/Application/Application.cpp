#include "Application.h"
#include <assert.h>

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::Application( const AppContext & context )
	{
		memset( m_keyPressed, 0, sizeof( m_keyPressed ) );
		InitPlatform( context );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::~Application()
	{
		TermPlatform( );
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

			// Update key states
			for( size_t key = (size_t)KeyCode::kEscape; key < (size_t)KeyCode::kCount; ++key )
			{
				if( m_keyPressed[ key ] )
				{
					if( !m_keyState[ key ].pressed )
						m_keyState[ key ].justPressed = 1;
					else
						m_keyState[ key ].justPressed = 0;
					m_keyState[ key ].pressed = 1;
				}
				else
				{
					m_keyState[ key ].justPressed = 0;
					m_keyState[ key ].pressed = 0;
				}
			}

			// Update frame
			FrameStart( );
			FrameUpdate( frameDuration );
			const auto endFrameTime = std::chrono::high_resolution_clock::now( );
			frameDuration = (float)( std::chrono::duration< double, std::milli >( endFrameTime - startFrameTime ).count() / 1000.0 );
			FrameEnd( frameDuration );
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	const KeyState & Application::GetKeyState( KeyCode key ) const
	{
		assert( key < KeyCode::kCount );

		return m_keyState[ (size_t)key ];
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::OnKeyDown( KeyCode key )
	{
		if( key < KeyCode::kCount )
		{
			m_keyPressed[ (size_t)key ] = 1;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::OnKeyUp( KeyCode key )
	{
		if( key < KeyCode::kCount )
		{
			m_keyPressed[ (size_t)key ] = 0;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::OnMouseMove( const MousePos & pos )
	{
		m_mousePosDelta.x = pos.x - m_mousePos.x;
		m_mousePosDelta.y = pos.y - m_mousePos.y;
		m_mousePos = pos;
	}



}
