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
	static GtkGesture * NewMouseClickGesture( GtkWidget * parent, guint button )
	{
		GtkGesture * gesture = gtk_gesture_click_new();
		
		gtk_widget_add_controller( parent, GTK_EVENT_CONTROLLER( gesture ) );
		gtk_gesture_single_set_button( GTK_GESTURE_SINGLE( gesture ), button );
		
		return gesture;
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
		
		// Keyboard
		{
			GtkEventController * ctl = gtk_event_controller_key_new();
			gtk_widget_add_controller( m_hWnd, ctl );
			g_signal_connect( ctl, "key-pressed", G_CALLBACK( GTKOnKeyPressed ), this );
			g_signal_connect( ctl, "key-released", G_CALLBACK( GTKOnKeyReleased ), this );
		}
		
		// Mouse
		{
			GtkEventController * ctl = gtk_event_controller_motion_new();
			gtk_widget_add_controller( m_hWnd, ctl );
			g_signal_connect( ctl, "motion", G_CALLBACK( GTKOnMouseMove ), this );
		}
		
		{
			GtkGesture * gesture = NewMouseClickGesture( m_hWnd, GDK_BUTTON_PRIMARY );
			g_signal_connect( gesture, "pressed", G_CALLBACK( GTKOnLMouseButtonDown ), this );
			g_signal_connect( gesture, "released", G_CALLBACK( GTKOnLMouseButtonUp ), this );
		}

		{
			GtkGesture * gesture = NewMouseClickGesture( m_hWnd, GDK_BUTTON_SECONDARY );
			g_signal_connect( gesture, "pressed", G_CALLBACK( GTKOnRMouseButtonDown ), this );
			g_signal_connect( gesture, "released", G_CALLBACK( GTKOnRMouseButtonUp ), this );
		}
		
		{
			GtkGesture * gesture = NewMouseClickGesture( m_hWnd, GDK_BUTTON_MIDDLE );
			g_signal_connect( gesture, "pressed", G_CALLBACK( GTKOnMMouseButtonDown ), this );
			g_signal_connect( gesture, "released", G_CALLBACK( GTKOnMMouseButtonUp ), this );
		}
		
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
	gboolean Application::GTKOnKeyPressed( GtkEventControllerKey * keyCtl, guint keyVal, guint keyCode, GdkModifierType state, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		const KeyCode appKeyCode = ConvertToKeyCode( keyVal );
		app->OnKeyDown( appKeyCode );
		
		if( keyVal==GDK_BUTTON_PRIMARY )
		{
			
			printf( "L Mouse Button\n" );
		}
		
		return true;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	gboolean Application::GTKOnKeyReleased( GtkEventControllerKey * keyCtl, guint keyVal, guint keyCode, GdkModifierType state, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		const KeyCode appKeyCode = ConvertToKeyCode( keyVal );
		app->OnKeyUp( appKeyCode );
		
		return true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnMouseMove( GtkEventControllerMotion * self, double x, double y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		MousePos pos { (int)x, (int)y };
		app->OnMouseMove( pos );
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnLMouseButtonUp( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyUp( KeyCode::kMouseLeftButton );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnLMouseButtonDown( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyDown( KeyCode::kMouseLeftButton );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnRMouseButtonUp( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyUp( KeyCode::kMouseRightButton );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnRMouseButtonDown( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyDown( KeyCode::kMouseRightButton );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnMMouseButtonUp( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyUp( KeyCode::kMouseMiddleButton );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Application::GTKOnMMouseButtonDown( GtkGestureClick * self, gint pressCount, gdouble x, gdouble y, gpointer userData )
	{
		Application * app = reinterpret_cast< Application * >( userData );
		app->OnKeyDown( KeyCode::kMouseMiddleButton );
	}
}
#endif
