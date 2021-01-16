#ifndef SRTAPPPLICATION_H
#define SRTAPPPLICATION_H

#include "Base.h"

namespace srt
{

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


private:
	Application( const Application & other ) = delete;
	Application operator = ( const Application & other ) = delete;

#if defined( SRT_PLATFORM_WINDOWS )
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
#endif

};

}

#endif
