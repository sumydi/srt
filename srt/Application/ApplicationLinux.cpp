#include "Application.h"

#if defined( SRT_PLATFORM_LINUX )
namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::InitPlatform( const AppContext & context )
	{
		m_hApp = gtk_application_new( "srt.sumydi", G_APPLICATION_DEFAULT_FLAGS );
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::TermPlatform( )
	{
	
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	bool Application::UpdatePlatform( )
	{
		bool canContinue = true;
		return canContinue;		
	}
}
#endif
