#ifndef SRT_OUTPUTDEVICE_H
#define SRT_OUTPUTDEVICE_H

#include "Base.h"

namespace srt
{
	class Image;

	// ============================================================================
	//
	// Base class to save or render an image.
	//
	// ============================================================================
	class OutputDevice
	{
	public:
		OutputDevice() = default;
		virtual ~OutputDevice() { };

		void			OutputText( int x, int y, const char * text, ...  );
		void			SetTextPosition( int x, int y );
		void			PushText( const char * text, ... );

		virtual int		GetFontHeight( ) const = 0;

		virtual void	BlitImage( const Image & image ) = 0;
		virtual void	Present( ) = 0;
		

	private:
		OutputDevice( const OutputDevice & other ) = delete;
		OutputDevice & operator = ( const OutputDevice & other ) = delete;

		virtual void OutputTextImpl( int x, int y, const char * text  ) = 0;

		int		m_textX { 0 };
		int		m_textY { 0 };
	};

}

#endif
