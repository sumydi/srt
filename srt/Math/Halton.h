#ifndef SRT_HALTON_H
#define SRT_HALTON_H

#include <cstdint>
#include <vector>
#include "Vector2.h"

namespace srt
{

	// ============================================================================
	// Class used to generate and use an Halton sequence
	// Ref: https://en.wikipedia.org/wiki/Halton_sequence
	// 
	// You must provide a size of the Halton sequence in the ctor. The distribution
	// is based on this size, so it makes no sense to use only a part of a sequence.
	//
	// ============================================================================
	class Halton final
	{
	public:
		Halton( uint32_t size );

		const Vec2 & GetValue( uint32_t index ) const
		{
			return m_values[ index ];
		}

	private:
		using Values = std::vector< Vec2 >;
		Values		m_values;
	};
}

#endif
