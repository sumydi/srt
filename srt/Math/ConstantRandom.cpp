#include "ConstantRandom.h"
#include "Random.h"

namespace srt
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ConstantRandom::ConstantRandom( )
	{
		m_values[ 0 ] = 0.0f;
		for( size_t valIdx = 1; valIdx < kValueCount; ++valIdx )
		{
			m_values[ valIdx ] = RandomFloat();
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	const ConstantRandom & ConstantRandom::GetInstance()
	{
		static ConstantRandom	g_instance;
		return g_instance;
	}
}
