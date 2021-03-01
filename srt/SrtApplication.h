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

	void OnKeyUp( KeyCode key ) final;
	void OnKeyDown( KeyCode key ) final;

	void OnMouseMove( const MousePos & pos ) final;

	SrtApplication( const SrtApplication & other ) = delete;


	Scene *				m_scene { nullptr };
	Image *				m_backBuffer { nullptr };
	OutputDevice *		m_outputDev { nullptr };
	MousePos			m_mousePos;

	bool				m_mouseLeft { false };
	bool				m_mouseRight { false };
	bool				m_isPaused { false };
};

}

#endif
