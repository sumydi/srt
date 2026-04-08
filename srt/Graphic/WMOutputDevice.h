#ifndef SRT_WMOUTPUPDEVICE_H
#define SRT_WMOUTPUPDEVICE_H

#include "OutputDevice.h"

namespace srt
{
	// ============================================================================
	//
	//	Window Manager default output device. It's system specific.
	//	It's a basic output to screen using current platform window manager.
	//
	// ============================================================================
	class WMOutputDevice final : public OutputDevice
	{
	public:
		WMOutputDevice( WindowHandle hWnd, uint32_t width = 0, uint32_t height = 0 );
		~WMOutputDevice( ) final;

		int		GetFontHeight( ) const final;

		void	BlitImage( const Image & image ) final;
		void	Present( ) final;

	private:
		void	SetTextColorImpl( uint8_t r, uint8_t g, uint8_t b ) final;
		void	OutputTextImpl( int x, int y, const char * text  ) final;
		
		uint32_t		m_width;
		uint32_t		m_height;
		WindowHandle	m_hWnd;

#if defined( SRT_PLATFORM_WINDOWS )
		HDC				m_hDC;
		HBITMAP			m_hBitmap;
		HFONT			m_hFont;
		HGDIOBJ			m_hOldBitmap;
		HGDIOBJ			m_hOldFont;
		void *			m_dcBits;
#endif

#if defined( SRT_PLATFORM_LINUX )
		static void OnAreaDraw( GtkDrawingArea * area, cairo_t * cr, int width, int height, gpointer userData );
		
		GtkWidget *			m_drawingArea { nullptr };
		cairo_surface_t *	m_surface { nullptr };
		cairo_t *			m_cr { nullptr };
		GdkPixbuf *			m_pixBuffer { nullptr };
		
#endif
	};

}

#endif

