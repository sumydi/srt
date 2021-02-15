#include "Camera.h"
#include "Math/MathUtils.h"

namespace srt
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Camera::Camera( const Vec3 & pos, const Vec3 & dir, const float fov, const float ratio )
	: m_position{ pos }
	, m_direction{ dir }
	, m_fov{ fov }
	, m_aspectRation{ ratio }
	{
		UpdateFocalDistance( );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	Ray Camera::GenerateRay( const float x, const float y )
	{
		// TODO: take into account direction
		const Ray ray{ m_position, Normalize( Vec3{ x * 2.0f * m_aspectRation - m_aspectRation, -( y * 2.0f - 1.0f ), -m_focalDistance } ) };

		return ray;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Camera::UpdateFocalDistance( )
	{
		m_focalDistance = tanf( DegToRad( m_fov / 2.0f ) );
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void Camera::SetFOV( const float fov )
	{
		m_fov = fov;
		UpdateFocalDistance( );
	}
}