#ifndef SRT_LIGHT_H
#define SRT_LIGHT_H

#include "Math/Vector3.h"

namespace srt
{

	// ============================================================================
	//
	//	A light
	//
	// ============================================================================
	class Light
	{
	public:
		Light() = default;
		~Light() = default;

		Vec3 ComputeLighting( const Vec3 & pos, const Vec3 & normal );

	private:
		Vec3	m_position{ 0.0f, 0.0f, 0.0f };
		Vec3	m_color{ 1.0f, 1.0f, 1.0f };
	
	};

}

#endif

