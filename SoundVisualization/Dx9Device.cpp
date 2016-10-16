#include "stdafx.h"
#include "Dx9Device.h"

CDx9Device* CDx9Device::pInstance = NULL;
CDx9Device::CDx9Device()
{
	this->m_bIsInit = false;
}

CDx9Device::CDx9Device(const CDx9Device & rDevice)
{
}

CDx9Device & CDx9Device::operator=(const CDx9Device & rDevice)
{
	return *this;
}


CDx9Device::~CDx9Device()
{
}

CDx9Device * CDx9Device::GetInstance()
{
	if (pInstance == null)
	{
		pInstance = new CDx9Device();
	}
	return pInstance;
}

unsigned long CDx9Device::DestroyInstance()
{
	if (0 == pInstance->m_dwRefCnt)
	{
		return pInstance->Release();
	}
	else
		return pInstance->m_dwRefCnt--;
}

bool CDx9Device::LoadShader(TCHAR * pFileName, LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT & pEffect)
{
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;
	CString strFilePat = g_strMyPath;
	strFilePat = strFilePat.Left(strFilePat.ReverseFind('\\'));
	strFilePat += pFileName;
	TCHAR* pFilePath;
	pFilePath = (TCHAR*)(LPCTSTR)strFilePat;
	HRESULT hr = D3DXCreateEffectFromFile(pDevice, pFilePath,
		NULL, NULL, dwShaderFlags, NULL, &pEffect, &pError);
	if (hr != S_OK)
		return false;
	if (!pEffect && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			return false;
		}
	}

	return true;
}

HRESULT CDx9Device::Initialize(HWND hWnd, const WORD & wSizeX, const WORD & wSizeY)
{
	this->m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if (FAILED(this->m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		return E_FAIL;
	}

	DWORD		vp;

	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = 1920;
	d3dpp.BackBufferHeight = 1080;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = ::g_bWindowMode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(this->m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, hWnd, vp, &d3dpp, &this->m_pDevice)))
	{
		return E_FAIL;
	}
	this->m_bIsInit = true;
	return S_OK;
}

void CDx9Device::ReSize(const WORD & wSizeX, const WORD & wSizeY, bool bDeviceLost)
{
	if (!this->m_bIsInit)
		return;
	if (bDeviceLost == false)
	{
		D3DPRESENT_PARAMETERS	d3dpp;
		ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

		d3dpp.BackBufferWidth = wSizeX;
		d3dpp.BackBufferHeight = wSizeY;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		d3dpp.MultiSampleQuality = 0;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.EnableAutoDepthStencil = TRUE;

		d3dpp.hDeviceWindow = ::g_hWnd;
		d3dpp.Windowed = ::g_bWindowMode;
		if (::g_bWindowMode == FALSE)
		{
			d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);;
			d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);;
		}

		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		HRESULT hr = this->m_pDevice->Reset(&d3dpp);
		if (FAILED(hr))
		{
			int iFail = 0;
		}
	}
	else
	{
		if (NULL != this->m_pDevice)
		{
			this->m_pDevice->Release();
		}
		if (NULL != this->m_p3D)
		{
			this->m_p3D->Release();
		}
		this->m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

		D3DCAPS9		devicecaps;
		ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

		if (FAILED(this->m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
		{
			return;
		}

		DWORD		vp;

		if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		vp |= D3DCREATE_MULTITHREADED;

		D3DPRESENT_PARAMETERS	d3dpp;
		ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

		d3dpp.BackBufferWidth = 1920;
		d3dpp.BackBufferHeight = 1080;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

		d3dpp.MultiSampleQuality = 0;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.EnableAutoDepthStencil = TRUE;

		d3dpp.hDeviceWindow = ::g_hWnd;
		d3dpp.Windowed = ::g_bWindowMode;

		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if (FAILED(this->m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
			, ::g_hWnd, vp, &d3dpp, &this->m_pDevice)))
		{
			return;
		}
		return;
	}
}

void CDx9Device::Free()
{
	if (NULL != this->m_pDevice)
	{
		this->m_pDevice->Release();
	}
	if (NULL != this->m_p3D)
	{
		this->m_p3D->Release();
	}
}