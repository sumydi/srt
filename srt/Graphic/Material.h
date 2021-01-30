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
		~Material( ) = default;

		const Vec3 &	GetDiffuse( ) const { return m_diffuse; }
		void			SetDiffuse( const Vec3 & diffuse ) { m_diffuse = diffuse; }

		float			GetRoughness( ) const { return m_roughness; }
		void			SetRoughness( const float roughness ) { m_roughness = roughness; }

		float			GetMetalness( ) const { return m_metalness; }
		void			SetMetalness( const float metalness ) { m_metalness = metalness; }
	
	private:

		Vec3	m_diffuse;
		float	m_roughness { 0.5f };
		float	m_metalness { 0.0f };
	
	};

}

#endif
