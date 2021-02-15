#ifndef SRT_CAMERA_H
#define SRT_CAMERA_H

#include "Math/Ray.h"

namespace srt
{
	// ============================================================================
	//
	//	A camera
	//
	// ============================================================================
	class Camera
	{
	public:
		Camera( ) = default;

		//
		// pos		: position
		// dir		: direction
		// fov		: field of view in degree
		// ratio	: aspect ratio
		//
		Camera( const Vec3 & pos, const Vec3 & dir, const float fov, const float ratio );

		~Camera( ) = default;

		const Vec3 &	GetPosition( ) const { return m_position; }
		float			GetFOV( ) const { return m_fov; }
		void			SetFOV( const float fov);

		Ray				GenerateRay( const float x, const float y );

	private:
		void	UpdateFocalDistance( );
		Vec3	m_position;
		Vec3	m_direction;
		float	m_fov { 90.0f };
		float	m_focalDistance { 1.0f };
		float	m_aspectRation { 1.0f };
	
	};

}

#endif
