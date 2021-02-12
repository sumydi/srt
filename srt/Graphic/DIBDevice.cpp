#include "DIBDevice.h"
#include <assert.h>
#include "Image.h"

#if defined( SRT_PLATFORM_WINDOWS )

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DIBDevice::DIBDevice( WindowHandle hWnd )
	: OutputDevice( )
	, m_hWnd{ hWnd }
	, m_hDC{ nullptr }
	, m_hBitmap{ nullptr }
	, m_hOldBitmap{nullptr }
	, m_dcBits{ nullptr }
	{
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
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DIBDevice::~DIBDevice( )
	{
		SelectObject( m_hDC, m_hOldBitmap );
		DeleteObject( m_hBitmap );
		DeleteDC( m_hDC );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void DIBDevice::Present( )
	{
		HDC wndDC = GetDC( m_hWnd );

		SIZE size;
		GetBitmapDimensionEx( m_hBitmap, &size );

		BitBlt( wndDC, 0, 0, m_width, m_height, m_hDC, 0, 0, SRCCOPY );

		ReleaseDC( m_hWnd, wndDC );

	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void DIBDevice::BlitImage( const Image & image )
	{
		const PixelSurface::Desc & surfDesc = image.GetMipDesc( 0 );
		memcpy( m_dcBits, image.GetMipSurface( 0 ), (size_t)surfDesc.pitch * (size_t)surfDesc.height );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void DIBDevice::OutputTextImpl( int x, int y, const char * text )
	{
		const int strLen = static_cast< int >( strlen( text ) );

		SetBkMode( m_hDC, TRANSPARENT );
		TextOutA( m_hDC, x, y, text, strLen );
	}
}

#endif // SRT_PLATFORM_WINDOWS
