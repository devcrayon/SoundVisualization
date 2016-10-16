// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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
