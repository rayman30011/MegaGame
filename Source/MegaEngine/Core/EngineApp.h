#pragma once
#include "MegaEngineStd.h"

#include "BaseGameLogic.h"

class IRenderer;

class EngineApp
{
public:
	EngineApp();

	virtual const TCHAR* GetTitle() const = 0;
	virtual const TCHAR* GetAppDirectory() const = 0;
	virtual HICON GetIcon() = 0;
	virtual BaseGameLogic* CreateGame() = 0;

	HWND GetHWnd();
	HINSTANCE GetInstance() { return _instance; }
	__forceinline bool IsQuiting() { return _quiting; }

	bool InitInstance(HINSTANCE hInstance, LPWSTR cmdLine, HWND hWnd, int screenWidth, int screenHeight);

	static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo* AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo* DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);
	static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
	static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
	static void CALLBACK OnRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);
	bool HasModalDialog();
	static void CALLBACK OnUpdate(double fTime, float fElapsedTime, void* pUserContext);

	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::wstring GetString(const std::wstring id);
	bool LoadStrings(const std::string& language);
	int GetExitCode();

	void FlashWhileMinimized();

	LRESULT OnClose();

protected:
	const std::wstring GetSaveGameDirectory(HWND hWnd, const TCHAR* gameAppDirectory);

private:
	HINSTANCE _instance;

	std::map<std::wstring, std::wstring> _textResource;
	std::shared_ptr<IRenderer> _renderer;

	BaseGameLogic* _game;

	bool _quiting;
};


extern EngineApp* GApp;
