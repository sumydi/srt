#include "Application.h"

#if defined( SRT_PLATFORM_LINUX )
namespace srt
{
	struct KeyConvert
	{
		guint		gtkKeyValue;
		KeyCode		keyCode;
	};

	// Only lists special keys
	static KeyConvert g_keyConvert[] = 
	{
		{ GDK_KEY_Escape,		KeyCode::kEscape },

		{ GDK_KEY_Page_Up,		KeyCode::kPageUp },
		{ GDK_KEY_Page_Down,	KeyCode::kPageDown },
		{ GDK_KEY_End,			KeyCode::kEnd },
		{ GDK_KEY_Home,			KeyCode::kHome },

		{ GDK_KEY_Left,			KeyCode::kLeft },
		{ GDK_KEY_Up,			KeyCode::kUp },
		{ GDK_KEY_Right,		KeyCode::kRight },
		{ GDK_KEY_Down,			KeyCode::kDown },
		{ GDK_KEY_space,		KeyCode::kSpace },

		{ GDK_KEY_KP_Add,		KeyCode::kAdd },
		{ GDK_KEY_KP_Subtract,	KeyCode::kSubtract },
		{ GDK_KEY_KP_Multiply,	KeyCode::kMultiply },
		{ GDK_KEY_KP_Divide,	KeyCode::kDivide }
	};
	constexpr size_t kKeyConvertSize = sizeof( g_keyConvert ) / sizeof( KeyConvert );
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	static KeyCode ConvertToKeyCode( guint keyVal )
	{
		KeyCode keyCode = KeyCode::kUnknow;

		if( keyVal >= GDK_KEY_KP_0 && keyVal <= GDK_KEY_KP_9 )
		{
			// Numpad 0 - 9
			keyCode = (KeyCode)( ( keyVal - GDK_KEY_KP_0 ) + (guint)KeyCode::k0 );
		}
		if( keyVal >= GDK_KEY_F1 && keyVal <= GDK_KEY_F10 )
		{
			// F1 - F10
			keyCode = (KeyCode)( ( keyVal - GDK_KEY_F1 ) + (guint)KeyCode::kF1 );
		}
		else if( keyVal >= 'a' && keyVal <= 'z' )
		{
			// A - Z
			keyCode = (KeyCode)( ( keyVal - 'a' ) + (guint)KeyCode::kA );
		}
		else if( keyVal == GDK_KEY_Shift_L || keyVal == GDK_KEY_Shift_R )
		{
			// Left or rihgt shift key
			keyCode = KeyCode::kShift;
		}
		else if( keyVal == GDK_KEY_Control_L || keyVal == GDK_KEY_Control_R )
		{
			// Left or rihgt control key
			keyCode = KeyCode::kControl;
		}
		else
		{
			// Others
			for( size_t i = 0; i < kKeyConvertSize; ++i )
			{
				if( g_keyConvert[ i ].gtkKeyValue==keyVal )
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
	static bool g_mustQuit = false;
	static void OnDestroy( GtkWidget * widget, gpointer userData )
	{
		g_mustQuit = true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::InitPlatform( const AppContext & context )
	{
		m_hApp = nullptr;
		gtk_init();
		
		m_hWnd = gtk_window_new();
		gtk_window_set_title( GTK_WINDOW( m_hWnd ), "SRT" );
		gtk_window_set_default_size( GTK_WINDOW( m_hWnd ), context.width, context.height + 40 );
		
		g_signal_connect( m_hWnd, "destroy", G_CALLBACK( OnDestroy ), nullptr );
		
		GtkEventController * keyCtl = gtk_event_controller_key_new();
		gtk_widget_add_controller( m_hWnd, keyCtl );
		g_signal_connect( keyCtl, "key-pressed", G_CALLBACK( OnKeyPressed ), this );
		g_signal_connect( keyCtl, "key-released", G_CALLBACK( OnKeyReleased ), this );
		
		gtk_window_present( GTK_WINDOW( m_hWnd ) );
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::TermPlatform( )
	{
		m_hWnd = nullptr;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	bool Application::UpdatePlatform( )
	{
		while( g_main_context_pending( NULL ) > 0 )
			g_main_context_iteration(NULL, TRUE);
		
		return g_mustQuit ? false : true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	gboolean Application::OnKeyPressed( GtkEventControllerKey * keyCtl, guint keyVal, guint keyCode, GdkModifierType state, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		const KeyCode appKeyCode = ConvertToKeyCode( keyVal );
		app->OnKeyDown( appKeyCode );
		
		return true;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	gboolean Application::OnKeyReleased( GtkEventControllerKey * keyCtl, guint keyVal, guint keyCode, GdkModifierType state, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		const KeyCode appKeyCode = ConvertToKeyCode( keyVal );
		app->OnKeyUp( appKeyCode );
		
		return true;
	}
}
#endif
