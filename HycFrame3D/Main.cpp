#include "RootSystem.h"

static RootSystem g_RootSystem = {};

int WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR szCmdLine,
    _In_ int iCmdShow
)
{
    if (g_RootSystem.startUp(hInstance, iCmdShow))
    {
        g_RootSystem.runGameLoop();
    }

    g_RootSystem.cleanAndStop();

    return 0;
}
