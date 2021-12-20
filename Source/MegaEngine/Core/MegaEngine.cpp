#include "MegaEngineStd.h"

#include "../Core/EngineApp.h"

#pragma comment (lib, "DXUT.lib")
#pragma comment (lib, "tinyxml2.lib")

INT WINAPI MegaEngineMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow)
{

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    DXUTSetCallbackD3D11DeviceAcceptable(EngineApp::IsD3D11DeviceAcceptable);
    DXUTSetCallbackD3D11DeviceCreated(EngineApp::OnD3D11CreateDevice);
    DXUTSetCallbackD3D11SwapChainResized(EngineApp::OnD3D11ResizedSwapChain);
    DXUTSetCallbackD3D11SwapChainReleasing(EngineApp::OnD3D11ReleasingSwapChain);
    DXUTSetCallbackD3D11DeviceDestroyed(EngineApp::OnD3D11DestroyDevice);

    DXUTSetCursorSettings(true, true);

    if (!GApp->InitInstance(hInstance, cmdLine, NULL, 800, 600))
    {
        return FALSE;
    }

    DXUTMainLoop();

    return GApp->GetExitCode();
}