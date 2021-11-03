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
		// pos		: camera position
		// lookAt	: position the camera is looking at
		// fov		: field of view in degree
		// ratio	: aspect ratio
		//
		Camera( const Vec3 & pos, const Vec3 & lookAt, const float fov, const float ratio );

		~Camera( ) = default;

		const Vec3 &	GetPosition( ) const { return m_position; }
		void			SetPosition( const Vec3 & pos );

		const Vec3 &	GetLookAt( ) const { return m_lookAt; }
		void			SetLookAt( const Vec3 & lookAt );

		float			GetFOV( ) const { return m_fov; }
		void			SetFOV( const float fov);

		Ray				GenerateRay( const float x, const float y ) const;

	private:
		void	UpdateProjectionPlane( );

		Vec3	m_position;
		Vec3	m_lookAt;
		float	m_fov { 90.0f };
		float	m_aspectRatio { 1.0f };

		// Projection plane... must be replaced later by matrix
		Vec3	m_ppRightAxis;
		Vec3	m_ppUpAxis;
		Vec3	m_ppBottomLeft;
	
	};

}

#endif
