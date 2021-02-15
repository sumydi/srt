#include "Application.h"

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Application::Application( const AppContext & context )
	{
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
			FrameStart( );
			FrameUpdate( frameDuration );
			const auto endFrameTime = std::chrono::high_resolution_clock::now( );
			frameDuration = (float)( std::chrono::duration< double, std::milli >( endFrameTime - startFrameTime ).count() / 1000.0 );
			FrameEnd( frameDuration );
		}
	}

}
