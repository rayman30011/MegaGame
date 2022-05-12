#include <MegaEngineStd.h>

#include "Initialization.h"
#include "Core/EngineApp.h"
#include "Graphics/D3D11/Directx11Renderer.h"
#include "ResourceCache/ResourceZipFile.h"
#include "ResourceCache/ResCache.h"
#include "ResourceCache/XmlResourceLoader.h"
#include "UI/HumanView.h"
#include "UI/UserInterface.h"

#include "timeapi.h"

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

    _screenHeight = screenHeight;
    _screenWidth = screenWidth;

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

    ResourceZipFile zipFile(_T("Assets.zip"));
    _resCache = _NEW ResCache(50, &zipFile);
    if (!_resCache->Init())
    {
        _ERROR(_T("Failed to initialize resource cache!  Are your paths set up correctly?"));
        return false;
    }

    extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();

    _resCache->RegisterLoader(CreateXmlResourceLoader());

    

    DXUTInit(true, true, cmdLine, true);
    DXUTCreateWindow(GetTitle(), hInstance, GetIcon());
    if (!GetHWnd())  return false;

    SetWindowText(GetHWnd(), GetTitle());

    DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, screenWidth, screenHeight);

    _renderer = std::make_shared<DirectX11Renderer>();
    _renderer->SetBackgroundColor(183, 237, 235, 255);
    _renderer->OnRestore();

    _game = CreateGame();
    if (!_game) return false;

    return true;
}

#pragma region DX11

bool EngineApp::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo* AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
    return true;
}

HRESULT EngineApp::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    HRESULT hr;
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
    V_RETURN(DirectX11Renderer::g_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));
    return S_OK;
}

HRESULT EngineApp::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    HRESULT hr;

    V_RETURN(DirectX11Renderer::g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    return S_OK;
}

void EngineApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
    DirectX11Renderer::g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

void EngineApp::OnD3D11DestroyDevice(void* pUserContext)
{
}

void EngineApp::OnRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
    const auto game = GApp->_game;
    for (const auto view : game->GetViews())
    {
        view->OnRender(fTime, fElapsedTime);
    }
}

bool EngineApp::HasModalDialog()
{
    return false;
}

void EngineApp::OnUpdate(double fTime, float fElapsedTime, void* pUserContext)
{
    if (GApp->HasModalDialog()) return;
    if (GApp->_quiting) PostMessage(GApp->GetHWnd(), WM_CLOSE, 0, 0);

    if (GApp->_game)
    {
        // IEventManager::Get()->Tick(20)

        GApp->_game->OnUpdate((float)fTime, fElapsedTime);
    }
    
}

LRESULT CALLBACK EngineApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
    *pbNoFurtherProcessing = DirectX11Renderer::g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    
    LRESULT result = 0;

    switch (uMsg)
    {
    case WM_CLOSE:
        return GApp->OnClose();
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_MOUSEMOVE:
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDOWN:
    // case MM_JOY1BUTTONDOWN:
    // case MM_JOY1BUTTONUP:
    // case MM_JOY1MOVE:
    // case MM_JOY1ZMOVE:
    // case MM_JOY2BUTTONDOWN:
    // case MM_JOY2BUTTONUP:
    // case MM_JOY2MOVE:
    // case MM_JOY2ZMOVE:
        {
            AppMsg msg;
            msg.HWnd = hWnd;
            msg.UMsg = uMsg;
            msg.LParam = lParam;
            msg.WParam = wParam;

            auto game = GApp->_game;
            for (auto view : game->GetViews())
            {
                if (view->OnMsgProc(msg))
                {
                    result = true;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    return result;
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
        DWORD now = GetTickCount();
        DWORD then = now;
        MSG msg;

        FlashWindow(GetHWnd(), true);

        while (true)
        {
            if (PeekMessage(&msg, NULL, 0, 0, 0))
            {
                if (msg.message != WM_SYSCOMMAND || msg.wParam != SC_CLOSE)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (!IsIconic(GetHWnd()))
                {
                    FlashWindow(GetHWnd(), false);
                    break;
                }
            }
            else
            {
                now = GetTickCount();
                DWORD timeSpan = now > then ? (now - then) : (then - now);
                if (timeSpan > 1000)
                {
                    then = now;
                    FlashWindow(GetHWnd(), true);
                }
            }
        }
    }
}

LRESULT EngineApp::OnClose()
{
    SAFE_DELETE(_game);
    DestroyWindow(GetHWnd());
    // DestroyNetworkEventForwarder()
    // SAFE_DELETE(m_pBaseSocketManager);
    // SAFE_DELETE(m_pEventManager);
    // ScriptExports::Unregister();
    // LuaStateManager::Destroy();
    SAFE_DELETE(_resCache);

    return 0;
}

int EngineApp::Modal(shared_ptr<IScreenElement> modalScreen, int defaultAnswer)
{
    HumanView* humanView = nullptr;
    for (const auto& view : _game->GetViews())
    {
        if (view->GetType() == GameViewType::Human)
        {
            const auto gameView = view;
            humanView = std::static_pointer_cast<HumanView>(gameView).get();
            break;
        }
    }

    if (!humanView)
    {
        return defaultAnswer;
    }

    assert(GetHWnd() != nullptr && _T("Main Window is NULL"));
    if ((GetHWnd() != nullptr) && IsIconic(GetHWnd()))
        FlashWhileMinimized();

    if (_hasModalDialog & 0x10000000)
    {
        assert(0 && "Too Many nested dialogs!");
        return defaultAnswer;
    }

    _hasModalDialog <<= 1;
    _hasModalDialog |= 1;

    humanView->PushElement(modalScreen);

    LPARAM lParam = 0;
    int result = PumpUntilMessage(MSG_END_MODAL, nullptr, &lParam);
    if (lParam != 0)
    {
        if (lParam == QUIT_NO_PROMPT)
            result = defaultAnswer;
        else
            result = static_cast<int>(lParam);
    }

    humanView->RemoveElement(modalScreen);
    _hasModalDialog >>= 1;
    
    return result;
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

int EngineApp::PumpUntilMessage(UINT msgEnd, WPARAM* wParam, LPARAM* lParam)
{
    int currentTime = timeGetTime();
    MSG msg;
    for (;;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
        {
            if (PeekMessage(&msg, nullptr, 0, 0, 0))
            {
                if (msg.message != WM_SYSCOMMAND || msg.wParam != SC_CLOSE)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (!IsIconic(GetHWnd()))
                {
                    FlashWindow(GetHWnd(), false);
                    break;
                }
            }
        }
        else
        {
            if (_game)
            {
                const int timeNow = timeGetTime();
                const int deltaMillisecond = timeNow - currentTime;

                for (const auto& view : _game->GetViews())
                {
                    view->OnUpdate(deltaMillisecond);
                }

                currentTime = timeNow;
                DXUTRender3DEnvironment();
            }
        }

        if (lParam)
            *lParam = msg.lParam;
        if (wParam)
            *wParam = msg.lParam;

        return 0;
    }
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
    const auto root = XmlResourceLoader::LoadAndReturnRoot(languageFile.c_str());
    if (!root)
    {
        _ERROR(TEXT("Strings are missing"));
        return false;
    }
    
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