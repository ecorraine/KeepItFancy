#include	"main.h"
#include	"Application.h"

int wmain(int argc, wchar_t** argv, wchar_t** evnp)
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	APPLICATION app(SCREEN_WIDTH, SCREEN_HEIGHT);
	app.Boot();

	int result = app.MainLoop();

	return result;
}