#include <Windows.h>
#include <tchar.h>
#ifdef _DEBUG
#include <iostream>
#endif // !_DEBUG

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace std;

// @brief �R���\�[����ʂɃt�H�[�}�b�g�t���������\��
// @param format�t�H�[�}�b�g�i%d�Ƃ�%f�Ƃ��́j
// @param �ϒ�����
// @remarks ���̊֐��̓f�o�b�O�p�ł��B�f�o�b�O���ɂ������삵�܂���B

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void DebugOutputFormatString(const char* format, ...)
{
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	printf(format, valist);
	va_end(valist);
#endif // !_DEBUG
}

#ifdef _DEBUG
int main()
{
	// �E�B���h�E�N���X�̐������o�^
	WNDCLASSEX w = {};

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// �R�[���o�b�N�֐��̎w��
	w.lpszClassName = _T("DX12Study");							// �A�v���P�[�V�������N���X���i�K���ł悢�j
	w.hInstance = GetModuleHandle(nullptr);				// �n���h���̎擾

	RegisterClassEx(&w);		// �A�v���P�[�V�����N���X�i�E�B���h�E�N���X�̎w���OS�ɓ`����j

	RECT wrc = { (LONG)0, (LONG)0, (LONG)WINDOW_WIDTH, (LONG)WINDOW_HEIGHT };	// �E�B���h�E�T�C�Y�����߂�
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

#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif // !_DEBUG
	DebugOutputFormatString("Show window test.");
	getchar();
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