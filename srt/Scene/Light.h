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
		enum class Type
		{
			kOmni = 0,
			kDirectionnal
		};

		Light() = default;
		explicit Light( Type type, const Vec3 & pos, const Vec3 & dir, const Vec3 & color );
		~Light() = default;

		Type			GetType() const { return m_type; }

		const Vec3 &	GetPosition( ) const { return m_position; }
		void			SetPosition( const Vec3 & pos ) { m_position = pos; }

		const Vec3 &	GetDirection( ) const { return m_direction; }

		const Vec3 &	GetColor( ) const { return m_color; }

	private:
		Type		m_type{ Type::kOmni };
		Vec3		m_position{ 0.0f, 0.0f, 0.0f };
		Vec3		m_direction{ 0.0f, 0.0f, 1.0f };
		Vec3		m_color{ 1.0f, 1.0f, 1.0f };
	
	};

}

#endif

