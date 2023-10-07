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

// @brief �R���\�[����ʂɃt�H�[�}�b�g�t���������\��
// @param format�t�H�[�}�b�g�i%d�Ƃ�%f�Ƃ��́j
// @param �ϒ�����
// @remarks ���̊֐��̓f�o�b�O�p�ł��B�f�o�b�O���ɂ������삵�܂���B
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

	// �E�B���h�E�N���X�̐������o�^
	WNDCLASSEX w = {};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// �R�[���o�b�N�֐��̎w��
	w.lpszClassName = _T("DX12Study");							// �A�v���P�[�V�������N���X���i�K���ł悢�j
	w.hInstance = GetModuleHandle(nullptr);				// �n���h���̎擾

	RegisterClassEx(&w);		// �A�v���P�[�V�����N���X�i�E�B���h�E�N���X�̎w���OS�ɓ`����j

	RECT wrc = { 0, 0, window_width, window_height };	// �E�B���h�E�T�C�Y�����߂�
	// �֐����g���ăE�B���h�E�̃T�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd = CreateWindow(w.lpszClassName,	// �N���X���w��
		_T("DX12Study"),							// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,					// �\��X���W��OS�ɂ��C��
		CW_USEDEFAULT,					// �\��Y���W��OS�ɂ��C��
		wrc.right - wrc.left,					// �E�B���h�E��
		wrc.bottom - wrc.top,				// �E�B���h�E��
		nullptr,									// �e�E�B���h�E�n���h��
		nullptr,									// ���j���[�n���h��
		w.hInstance,							// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);									// �ǉ��p�����[�^�[

	// DirectX12����̏�����
	// �t�B�[�`���[���x����
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	HRESULT result = S_OK;
	
	// DxgiFactory�̏�����
	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&pDxgiFactory_)))) {
		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&pDxgiFactory_)))) {
			return -1;
		}
	}
	std::vector <IDXGIAdapter*> pAdapters;	// �A�_�v�^�[�̗񋓗p
	IDXGIAdapter* pTmpAdapter = nullptr;		//�@�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	for (int i = 0; pDxgiFactory_->EnumAdapters(i, &pTmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		pAdapters.push_back(pTmpAdapter);
	}
	for (auto adpt : pAdapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);	// �A�_�v�^�[�̐����I�u�W�F�N�g�擾

		std::wstring strDesc = adesc.Description;

		// �T�������A�_�v�^�[�̖��O���m�F
		// ���̃v���O�����ł�"NVIDIA"���܂܂�Ă���A�_�v�^�[�I�u�W�F�N�g��T��
		if (strDesc.find(L"NVIDIA") != std::string::npos)
		{
			pTmpAdapter = adpt;
			break;
		}
	}

	// Direct3D�f�o�C�X�̏�����
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&pDevice_)) == S_OK)
		{
			featureLevel = lv;
			break;	// �����\�ȃo�[�W���������������烋�[�v��ł��؂�
		}
	}



	// �E�B���h�E�\��
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

	// �����N���X�͎g��Ȃ��̂œo�^��������
	UnregisterClass(w.lpszClassName, w.hInstance);
	return 0;
}

//==============================
// �E�B���h�E�̐���
//==============================
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// �E�B���h�E���j�����ꂽ��Ă΂��
	if(msg == WM_DESTROY)
	{
		PostQuitMessage(0);		// OS�ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	// ����̏������s��
}

HRESULT D3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MiniMumFeatureLevel, REFIID riid, void** ppDevice)
{
	return E_NOTIMPL;
}
