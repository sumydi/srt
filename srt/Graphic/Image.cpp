#include "Image.h"
#include <algorithm>
#include <assert.h>

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Image::Image( uint32_t width, uint32_t height, PixelFormat pf, uint32_t mipCount, void * surface )
	: m_pixelFormat( pf )
	{
		// find the greatest mip count to either:
		//	- validate mipCount if > 0
		//	- find mipCount if == 0
		uint32_t curSize = std::min( width, height );
		uint32_t greatestMipCount = 0;
		while( curSize > 0 )
		{
			++greatestMipCount;
			curSize >>= 1;
		}

		if( mipCount==0 )
		{
			m_mipCount = mipCount;
		}
		else
		{
			m_mipCount = std::min( mipCount, greatestMipCount );
		}

		// allocate surface
		m_bpp = GetPixelFormatBPP( pf );
		size_t totalSurfaceSize = 0;
		{
			uint32_t mipWidth = width;
			uint32_t mipHeight = height;
			for( uint32_t mipIdx = 0; mipIdx < mipCount; ++mipIdx )
			{
				totalSurfaceSize += static_cast< size_t >( ( ( mipWidth * m_bpp ) / 8 ) * mipHeight );
				mipWidth >>= 1;
				mipHeight >>= 1;
			}
		}

		m_surface = malloc( totalSurfaceSize );
		if( surface )
		{
			memcpy( m_surface, surface, totalSurfaceSize );
		}
		
		// allocate & fill mip infos
		uint8_t * mipSurface = reinterpret_cast< uint8_t * >( m_surface );
		m_mips = new PixelSurface [ m_mipCount ];
		for( uint32_t mipIdx = 0; mipIdx < mipCount; ++mipIdx )
		{
			m_mips[ mipIdx ].width = (uint16_t)width;
			m_mips[ mipIdx ].height = (uint16_t)height;
			m_mips[ mipIdx ].pitch = ( width * m_bpp ) / 8;
			m_mips[ mipIdx ].surface = reinterpret_cast< void * >( mipSurface );
			mipSurface  += m_mips[ mipIdx ].pitch * height;
			width >>= 1;
			height >>= 1;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Image::~Image( )
	{
		delete [] m_mips;
		free( m_surface );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	const PixelSurface & Image::GetMip( uint32_t mipIdx ) const
	{
		assert( mipIdx < m_mipCount );
		return m_mips[ mipIdx ];
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	uint32_t Image::GetPixelFormatBPP( PixelFormat pf )
	{
		uint32_t bpp = 0;
		switch( pf )
		{
			case PixelFormat::kX8R8G8B8_UInt:
				bpp = 32;
				break;

			case PixelFormat::kA8R8G8B8_UInt:
				bpp = 32;
				break;

			default:
				bpp = 0;
		}
		return bpp;
	}

}
