#include <Windows.h>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif // !_DEBUG

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// @brief コンソール画面にフォーマット付き文字列を表示
// @param formatフォーマット（%dとか%fとかの）
// @param 可変長引数
// @remarks この関数はデバッグ用です。デバッグ時にしか動作しません。
void DebugOutputFormatString(const char* format, ...)
{
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif // !_DEBUG
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

const unsigned int window_width = 1280;
const unsigned int window_height = 720;

ID3D12Device* pDevice_ = nullptr;
IDXGIFactory6* pDxgiFactory_ = nullptr;
IDXGISwapChain4* pSwapChain_ = nullptr;



#ifdef _DEBUG
int main() {
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
#endif // !_DEBUG

	// ウィンドウクラスの生成＆登録
	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// コールバック関数の指定
	w.lpszClassName = _T("DX12Study");							// アプリケーションンクラス名（適当でよい）
	w.hInstance = GetModuleHandle(nullptr);				// ハンドルの取得

	RegisterClassEx(&w);		// アプリケーションクラス（ウィンドウクラスの指定をOSに伝える）

	RECT wrc = { 0, 0, window_width, window_height };	// ウィンドウサイズを決める
	// 関数を使ってウィンドウのサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	HWND hwnd = CreateWindow(w.lpszClassName,	// クラス名指定
		_T("DX12Study"),							// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,					// 表示X座標はOSにお任せ
		CW_USEDEFAULT,					// 表示Y座標はOSにお任せ
		wrc.right - wrc.left,					// ウィンドウ幅
		wrc.bottom - wrc.top,				// ウィンドウ高
		nullptr,									// 親ウィンドウハンドル
		nullptr,									// メニューハンドル
		w.hInstance,							// 呼び出しアプリケーションハンドル
		nullptr);									// 追加パラメーター

	// DirectX12周りの初期化
	// フィーチャーレベル列挙
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	HRESULT result = S_OK;
	
	// DxgiFactoryの初期化
	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pDxgiFactory_)))) {
		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&pDxgiFactory_)))) {
			return -1;
		}
	}
	std::vector <IDXGIAdapter*> pAdapters;	// アダプターの列挙用
	IDXGIAdapter* pTmpAdapter = nullptr;		//　ここに特定の名前を持つアダプターオブジェクトが入る
	for (int i = 0; pDxgiFactory_->EnumAdapters(i, &pTmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		pAdapters.push_back(pTmpAdapter);
	}
	for (auto adpt : pAdapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);	// アダプターの説明オブジェクト取得

		std::wstring strDesc = adesc.Description;

		// 探したいアダプターの名前を確認
		// このプログラムでは"NVIDIA"が含まれているアダプターオブジェクトを探す
		if (strDesc.find(L"NVIDIA") != std::string::npos)
		{
			pTmpAdapter = adpt;
			break;
		}
	}

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&pDevice_)) == S_OK)
		{
			featureLevel = lv;
			break;	// 生成可能なバージョンが見つかったらループを打ち切り
		}
	}



	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}
	}

	// もうクラスは使わないので登録解除する
	UnregisterClass(w.lpszClassName, w.hInstance);
	return 0;
}

//==============================
// ウィンドウの生成
//==============================
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ウィンドウが破棄されたら呼ばれる
	if(msg == WM_DESTROY)
	{
		PostQuitMessage(0);		// OSに対して「もうこのアプリは終わる」と伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	// 既定の処理を行う
}

HRESULT D3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MiniMumFeatureLevel, REFIID riid, void** ppDevice)
{
	return E_NOTIMPL;
}
