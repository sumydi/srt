#include "DIBDevice.h"
#include <assert.h>
#include "Image.h"

#if defined( SRT_PLATFORM_WINDOWS )

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DIBDevice::DIBDevice( HWND hWnd )
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

		BITMAPINFO bmpInfos{};
		bmpInfos.bmiHeader.biSize			= sizeof( BITMAPINFOHEADER );
		bmpInfos.bmiHeader.biWidth			= ( rect.right - rect.left );
		bmpInfos.bmiHeader.biHeight			= -( rect.bottom - rect.top );
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
	void DIBDevice::Present( const Image & image )
	{
		HDC wndDC = GetDC( m_hWnd );

		const PixelSurface::Desc & surfDesc = image.GetMipDesc( 0 );

		memcpy( m_dcBits, image.GetMipSurface( 0 ), (size_t)surfDesc.pitch * (size_t)surfDesc.height );
		BitBlt( wndDC, 0, 0, surfDesc.width, surfDesc.height, m_hDC, 0, 0, SRCCOPY );

		ReleaseDC( m_hWnd, wndDC );
	}

}

#endif // SRT_PLATFORM_WINDOWS
