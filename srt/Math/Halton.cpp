#include "Halton.h"

namespace srt
{

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	static float GenerateHalton( uint32_t index, uint32_t base )
	{
		float result = 0.0f;
		float invBase = 1.0f / (float)base;
		float fraction = invBase;

		while( index > 0 )
		{
			result += ( index % base ) * fraction;
			index /= base;
			fraction *= invBase;
		}
		return result;
	}

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	Halton::Halton( uint32_t size )
	{
		m_values.resize( size );
		for( uint32_t i = 0; i < size; ++i )
		{
			m_values[ i ] = Vec2( GenerateHalton( i + 1, 2 ), GenerateHalton( i + 1, 3 ) ) - Vec2( 0.5f, 0.5f );
		}
	}
}