#ifndef SRT_SRTAPPPLICATION_H
#define SRT_SRTAPPPLICATION_H

#include "Application/Application.h"
#include "Scene/SceneTraceResult.h"
#include "Memory/FreeAllAllocator.h"

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

	FreeAllAllocator	m_freeAllAllocator;

	uint32_t			m_sampleCount { 1 };
	bool				m_isPaused { false };

	SceneTraceResult	m_pickResult;

	enum class RenderMode
	{
		kSimple = 0,
		kPBR,
		kFullRT
	};

	RenderMode			m_renderMode { RenderMode::kPBR };
};

}

#endif
