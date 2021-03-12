#include "Camera.h"
#include "Math/MathUtils.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Camera::Camera( const Vec3 & pos, const Vec3 & lookAt, const float fov, const float ratio )
	: m_position{ pos }
	, m_lookAt{ lookAt }
	, m_fov{ fov }
	, m_aspectRatio{ ratio }
	{
		UpdateProjectionPlane( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Camera::SetPosition( const Vec3 & pos )
	{
		m_position = pos;
		UpdateProjectionPlane( );

	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Camera::SetFOV( const float fov )
	{
		m_fov = fov;
		UpdateProjectionPlane( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Ray Camera::GenerateRay( const float x, const float y ) const
	{
		// NOTE: revert y because the screen space Y is the inverse of the scene Y axis
		const Ray ray{ m_position, Normalize( m_ppBottomLeft + x * m_ppRightAxis + ( 1.0f - y ) * m_ppUpAxis - m_position ) };

		return ray;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Camera::UpdateProjectionPlane( )
	{
		const float focalDist	= tanf( DegToRad( m_fov ) / 2.0f );
		const float ppHeight	= 2.0f * focalDist;
		const float ppWidth		= m_aspectRatio * ppHeight;

		// TODO: take into account for aligned axis
		const Vec3 vDir			= Normalize( m_position - m_lookAt );
		const Vec3 vRight		= Normalize( Cross( Vec3( 0.0f, 1.0f, 0.0f ), vDir ) );
		const Vec3 vUp			= Cross( vDir, vRight );

		m_ppRightAxis			= vRight * ppWidth;
		m_ppUpAxis				= vUp * ppHeight;
		m_ppBottomLeft			= m_position - m_ppRightAxis / 2.0f - m_ppUpAxis / 2.0f - vDir;
	}
}