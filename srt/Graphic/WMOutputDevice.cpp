#include "WMOutputDevice.h"
#include <assert.h>
#include "Image.h"

namespace srt
{

	namespace
	{
		constexpr int kFontHeight = 10;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	WMOutputDevice::WMOutputDevice( WindowHandle hWnd, uint32_t width, uint32_t height )
	: OutputDevice( )
	, m_hWnd{ hWnd }
#if defined( SRT_PLATFORM_WINDOWS )
	, m_hDC{ nullptr }
	, m_hBitmap{ nullptr }
	, m_hOldBitmap{nullptr }
	, m_dcBits{ nullptr }
#endif
	{
#if defined( SRT_PLATFORM_WINDOWS )
		// Create DIB
		HDC hDC = GetDC( hWnd );
		m_hDC = CreateCompatibleDC( hDC );
		assert( m_hDC!=nullptr );
		ReleaseDC( hWnd, hDC );

		RECT rect;
		GetClientRect( hWnd, &rect );

		m_width		= ( rect.right - rect.left );
		m_height	= ( rect.bottom - rect.top );

		BITMAPINFO bmpInfos{};
		bmpInfos.bmiHeader.biSize			= sizeof( BITMAPINFOHEADER );
		bmpInfos.bmiHeader.biWidth			= m_width;
		bmpInfos.bmiHeader.biHeight			= -m_height;
		bmpInfos.bmiHeader.biPlanes			= 1;
		bmpInfos.bmiHeader.biBitCount		= 32;
		bmpInfos.bmiHeader.biCompression	= BI_RGB;

		m_hBitmap = CreateDIBSection( m_hDC, &bmpInfos, DIB_RGB_COLORS, &m_dcBits, nullptr, 0 );
		assert( m_hBitmap!=nullptr );

		m_hOldBitmap = SelectObject( m_hDC, m_hBitmap );

		// Font
		LOGFONT logFont;
		memset( &logFont, 0, sizeof( logFont ) );
		logFont.lfHeight	= -kFontHeight;
		logFont.lfWeight	= FW_NORMAL;
		logFont.lfCharSet	= ANSI_CHARSET;
		logFont.lfPitchAndFamily = FIXED_PITCH;
		wcscpy_s( logFont.lfFaceName, L"Consolas" );
		m_hFont = CreateFontIndirect( &logFont );

		m_hOldFont = SelectObject( m_hDC, m_hFont );
#endif

#if defined( SRT_PLATFORM_LINUX )
		m_width = width;
		m_height = height;

		// The drawing area will allow us know where and when we have to display something
		m_drawingArea = gtk_drawing_area_new( );
		gtk_drawing_area_set_draw_func( GTK_DRAWING_AREA( m_drawingArea ), OnAreaDraw, this, nullptr );
		gtk_window_set_child( GTK_WINDOW( m_hWnd ), m_drawingArea );

		// The cairo surface & render context will allow us to render into a temporary buffer
		// that will be copied into the drawing area surface when needed
		m_surface = cairo_image_surface_create( CAIRO_FORMAT_RGB24, m_width, m_height );
		m_cr = cairo_create( m_surface );
		
		// The pixBuf will be used to blit an Image into the cairo surface
		m_pixBuffer = gdk_pixbuf_new( GdkColorspace::GDK_COLORSPACE_RGB, false, 8, m_width, m_height );		
		
		// Font
		// Courier 10 Pitch
		m_pangoLayout = pango_cairo_create_layout( m_cr );
		
		PangoFontDescription * fontDesc = pango_font_description_from_string( "Courier 10" );
		pango_layout_set_font_description( m_pangoLayout, fontDesc );
		pango_font_description_free( fontDesc );
#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	WMOutputDevice::~WMOutputDevice( )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		SelectObject( m_hDC, m_hOldFont );
		SelectObject( m_hDC, m_hOldBitmap );
		DeleteObject( m_hFont );
		DeleteObject( m_hBitmap );
		DeleteDC( m_hDC );
#endif

#if defined( SRT_PLATFORM_LINUX )
		g_object_unref( m_pangoLayout );
		g_object_unref( m_pixBuffer );
		cairo_destroy( m_cr );
		cairo_surface_destroy( m_surface );
#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	int WMOutputDevice::GetFontHeight( ) const
	{
		return kFontHeight;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
#if defined( SRT_PLATFORM_LINUX )
	void WMOutputDevice::OnAreaDraw( GtkDrawingArea * area, cairo_t * cr, int width, int height, gpointer userData )
	{
		WMOutputDevice * dev = reinterpret_cast< WMOutputDevice * >( userData );
		
		cairo_set_source_surface( cr, dev->m_surface, 0, 0 );
		cairo_paint( cr );
	}
#endif
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void WMOutputDevice::Present( )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		HDC wndDC = GetDC( m_hWnd );

		SIZE size;
		GetBitmapDimensionEx( m_hBitmap, &size );

		BitBlt( wndDC, 0, 0, m_width, m_height, m_hDC, 0, 0, SRCCOPY );

		ReleaseDC( m_hWnd, wndDC );
#endif

#if defined( SRT_PLATFORM_LINUX )
		gtk_widget_queue_draw( m_drawingArea );
#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void WMOutputDevice::BlitImage( const Image & image )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		const PixelSurface::Desc & surfDesc = image.GetMipDesc( 0 );
		memcpy( m_dcBits, image.GetMipSurface( 0 ), (size_t)surfDesc.pitch * (size_t)surfDesc.height );
#endif

#if defined( SRT_PLATFORM_LINUX )
		const PixelSurface::Desc & surfDesc = image.GetMipDesc( 0 );
		assert( surfDesc.width == m_width );
		assert( surfDesc.height == m_height );
		
		for( uint32_t y = 0; y < m_height; ++y )
		{
			const uint8_t * srcSurf = reinterpret_cast< const uint8_t * >( image.GetMipSurface( 0 ) ) + y * surfDesc.pitch;
			guchar * dstSurf = gdk_pixbuf_get_pixels( m_pixBuffer ) + y * gdk_pixbuf_get_rowstride( m_pixBuffer );
			
			for( uint32_t x = 0; x < m_width; ++x )
			{
				dstSurf[ 0 ] = srcSurf[ 2 ];
				dstSurf[ 1 ] = srcSurf[ 1 ];
				dstSurf[ 2 ] = srcSurf[ 0 ];
				dstSurf += 3;
				srcSurf += 4;
			}
		}
		gdk_cairo_set_source_pixbuf( m_cr, m_pixBuffer, 0, 0 );
		cairo_paint( m_cr );
#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void WMOutputDevice::SetTextColorImpl( uint8_t r, uint8_t g, uint8_t b )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		COLORREF color = RGB( r, g, b );
		::SetTextColor( m_hDC, color );
#endif
#if defined( SRT_PLATFORM_LINUX )
		cairo_set_source_rgb( m_cr, (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
#endif
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void WMOutputDevice::OutputTextImpl( int x, int y, const char * text )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		const int strLen = static_cast< int >( strlen( text ) );

		SetBkMode( m_hDC, TRANSPARENT );
		TextOutA( m_hDC, x, y, text, strLen );
#endif

#if defined( SRT_PLATFORM_LINUX )
		cairo_move_to( m_cr, (double)x, (double)y );
		pango_layout_set_text( m_pangoLayout, text, -1 );
		pango_cairo_show_layout( m_cr, m_pangoLayout );
#endif
	}
}


