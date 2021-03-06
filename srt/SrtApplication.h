#ifndef SRT_SRTAPPPLICATION_H
#define SRT_SRTAPPPLICATION_H

#include "Application/Application.h"
#include "Scene/SceneTraceResult.h"

#include <chrono>

namespace srt
{

class Image;
class OutputDevice;
class Scene;
class Ray;
class JobScheduler;

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

	void UpdateEditMode( );

	void FrameStart( ) final;
	void FrameUpdate( const float dt ) final;
	void FrameEnd( const float frameDuration ) final;

	SrtApplication( const SrtApplication & other ) = delete;

	JobScheduler *		m_jobScheduler { nullptr };
	 
	Scene *				m_scene { nullptr };
	Image *				m_backBuffer { nullptr };
	OutputDevice *		m_outputDev { nullptr };

	bool				m_isPaused { false };

	SceneTraceResult	m_pickResult;
};

}

#endif
