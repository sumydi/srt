#ifndef SRT_WMOUTUPDEVICE_H
#define SRT_WMOUTUPDEVICE_H

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
		WMOutputDevice( WindowHandle hWnd );
		~WMOutputDevice( ) final;

		void BlitImage( const Image & image ) final;
		void Present( ) final;

	private:
		virtual void OutputTextImpl( int x, int y, const char * text  ) final;

		int32_t			m_width;
		int32_t			m_height;
		WindowHandle	m_hWnd;

#if defined( SRT_PLATFORM_WINDOWS)
		HDC		m_hDC;
		HBITMAP	m_hBitmap;
		HGDIOBJ	m_hOldBitmap;
		void *	m_dcBits;
#endif
	};

}

#endif

