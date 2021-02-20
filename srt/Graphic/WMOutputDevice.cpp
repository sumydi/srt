#include "WMOutputDevice.h"
#include <assert.h>
#include "Image.h"

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	WMOutputDevice::WMOutputDevice( WindowHandle hWnd )
	: OutputDevice( )
	, m_hWnd{ hWnd }
	, m_hDC{ nullptr }
#if defined( SRT_PLATFORM_WINDOWS )
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
		logFont.lfHeight	= -12;
		logFont.lfWeight	= FW_NORMAL;
		logFont.lfCharSet	= ANSI_CHARSET;
		logFont.lfPitchAndFamily = FIXED_PITCH;
		wcscpy_s( logFont.lfFaceName, L"Consolas" );
		m_hFont = CreateFontIndirect( &logFont );

		m_hOldFont = SelectObject( m_hDC, m_hFont );
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
	}

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
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void WMOutputDevice::BlitImage( const Image & image )
	{
#if defined( SRT_PLATFORM_WINDOWS )
		const PixelSurface::Desc & surfDesc = image.GetMipDesc( 0 );
		memcpy( m_dcBits, image.GetMipSurface( 0 ), (size_t)surfDesc.pitch * (size_t)surfDesc.height );
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
	}
}

