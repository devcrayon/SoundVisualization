// SoundVisualization.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "SoundVisualization.h"
#include "Dx9Device.h"
#include "MainApp.h"
#include "MySound.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

HWND g_hWnd = NULL;
HINSTANCE g_hInstance = NULL;

int g_iWinCX = 1280;
int g_iWinCY = 720;
BOOL g_bWindowMode = TRUE;
CMainApp* pMainApp = null;
bool bAppUpdate = true;
TCHAR g_strMyPath[256] = {};

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ColorSetting(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOUNDVISUALIZATION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOUNDVISUALIZATION));

    MSG msg;
	msg.message = WM_NULL;

	GetCurrentDirectory(256, ::g_strMyPath);
	CDx9Device::GetInstance()->Initialize(::g_hWnd, ::g_iWinCX, ::g_iWinCY);
	srand((unsigned int)time(NULL));

	pMainApp = CMainApp::Create();
    // �⺻ �޽��� �����Դϴ�.
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//PeekMessage�� ��� ������ ���� ������ cpu �ڿ��� ���� �Դ´�. ������ �츮�� ������ ����� ���̱� ������ cpu�� ��� ����ص� ��� ����.
			//�� ��ġ���� ����� ������ ����
			//�޽����� ��ġ�� �ʰ� Ű ó���� ��
			if (bAppUpdate)
			{
				if (pMainApp->Update() == 1)
				{
					pMainApp->Release();
					DestroyWindow(g_hWnd);
				}
				else
					pMainApp->Render();
			}
		}
	}

    return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOUNDVISUALIZATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOUNDVISUALIZATION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ::g_hWnd = hWnd;
   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bIsAltKeyDown = false;
	static bool bIsRetrunKeyDown = false;
    switch (message)
    {
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 512;
		lpMMI->ptMinTrackSize.y = 512;
	}
	case WM_ACTIVATE:
	{
		if (wParam == false)
		{
			if (::g_bWindowMode == FALSE)
			{
				bAppUpdate = false;
			}
		}
		else
		{
			if (::g_bWindowMode == FALSE)
			{
				bAppUpdate = true;
			}
		}
	}
	break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN)
		{
			if (bIsRetrunKeyDown == false)
			{
				bIsRetrunKeyDown = true;

				if (bIsAltKeyDown == true)
				{
					if (::g_bWindowMode == TRUE)
					{
						::g_bWindowMode = FALSE;
					}
					else
					{
						::g_bWindowMode = TRUE;
						SetWindowPos(::g_hWnd, HWND_NOTOPMOST, 10, 10, 1290, 730, SWP_SHOWWINDOW);
					}
					if (pMainApp != NULL)
					{
						pMainApp->OnResetDeviceDevice();
					}
				}
			}
			else
			{
				bIsRetrunKeyDown = false;
			}
		}
		if (wParam == VK_MENU)
		{
			bIsAltKeyDown = true;
		}
		break;
	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
		{
			bIsAltKeyDown = false;
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_Color_Setting:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_COLOR_DIALOG), hWnd, ColorSetting);
				break;
			case IDM_READ_WAV:
				{
					TCHAR lpStrFile[2000] = {};
					TCHAR szFileTitle[2000] = {};

					OPENFILENAME openFileName;
					memset(&openFileName, 0, sizeof(OPENFILENAME));
					openFileName.lStructSize = sizeof(OPENFILENAME);
					openFileName.hwndOwner = NULL;
					openFileName.lpstrFilter = TEXT("wav ����(*.wav)\0*.wav;");
					openFileName.lpstrFile = lpStrFile;
					openFileName.nMaxFile = sizeof(lpStrFile);
					openFileName.lpstrTitle = TEXT("������ ������ �ֽʽÿ�");
					openFileName.lpstrFileTitle = szFileTitle;
					openFileName.nMaxFileTitle = 256;
					openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameW(&openFileName))
					{
						pMainApp->SetSound(lpStrFile);
					}
				}
				break;
			case IDM_WAVSTOP:
				if (pMainApp != NULL)
				{
					pMainApp->StopSound();
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_EXITSIZEMOVE:
		{
			if (pMainApp != NULL)
			{
				pMainApp->OnResetDeviceDevice();
			}
		}
		break;
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED)
		{
			if (pMainApp != NULL)
			{
				pMainApp->OnResetDeviceDevice();
			}
		}
		else if (wParam == SIZE_MINIMIZED)
		{
		}
		else if (wParam == SIZE_RESTORED)
		{
			bAppUpdate = true;
			if (pMainApp != NULL)
			{
				pMainApp->OnResetDeviceDevice();
			}
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ColorSetting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			int iR = ::GetDlgItemInt(hDlg, IDC_R_EDIT, NULL, FALSE);
			int iG = ::GetDlgItemInt(hDlg, IDC_G_EDIT, NULL, FALSE);
			int iB = ::GetDlgItemInt(hDlg, IDC_B_EDIT, NULL, FALSE);
			pMainApp->SetColor(iR, iG, iB);
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
