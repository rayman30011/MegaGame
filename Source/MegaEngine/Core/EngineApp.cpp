#include <MegaEngineStd.h>

#include "Initialization.h"
#include "../Core/EngineApp.h"
#include "../Graphics/D3D11/Directx11Renderer.h"

using namespace MegaEngine::Core;

EngineApp* GApp = nullptr;


EngineApp::EngineApp()
{
    GApp = this;
}

bool EngineApp::InitInstance(HINSTANCE hInstance, LPWSTR cmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
#ifndef _DEBUG
    if (!IsOnlyInstance(GetTitle()))
        return false;
#endif

    SetCursor(NULL);

    bool resourceCheck = false;
    while (!resourceCheck)
    {
        const DWORDLONG physicalRAM = 512 * MEGABYTE;
        const DWORDLONG virtualRAM = 1024 * MEGABYTE;
        const DWORDLONG diskSpace = 10 * MEGABYTE;
        if (!Initialization::CheckStorage(diskSpace))
            return false;

        const DWORD minCpuSpeed = 1300;			// 1.3Ghz
        DWORD thisCPU = Initialization::ReadCPUSpeed();
        if (thisCPU < minCpuSpeed)
        {
            _ERROR(TEXT("GetCPUSpeed reports CPU is too slow for this game."));
            return false;
        }

        resourceCheck = true;
    }

    DXUTInit(true, true, cmdLine, true);
    DXUTCreateWindow(GetTitle(), hInstance, GetIcon());
    if (!GetHWnd())  return false;

    SetWindowText(GetHWnd(), GetTitle());

    DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, screenWidth, screenHeight);

    _renderer = std::shared_ptr<IRenderer>(_NEW DirectX11Renderer());
    _renderer->SetBackgroupColor(255, 20, 20, 200);
    _renderer->OnRestore();

    return true;
}

#pragma region DX11

bool EngineApp::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo* AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
    return true;
}

HRESULT EngineApp::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    return S_OK;
}

HRESULT EngineApp::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    return S_OK;
}

void EngineApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
}

void EngineApp::OnD3D11DestroyDevice(void* pUserContext)
{
}

void EngineApp::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
}

#pragma endregion

#pragma region WIN

HWND EngineApp::GetHWnd()
{
    return DXUTGetHWND();
}

#pragma endregion

int EngineApp::GetExitCode()
{
    return DXUTGetExitCode();
}

void EngineApp::FlashWhileMinimized()
{
    if (!GetHWnd()) return;

    if (IsIconic(GetHWnd()))
    {

    }
}

const std::wstring EngineApp::GetSaveGameDirectory(HWND hWnd, const TCHAR* gameAppDirectory)
{
    static TCHAR saveGameDirectory[MAX_PATH];
    TCHAR userDataPath[MAX_PATH];

    if (!SHGetSpecialFolderPath(hWnd, userDataPath, CSIDL_APPDATA, true))
    {
        _ERROR(TEXT("NOT FOUND PATH"));
        return L"";
    }

    _tcscpy_s(saveGameDirectory, userDataPath);
    _tcscat_s(saveGameDirectory, TEXT("\\"));
    _tcscat_s(saveGameDirectory, gameAppDirectory);

    if (0xffffffff == GetFileAttributes(saveGameDirectory))
    {
        if (SHCreateDirectoryEx(hWnd, saveGameDirectory, nullptr) != S_OK)
        {
            return std::wstring();
        }
    }

    _tcscat_s(saveGameDirectory, TEXT("\\"));

    return saveGameDirectory;
}


std::wstring EngineApp::GetString(const std::wstring id)
{
    const auto localizedStr = _textResource.find(id);
    if (localizedStr == _textResource.end())
    {
        E_ASSERT(0 && "String not found!");
        return id;
    }

    return localizedStr->second;
}

bool EngineApp::LoadStrings(const std::string& language)
{
    std::string languageFile = "Strings\\" + language + ".xml";
    xml::XMLDocument doc;
    const auto error = doc.LoadFile(languageFile.c_str());
    if (error != xml::XMLError::XML_SUCCESS)
    {
        _ERROR(TEXT("Strings are missing"));
        return false;
    }

    const auto root = doc.RootElement();
    for (auto el = root->FirstChildElement(); el; el = el->NextSiblingElement())
    {
        std::string key = el->Attribute("id");
        std::string text = el->Attribute("value");

        if (!key.empty() && !text.empty())
        {
            int wchars_num = MultiByteToWideChar(CP_UTF8, 0, key.c_str(), -1, NULL, 0);
            wchar_t* wstr_key = new wchar_t[wchars_num];
            MultiByteToWideChar(CP_UTF8, 0, key.c_str(), -1, wstr_key, wchars_num);

            wchars_num = MultiByteToWideChar(CP_UTF8, 0, key.c_str(), -1, NULL, 0);
            wchar_t* wstr_text = new wchar_t[wchars_num];
            MultiByteToWideChar(CP_UTF8, 0, key.c_str(), -1, wstr_text, wchars_num);

            _textResource[wstr_key] = wstr_text;
        }
    }

    return true;
}