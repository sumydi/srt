#ifndef SRT_LIGHT_H
#define SRT_LIGHT_H

#include "Math/Vector3.h"

namespace srt
{

	class Material;

	// ============================================================================
	//
	//	A light
	//
	// ============================================================================
	class Light
	{
	public:
		Light() = default;
		explicit Light( const Vec3 & pos, const Vec3 & color );
		~Light() = default;

		const Vec3 &	GetPosition( ) const { return m_position; }
		void			SetPosition( const Vec3 & pos ) { m_position = pos; }

		const Vec3 &	GetColor( ) const { return m_color; }

	private:
		Vec3	m_position{ 0.0f, 0.0f, 0.0f };
		Vec3	m_color{ 1.0f, 1.0f, 1.0f };
	
	};

}

#endif

