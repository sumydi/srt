// srt.cpp : Defines the entry point for the application.
//

#include "srt/Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	srt::AppContext context;

	context.width = 800; //640;
	context.height = 450; //480;

	#if defined( SRT_PLATFORM_WINDOWS )
		context.hInstance = hInstance;
	#endif

	srt::Application app{ context };

	app.Run();

    return  0;
}
