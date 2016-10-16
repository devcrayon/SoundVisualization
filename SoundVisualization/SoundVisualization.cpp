// SoundVisualization.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SoundVisualization.h"
#include "Dx9Device.h"
#include "MainApp.h"
#include "MySound.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd = NULL;
HINSTANCE g_hInstance = NULL;

int g_iWinCX = 1280;
int g_iWinCY = 720;
BOOL g_bWindowMode = TRUE;
CMainApp* pMainApp = null;
bool bAppUpdate = true;
TCHAR g_strMyPath[256] = {};

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOUNDVISUALIZATION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
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
    // 기본 메시지 루프입니다.
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
			//PeekMessage는 계속 루프를 돌기 때문에 cpu 자원을 많이 먹는다. 하지만 우리는 게임을 만드는 것이기 때문에 cpu를 계속 사용해도 상관 없음.
			//이 위치에서 만드는 게임의 루프
			//메시지를 거치지 않고 키 처리를 함
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
            // 메뉴 선택을 구문 분석합니다.
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
					openFileName.lpstrFilter = TEXT("wav 파일(*.wav)\0*.wav;");
					openFileName.lpstrFile = lpStrFile;
					openFileName.nMaxFile = sizeof(lpStrFile);
					openFileName.lpstrTitle = TEXT("파일을 선택해 주십시오");
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
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

// 정보 대화 상자의 메시지 처리기입니다.
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
