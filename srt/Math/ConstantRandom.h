#ifndef SRT_CONSTANTRANDOM_H
#define SRT_CONSTANTRANDOM_H


namespace srt
{
	// ============================================================================
	//
	// Provides constant random values at time t.
	//
	// ============================================================================
	class ConstantRandom
	{
	public:
		static const ConstantRandom &	GetInstance();
		float							GetValue( size_t t ) const { return m_values[ t ]; }
	
	private:
		ConstantRandom();
		~ConstantRandom() = default;

		static constexpr size_t kValueCount = 256;
		float m_values[ kValueCount ];
	
	};

}

#endif
