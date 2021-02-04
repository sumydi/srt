#ifndef SRT_MATERIAL_H
#define SRT_MATERIAL_H

#include "Math/Vector3.h"

namespace srt
{
	// ============================================================================
	//
	// A material has:
	//	- a siffuse color
	//	- roughness
	//	- metalness
	//
	// ============================================================================
	class Material
	{
	public:
		Material( ) = default;
		Material( const Vec3 & diffuse, const float roughness, const float metalness ) 
		: m_albedo{ diffuse }
		, m_roughness{ roughness }
		, m_metalness{ metalness }
		{
		
		}

		~Material( ) = default;

		const Vec3 &	GetAlbedo( ) const { return m_albedo; }
		void			SetAlbedo( const Vec3 & albedo ) { m_albedo = albedo; }

		float			GetRoughness( ) const { return m_roughness; }
		void			SetRoughness( const float roughness ) { m_roughness = roughness; }

		float			GetMetalness( ) const { return m_metalness; }
		void			SetMetalness( const float metalness ) { m_metalness = metalness; }
	
	private:

		Vec3	m_albedo;
		float	m_roughness { 0.5f };
		float	m_metalness { 0.0f };
	
	};

}

#endif
