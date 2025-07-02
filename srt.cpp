// srt.cpp : Defines the entry point for the application.
//

#include "srt/SrtApplication.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	srt::AppContext context;

	context.width = 800;// + 400; //640;
	context.height = 450;// + 225; //480;

	#if defined( SRT_PLATFORM_WINDOWS )
		context.appHandle = hInstance;
	#endif

	srt::SrtApplication app{ context };

	app.Run();

    return  0;
}
