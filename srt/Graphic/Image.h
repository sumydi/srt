#ifndef SRT_IMAGE_H
#define SRT_IMAGE_H

#include "Types.h"

namespace srt
{


	//
	class Image
	{
	public:

		Image( uint32_t width, uint32_t height, PixelFormat pf, uint32_t mipCount = 1, void * surface = nullptr );
		~Image();

		Image & operator = ( const Image & other );

		PixelFormat GetPixelFormat( ) const { return m_pixelFormat; }
		uint32_t GetBPP( ) const { return m_bpp; }
		uint32_t GetMipCount( ) const { return m_mipCount; }
		const PixelSurface::Desc & GetMipDesc( uint32_t mipIdx ) const;
		void * GetMipSurface( uint32_t mipIdx );
		static uint32_t GetPixelFormatBPP( PixelFormat pf );

	protected:
		PixelFormat		m_pixelFormat;
		uint32_t		m_bpp;
		uint32_t		m_mipCount;
		PixelSurface *	m_mips;
		void *			m_surface;
	};

}
#endif
