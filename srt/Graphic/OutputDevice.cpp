#include "OutputDevice.h"
#include <stdio.h>
#include <stdarg.h>

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void OutputDevice::OutputText( int x, int y, const char * text, ... )
	{
		constexpr size_t kBufferSize = 4 * 1024;
		char buffer[ kBufferSize ];

		va_list arg;
		va_start( arg, text );
		vsnprintf( buffer, kBufferSize - 1, text, arg );
		va_end( arg );

		OutputTextImpl( x, y, buffer );

	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void OutputDevice::SetTextPosition( int x, int y )
	{
		m_textX = x;
		m_textY = y;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void OutputDevice::PushText( const char * text, ... )
	{
		constexpr size_t kBufferSize = 4 * 1024;
		char buffer[ kBufferSize ];

		va_list arg;
		va_start( arg, text );
		vsnprintf( buffer, kBufferSize - 1, text, arg );
		va_end( arg );

		OutputTextImpl( m_textX, m_textY, buffer );

		m_textY += this->GetFontHeight();
	}

}


