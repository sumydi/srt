#include "Image.h"
#include <algorithm>
#include <assert.h>
#include "Graphic/Color.h"
#include "Math/Vector4.h"

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
			m_mipCount = greatestMipCount;
		}
		else
		{
			m_mipCount = std::min( mipCount, greatestMipCount );
		}

		// allocate surface
		m_bpp = GetPixelFormatBPP( pf );
		size_t totalSurfaceSize = 0;
		{
			size_t mipWidth = width;
			size_t mipHeight = height;
			for( uint32_t mipIdx = 0; mipIdx < mipCount; ++mipIdx )
			{
				totalSurfaceSize += ( ( mipWidth * static_cast< size_t >( m_bpp ) ) / 8 ) * mipHeight;
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
		for( uint32_t mipIdx = 0; mipIdx < m_mipCount; ++mipIdx )
		{
			m_mips[ mipIdx ].desc.width = (uint16_t)width;
			m_mips[ mipIdx ].desc.height = (uint16_t)height;
			m_mips[ mipIdx ].desc.pitch = ( width * m_bpp ) / 8;
			m_mips[ mipIdx ].surface = reinterpret_cast< void * >( mipSurface );
			mipSurface  += (size_t)m_mips[ mipIdx ].desc.pitch * (size_t)height;
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
	const PixelSurface::Desc & Image::GetMipDesc( uint32_t mipIdx ) const
	{
		assert( mipIdx < m_mipCount );
		return m_mips[ mipIdx ].desc;
	}
	
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	const void * Image::GetMipSurface( uint32_t mipIdx ) const
	{
		assert( mipIdx < m_mipCount );
		return m_mips[ mipIdx ].surface;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void * Image::LockMipSurface( uint32_t mipIdx ) const
	{
		assert( mipIdx < m_mipCount );
		return m_mips[ mipIdx ].surface;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Image::UnlockMipSurface( uint32_t mipIdx ) const
	{
		assert( mipIdx < m_mipCount );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Image::ClearMipSurface( uint32_t mipIdx, const Vec4 & color )
	{
		if( mipIdx < m_mipCount )
		{
			// Note: currently manage only 32 bits pixel formats
			const PixelSurface::Desc & mipDesc	= GetMipDesc( mipIdx );
			uint32_t * mipSurface				= reinterpret_cast< uint32_t * >( LockMipSurface( mipIdx ) );
			const uint32_t finalColor			= MakeRGBA( color );
			
			for( uint32_t y = 0; y < mipDesc.height; ++y )
			{
				uint32_t * line = mipSurface + ( y * mipDesc.pitch / 4 );
				for( uint32_t x = 0; x < mipDesc.width; ++x )
				{
					line[ x ] = finalColor;
				}
			}

			UnlockMipSurface( mipIdx );
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	uint32_t Image::GetPixelFormatBPP( PixelFormat pf )
	{
		uint32_t bpp = 0;
		switch( pf )
		{
			case PixelFormat::kBGRX8_UInt:
				bpp = 32;
				break;

			case PixelFormat::kBGRA8_UInt:
				bpp = 32;
				break;

			default:
				bpp = 0;
		}
		return bpp;
	}

}
