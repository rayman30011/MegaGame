#include "MegaEngineStd.h"

#include <iostream>

#include "../Core/EngineApp.h"

#pragma comment (lib, "DXUT.lib")
#pragma comment (lib, "DXUTOpt.lib")
#pragma comment (lib, "tinyxml2.lib")
#pragma comment (lib, "libz-static.lib")

INT WINAPI MegaEngineMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int cmdShow)
{

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    AllocConsole();

    /*GETConsole

    HANDLE hConsoleSTDIN = CreateFile(TEXT("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    SetStdHandle(STD_INPUT_HANDLE, hConsoleSTDIN);
    HANDLE hConsoleSTDOUT = CreateFile(TEXT("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    SetStdHandle(STD_OUTPUT_HANDLE, hConsoleSTDOUT);*/
#endif // _DEBUG

    DXUTSetCallbackD3D11DeviceAcceptable(EngineApp::IsD3D11DeviceAcceptable);
    DXUTSetCallbackD3D11DeviceCreated(EngineApp::OnD3D11CreateDevice);
    DXUTSetCallbackD3D11SwapChainResized(EngineApp::OnD3D11ResizedSwapChain);
    DXUTSetCallbackD3D11SwapChainReleasing(EngineApp::OnD3D11ReleasingSwapChain);
    DXUTSetCallbackD3D11DeviceDestroyed(EngineApp::OnD3D11DestroyDevice);
    DXUTSetCallbackFrameMove(EngineApp::OnUpdate);
    DXUTSetCallbackD3D11FrameRender(EngineApp::OnRender);

    DXUTSetCursorSettings(true, true);

    if (!GApp->InitInstance(hInstance, cmdLine, NULL, 800, 600))
    {
        return FALSE;
    }

    DXUTMainLoop();

    return GApp->GetExitCode();
}