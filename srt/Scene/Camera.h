#ifndef SRT_CAMERA_H
#define SRT_CAMERA_H

#include "Math/Vector3.h"

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
		Camera( const Vec3 & pos ) : m_position{ pos } { }; 
		~Camera( ) = default;

		const Vec3 &	GetPosition( ) const { return m_position; }

	private:
		Vec3	m_position;
	
	};

}

#endif
