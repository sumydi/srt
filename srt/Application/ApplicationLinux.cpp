#include "Application.h"

#if defined( SRT_PLATFORM_LINUX )
namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::InitPlatform( const AppContext & context )
	{
		m_hApp = nullptr;
		gtk_init();
		m_hWnd = gtk_window_new();
		gtk_window_set_title( GTK_WINDOW( m_hWnd ), "SRT" );
		gtk_window_set_default_size( GTK_WINDOW( m_hWnd ), context.width, context.height );
		gtk_window_present( GTK_WINDOW( m_hWnd ) );
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::TermPlatform( )
	{
		g_object_unref( m_hApp );
		m_hApp = nullptr;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	bool Application::UpdatePlatform( )
	{
		bool canContinue = true;
		//while( g_list_model_get_n_items( gtk_window_get_toplevels() ) > 0 )
		while( g_main_context_pending( NULL ) > 0 )
			g_main_context_iteration(NULL, TRUE);
		
		return canContinue;		
	}
}
#endif
