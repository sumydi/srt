#include "Application.h"

#if defined( SRT_PLATFORM_LINUX )
namespace srt
{
	namespace
	{
		static bool g_mustQuit = false;
		static void OnDestroy( GtkWidget * widget, gpointer userData )
		{
			g_mustQuit = true;
		}
	}
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::InitPlatform( const AppContext & context )
	{
		m_hApp = nullptr;
		gtk_init();
		m_hWnd = gtk_window_new();
		gtk_window_set_title( GTK_WINDOW( m_hWnd ), "SRT" );
		gtk_window_set_default_size( GTK_WINDOW( m_hWnd ), context.width, context.height );
		g_signal_connect( m_hWnd, "destroy", G_CALLBACK( OnDestroy ), nullptr );
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
		while( g_main_context_pending( NULL ) > 0 )
			g_main_context_iteration(NULL, TRUE);
		
		return g_mustQuit ? false : true;
	}
}
#endif
