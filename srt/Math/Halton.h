#ifndef SRT_HALTON_H
#define SRT_HALTON_H

#include <cstdint>
#include <vector>
#include "Vector2.h"

namespace srt
{
	float GenerateHalton( uint32_t index, uint32_t base );

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
