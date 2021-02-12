#ifndef SRT_SRTAPPPLICATION_H
#define SRT_SRTAPPPLICATION_H

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
class SrtApplication
{
public:
	SrtApplication( const AppContext & context);
	~SrtApplication( );

	void Run( );
	void Update( float dt );

	void OnKeyUp( char key );
	void OnKeyDown( char key );

private:
	SrtApplication( const SrtApplication & other ) = delete;
	SrtApplication operator = ( const SrtApplication & other ) = delete;

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
