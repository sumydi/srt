#ifndef SRTAPPPLICATION_H
#define SRTAPPPLICATION_H

#include "Base.h"

namespace srt
{

class Image;
class OutputDevice;
class Scene;
class Ray;

struct AppContext
{
	uint32_t	width;
	uint32_t	height;
#if defined( SRT_PLATFORM_WINDOWS )
	HINSTANCE	hInstance;
#endif
};

// ============================================================================
//
//
//
// ============================================================================
class Application
{
public:
	Application( const AppContext & context);
	~Application( );

	void Run( );
	void Update( float dt );

private:
	Application( const Application & other ) = delete;
	Application operator = ( const Application & other ) = delete;

#if defined( SRT_PLATFORM_WINDOWS )
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
#endif

	Scene *			m_scene;
	Image *			m_backBuffer;
	OutputDevice *	m_outputDev;

};

}

#endif
