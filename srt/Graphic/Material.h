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

		Material( const char * name ) 
		: m_name{ name }
		{
		
		}

		~Material( ) = default;

		const char *	GetName( ) const { return m_name.c_str(); }

		const Vec3 &	GetAlbedo( ) const { return m_albedo; }
		Material &		SetAlbedo( const Vec3 & albedo ) { m_albedo = albedo; return *this; }

		float			GetRoughness( ) const { return m_roughness; }
		Material &		SetRoughness( const float roughness ) { m_roughness = std::max( 0.01f, std::min( 1.0f, roughness ) ); return *this; }

		float			GetMetalness( ) const { return m_metalness; }
		Material &		SetMetalness( const float metalness ) { m_metalness = std::max( 0.0f, std::min( 1.0f, metalness ) ); return *this; }

		float			GetIOR( ) const { return m_ior; }
		Material		SetIOR( const float ior ) { m_ior = ior; return *this; }
	
	private:
		std::string		m_name;
		Vec3			m_albedo { 0.8f, 0.8f, 0.8f };
		float			m_roughness { 0.5f };
		float			m_metalness { 0.0f };
		float			m_ior { 1.0f };
	
	};

}

#endif
