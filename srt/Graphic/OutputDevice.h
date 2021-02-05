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

		virtual void BlitImage( const Image & image ) = 0;

	private:
		OutputDevice( const OutputDevice & other ) = delete;
		OutputDevice & operator = ( const OutputDevice & other ) = delete;
	};

}

#endif
