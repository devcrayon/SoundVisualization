// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <mmsystem.h>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <commdlg.h>
#include <atlstr.h>
#pragma comment(lib, "winmm.lib")

#include <list>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


typedef struct tagIndex16
{
	WORD		_1, _2, _3;

}INDEX16;

typedef struct tagVertexTex
{
	D3DXVECTOR3			vPos;
	D3DXVECTOR2			vTexUV;

}VTXTEX;

typedef struct tagVertexColor
{
	D3DXVECTOR3			vPos;
	DWORD				dwColor;

}VTXCOL;

template <typename T>
DWORD Safe_Release(T& pointer)
{
	DWORD		dwRefCnt = 0;

	if (NULL != pointer)
	{
		dwRefCnt = pointer->Release();
		if (dwRefCnt == 0)
			pointer = NULL;
	}

	return dwRefCnt;
}

#define null 0

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

extern int g_iWinCX;
extern int g_iWinCY;

extern BOOL g_bWindowMode;
extern TCHAR g_strMyPath[256];
