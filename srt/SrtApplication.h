#ifndef SRT_SRTAPPPLICATION_H
#define SRT_SRTAPPPLICATION_H

#include "Application/Application.h"
#include <chrono>

namespace srt
{

class Image;
class OutputDevice;
class Scene;
class Ray;

// ============================================================================
//
//
//
// ============================================================================
class SrtApplication final : public Application
{
public:
	SrtApplication( const AppContext & context);
	~SrtApplication( );

private:

	void FrameStart( ) final;
	void FrameUpdate( const float dt ) final;
	void FrameEnd( const float frameDuration ) final;

	void OnKeyUp( char key );
	void OnKeyDown( char key );

	SrtApplication( const SrtApplication & other ) = delete;


	Scene *			m_scene;
	Image *			m_backBuffer;
	OutputDevice *	m_outputDev;
};

}

#endif
