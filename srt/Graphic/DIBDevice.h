#ifndef SRT_DIBDEVICE_H
#define SRT_DIBDEVICE_H

#include "OutputDevice.h"

#if defined( SRT_PLATFORM_WINDOWS )
namespace srt
{

	class DIBDevice final : public OutputDevice
	{
	public:
		DIBDevice( HWND hWnd );
		~DIBDevice( ) final;

		void Present( const Image & image ) final;

	private:
		HWND	m_hWnd;
		HDC		m_hDC;
		HBITMAP	m_hBitmap;
		HGDIOBJ	m_hOldBitmap;
		void *	m_dcBits;
	};

}

#endif // SRT_PLATFORM_WINDOWS

#endif