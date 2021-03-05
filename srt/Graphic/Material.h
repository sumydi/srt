#ifndef SRT_MATERIAL_H
#define SRT_MATERIAL_H

#include "Math/Vector3.h"
#include <string>

namespace srt
{
	// ============================================================================
	//
	// A material has:
	//	- an albedo color
	//	- roughness
	//	- metalness
	//
	//
	// ============================================================================
	class Material
	{
	public:
		Material( ) = default;

		Material( const char * name, const Vec3 & diffuse, const float roughness, const float metalness ) 
		: m_name{ name }
		, m_albedo{ diffuse }
		, m_roughness{ roughness }
		, m_metalness{ metalness }
		{
		
		}

		~Material( ) = default;

		const char *	GetName( ) const { return m_name.c_str(); }

		const Vec3 &	GetAlbedo( ) const { return m_albedo; }
		void			SetAlbedo( const Vec3 & albedo ) { m_albedo = albedo; }

		float			GetRoughness( ) const { return m_roughness; }
		void			SetRoughness( const float roughness ) { m_roughness = std::max( 0.02f, std::min( 1.0f, roughness ) ); }

		float			GetMetalness( ) const { return m_metalness; }
		void			SetMetalness( const float metalness ) { m_metalness = std::max( 0.0f, std::min( 1.0f, metalness ) ); }
	
	private:
		std::string		m_name;
		Vec3			m_albedo;
		float			m_roughness { 0.5f };
		float			m_metalness { 0.0f };
	
	};

}

#endif
