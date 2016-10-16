#include "stdafx.h"
#include "MainApp.h"
#include "MyTime.h"


CMainApp::CMainApp()
	: m_pDevice(null)
	, m_pMainScene(null)
	, m_bIsRestoring(false)
	, m_bIsLostDevice(false)
{
}

CMainApp::CMainApp(const CMainApp & rApp)
{
}

CMainApp & CMainApp::operator=(const CMainApp & rApp)
{
	return *this;
}


CMainApp::~CMainApp()
{
}

CMainApp * CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp();
	if (FAILED(pMainApp->Initialize()))
	{
		pMainApp->Release();
		return null;
	}
	return pMainApp;
}

HRESULT CMainApp::Initialize()
{
	CMyTime::InitTime();
	this->m_pDevice = CDx9Device::GetInstance()->GetDevice();
	this->m_pDevice->AddRef();
	this->m_pMainScene = CMainScene::Create(this->m_pDevice);
	return S_OK;
}

int CMainApp::Update()
{
	CMyTime::SetTime();
	if (this->m_bIsLostDevice == true)
	{
		if (this->m_pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			this->OnResetDeviceDevice();
			this->m_bIsLostDevice = false;
		}
		return 0;
	}
	else
	{
		this->m_pMainScene->Update();
	}
	return 0;
}

void CMainApp::Render()
{
	if (NULL == this->m_pDevice)
	{
		return;
	}
	if (this->m_bIsLostDevice == true)
	{
		if (this->m_pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			this->OnResetDeviceDevice();
			this->m_bIsLostDevice = false;
		}
		return;
	}
	this->m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	this->m_pDevice->BeginScene();

	this->m_pMainScene->Render();

	this->m_pDevice->EndScene();
	HRESULT hr = this->m_pDevice->Present(NULL, NULL, NULL, NULL);
	if (hr == D3DERR_DEVICENOTRESET)
	{
		this->OnResetDeviceDevice();
	}
	if (hr == D3DERR_DEVICELOST)
	{
		this->m_bIsLostDevice = true;
		this->OnResetDeviceDevice();
		this->m_bIsLostDevice = false;
	}
}

void CMainApp::OnResetDeviceDevice()
{
	if (this->m_bIsRestoring)
		return;
	this->m_bIsRestoring = true;
	RECT rect;
	GetWindowRect(::g_hWnd, &rect);
	::g_iWinCX = rect.right - rect.left;
	if (::g_iWinCX < 512)
		::g_iWinCX = 512;
	::g_iWinCY = rect.bottom - rect.top;
	if (::g_iWinCY < 512)
		::g_iWinCY = 512;
	this->m_pMainScene->OnLostDevice();
	this->m_pDevice->Release();
	this->m_pDevice = NULL;
	CDx9Device::GetInstance()->ReSize(::g_iWinCX, ::g_iWinCY, this->m_bIsLostDevice);
	this->m_pDevice = CDx9Device::GetInstance()->GetDevice();
	this->m_pDevice->AddRef();
	this->m_pMainScene->OnResetDevice(this->m_pDevice);
	this->m_bIsRestoring = false;
}

void CMainApp::SetSound(std::wstring wstrFilePath)
{
	this->m_pMainScene->SetSound(wstrFilePath);
}

void CMainApp::StopSound()
{
	this->m_pMainScene->StopSound();
}

void CMainApp::SetColor(int iR, int iG, int iB)
{
	this->m_pMainScene->SetColor(iR, iG, iB);
}


void CMainApp::Free()
{
	::Safe_Release(this->m_pDevice);
	::Safe_Release(this->m_pMainScene);
	CDx9Device::DestroyInstance();
}