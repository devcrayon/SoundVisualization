#include "stdafx.h"
#include "MainScene.h"
#include "MyTime.h"
#include "ParticleSpreadIntoCricle.h"


CMainScene::CMainScene()
	: m_pDevice(NULL)
	, m_pCamera(NULL)
	, m_fTime(0.0f)
	, m_pSound(NULL)
	, m_fParticleHeight(0.0f)
	, m_fCenterRot(0.0f)
	, m_bIsUp(true)
	, m_iColorR(69)
	, m_iColorG(248)
	, m_iColorB(145)
{
	FILE* pF = NULL;
	CString strFilePat = g_strMyPath;
	strFilePat = strFilePat.Left(strFilePat.ReverseFind('\\'));
	strFilePat += _T("/bin/color/color.txt");
	TCHAR* pFilePath;
	pFilePath = (TCHAR*)(LPCTSTR)strFilePat;
	_wfopen_s(&pF, pFilePath, L"r+");
	fscanf_s(pF, "%d,", &this->m_iColorR);
	fscanf_s(pF, "%d,", &this->m_iColorG);
	fscanf_s(pF, "%d,", &this->m_iColorB);
	fclose(pF);
}

CMainScene::CMainScene(const CMainScene & rScene)
{
}

CMainScene & CMainScene::operator=(const CMainScene & rScene)
{
	return *this;
}


CMainScene::~CMainScene()
{
}

CMainScene * CMainScene::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainScene* pScene = new CMainScene();
	if (FAILED(pScene->Initialize(pDevice)))
	{
		pScene->Release();
		return NULL;
	}
	return pScene;
}

HRESULT CMainScene::Initialize(LPDIRECT3DDEVICE9 pDevice)
{
	this->m_pDevice = pDevice;
	this->m_pDevice->AddRef();
	this->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
	this->m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	if (FAILED(this->LoadShader()))
	{
		return E_FAIL;
	}
	if (FAILED(this->LoadTexture()))
	{
		return E_FAIL;
	}
	if (FAILED(this->MakeRenderTarget()))
	{
		return E_FAIL;
	}
	if (FAILED(this->MakeObj()))
	{
		return E_FAIL;
	}
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetTexture("DiffuseMap_Tex", this->m_pTexture);
	return S_OK;
}

int CMainScene::Update()
{
	this->m_fTime += CMyTime::deltaTime;
	this->m_fCenterRot += CMyTime::deltaTime;
	D3DXVECTOR3 vecRot = D3DXVECTOR3(0.0f, this->m_fCenterRot, 0.0f);
	this->m_pCenterObj->SetRot(vecRot);

	if (this->m_fTime > 0.02f)
	{
		this->m_fTime -= 0.02f;
		this->MakeParticle();

	}

	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();

	while (iter != iterEnd)
	{
		if ((*iter)->Update() != 0)
		{
			::Safe_Release((*iter));
			iter = this->m_particleList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	return 0;
}

void CMainScene::Render()
{
	std::list<CRenderObj*>::iterator iter;
	std::list<CRenderObj*>::iterator iterEnd;
	D3DXVECTOR4 vecColor;
	HRESULT hr;
	this->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	hr = this->m_pDevice->GetRenderTarget(0, &this->m_pOriBackBuffer);
	hr = this->m_pDevice->SetRenderTarget(0, this->m_pGroundRectSurface);
	this->m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.f, 0);

	this->m_pShader[SHADER_TPYE_CENTER_OBJ]->SetMatrix("gViewMatrix", &this->m_matGroundRectView);
	this->m_pShader[SHADER_TPYE_CENTER_OBJ]->SetMatrix("gViewProjectionMatrix", &this->m_matGroundRectViewProj);
	this->m_pCenterObj->Render(this->m_pShader[SHADER_TPYE_CENTER_OBJ]);

	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetMatrix("gViewMatrix", &this->m_matGroundRectView);
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetMatrix("gViewProjectionMatrix", &this->m_matGroundRectViewProj);
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetFloat("gViewPortHeight", 512);
	vecColor = D3DXVECTOR4(((FLOAT)this->m_iColorR) / 255.0f, ((FLOAT)this->m_iColorG) / 255.0f, ((FLOAT)this->m_iColorB) / 255.0f, 1.0f);
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetVector("gColor", &vecColor);
	iter = this->m_particleList.begin();
	iterEnd = this->m_particleList.end();
	while (iter != iterEnd)
	{
		(*iter)->Render(this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]);
		++iter;
	}
	//------------------------------------------------------------------------------------------

	this->m_pDevice->SetRenderTarget(0, this->m_pOriBackBuffer);

	this->m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.f, 0);
	this->m_pCamera->SetViewProj(this->m_pShader[SHADER_TPYE_GROUND_RECT_TEXTURE], "gViewMatrix", "gProjectionMatrix", "gViewProjectionMatrix");
	hr = this->m_pShader[SHADER_TPYE_GROUND_RECT_TEXTURE]->SetTexture("DiffuseMap_Tex", this->m_pGroundRectTarget);
	this->m_pGroundRect->Render(this->m_pShader[SHADER_TPYE_GROUND_RECT_TEXTURE]);

	this->m_pCamera->SetViewProj(this->m_pShader[SHADER_TPYE_CENTER_OBJ], "gViewMatrix", "gProjectionMatrix", "gViewProjectionMatrix");
	this->m_pCenterObj->Render(this->m_pShader[SHADER_TPYE_CENTER_OBJ]);

	this->m_pCamera->SetViewProj(this->m_pShader[SHADER_TPYE_COLOR_PARTICLE], "gViewMatrix", "gProjectionMatrix", "gViewProjectionMatrix");
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetFloat("gViewPortHeight", ((FLOAT)::g_iWinCY));
	vecColor = D3DXVECTOR4(((FLOAT)this->m_iColorR) / 255.0f, ((FLOAT)this->m_iColorG) / 255.0f, ((FLOAT)this->m_iColorB) / 255.0f, 1.0f);
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetVector("gColor", &vecColor);
	iter = this->m_particleList.begin();
	iterEnd = this->m_particleList.end();
	while (iter != iterEnd)
	{
		((CParticle*)(*iter))->PreRender();
		(*iter)->Render(this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]);
		((CParticle*)(*iter))->PostRender();
		++iter;
	}

	this->m_pOriBackBuffer->Release();
}

void CMainScene::OnLostDevice()
{
	int iA = this->m_pDevice->Release();


	for (int i = 0; i < SHADER_TPYE_END; ++i)
	{
		while (this->m_pShader[i]->Release() > 0);
		this->m_pShader[i] = NULL;
	}
	this->m_pCenterObj->OnLostDevice();
	this->m_pGroundRect->OnLostDevice();
	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();
	while (iter != iterEnd)
	{
		(*iter)->OnLostDevice();
		++iter;
	}
	while (this->m_pTexture->Release() > 0);
	this->m_pTexture = NULL;
	while (this->m_pGroundRectSurface->Release() > 0);
	this->m_pGroundRectSurface = NULL;
	while (this->m_pGroundRectTarget->Release() > 0);
	this->m_pGroundRectTarget = NULL;
}

HRESULT CMainScene::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	this->m_pDevice = pDevice;
	this->m_pDevice->AddRef();
	this->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	this->m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
	this->m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	if (FAILED(this->LoadShader()))
	{
		return E_FAIL;
	}
	if (FAILED(this->LoadTexture()))
	{
		return E_FAIL;
	}
	if (FAILED(this->MakeRenderTarget()))
	{
		return E_FAIL;
	}
	this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]->SetTexture("DiffuseMap_Tex", this->m_pTexture);

	this->m_pCenterObj->OnResetDevice(this->m_pDevice);
	this->m_pGroundRect->OnResetDevice(this->m_pDevice);
	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();
	while (iter != iterEnd)
	{
		(*iter)->OnResetDevice(this->m_pDevice);
		++iter;
	}
}

void CMainScene::SetSound(std::wstring wstrFilePath)
{
	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();

	while (iter != iterEnd)
	{
		::Safe_Release((*iter));
		++iter;
	}
	this->m_particleList.clear();
	if (this->m_pSound != NULL)
	{
		this->m_pSound->Stop();
		this->m_pSound->Release();
		this->m_pSound = NULL;
	}
	this->m_fParticleHeight = 0.0f;
	this->m_pSound = CMySound::Create(1);
	this->m_pSound->SetFile(wstrFilePath);
	this->m_pSound->Play();
}

void CMainScene::StopSound()
{
	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();

	while (iter != iterEnd)
	{
		::Safe_Release((*iter));
		++iter;
	}
	this->m_particleList.clear();
	if (this->m_pSound != NULL)
	{
		this->m_pSound->Stop();
		this->m_pSound->Release();
		this->m_pSound = NULL;
	}
	this->m_fParticleHeight = 0.0f;
}

void CMainScene::SetColor(int iR, int iG, int iB)
{
	this->m_iColorR = iR;
	this->m_iColorG = iG;
	this->m_iColorB = iB;
	if (this->m_pCenterObj != NULL)
	{
		this->m_pCenterObj->SetColor(this->m_iColorR, this->m_iColorG, this->m_iColorB);
	}
}

void CMainScene::Free()
{
	std::list<CRenderObj*>::iterator iter = this->m_particleList.begin();
	std::list<CRenderObj*>::iterator iterEnd = this->m_particleList.end();

	while (iter != iterEnd)
	{
		::Safe_Release((*iter));
		++iter;
	}
	this->m_particleList.clear();
	::Safe_Release(this->m_pCenterObj);
	::Safe_Release(this->m_pCamera);
	::Safe_Release(this->m_pGroundRect);
	for (int i = 0; i < SHADER_TPYE_END; ++i)
	{
		::Safe_Release(this->m_pShader[i]);
	}
	::Safe_Release(this->m_pTexture);
	::Safe_Release(this->m_pGroundRectSurface);
	::Safe_Release(this->m_pGroundRectTarget);
	if (this->m_pSound != NULL)
	{
		this->m_pSound->Stop();
		this->m_pSound->Release();
		this->m_pSound = NULL;
	}
	this->m_pDevice->Release();
}


HRESULT CMainScene::LoadShader()
{
	if (false == CDx9Device::LoadShader(_T("/bin/fx/ColorShader.fx"), this->m_pDevice, this->m_pShader[SHADER_TPYE_CENTER_OBJ]))
	{
		return E_FAIL;
	}
	if (false == CDx9Device::LoadShader(_T("/bin/fx/ParticleSpreadIntoCricleShader.fx"), this->m_pDevice, this->m_pShader[SHADER_TPYE_COLOR_PARTICLE]))
	{
		return E_FAIL;
	}
	if (false == CDx9Device::LoadShader(_T("/bin/fx/TextureMappingShader.fx"), this->m_pDevice, this->m_pShader[SHADER_TPYE_GROUND_RECT_TEXTURE]))
	{
		return E_FAIL;
	}

	D3DXMATRIX matProj;
	D3DXMatrixLookAtLH(&this->m_matGroundRectView, &D3DXVECTOR3(150.0f, -150.0f, 0.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.0f), 1.0f, 1.0f, 1000.0f);
	D3DXMatrixMultiply(&this->m_matGroundRectViewProj, &this->m_matGroundRectView, &matProj);
	return S_OK;
}

HRESULT CMainScene::LoadTexture()
{
	CString strFilePat = g_strMyPath;
	strFilePat = strFilePat.Left(strFilePat.ReverseFind('\\'));
	strFilePat += _T("/bin/texture/particle.tga");
	TCHAR* pFilePath;
	pFilePath = (TCHAR*)(LPCTSTR)strFilePat;
	HRESULT hr = D3DXCreateTextureFromFile(this->m_pDevice, pFilePath, &this->m_pTexture);
	if (hr != S_OK)
		return E_FAIL;
	return S_OK;
}

HRESULT CMainScene::MakeRenderTarget()
{
	HRESULT hr = this->m_pDevice->CreateTexture(512, 512, 1, D3DUSAGE_RENDERTARGET
		, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->m_pGroundRectTarget, NULL);
	if (hr != S_OK)
		return E_FAIL;
	hr = this->m_pGroundRectTarget->GetSurfaceLevel(0, &this->m_pGroundRectSurface);
	if (hr != S_OK)
		return E_FAIL;
	return S_OK;
}

HRESULT CMainScene::MakeObj()
{
	this->m_pCamera = CMyCamera::Create();
	if (this->m_pCamera == NULL)
		return E_FAIL;
	this->m_pCenterObj = CCenterObj::Create(this->m_pDevice);
	if (this->m_pCenterObj == NULL)
		return E_FAIL;
	this->m_pCenterObj->SetColor(this->m_iColorR, this->m_iColorG, this->m_iColorB);
	this->m_pGroundRect = CGroundRect::Create(this->m_pDevice);
	if (this->m_pGroundRect == NULL)
		return E_FAIL;
	return S_OK;
}

void CMainScene::MakeParticle()
{
	if (this->m_pSound == NULL)
		return;
	int iIndex = this->m_pSound->GetIndex();
	if (iIndex < 0)
		return;
	int iNowSoundHeight = this->m_pSound->GetData(iIndex);
	int iGap = this->m_iWaveHeight - iNowSoundHeight;
	if (iGap < 0)
		iGap = -iGap;
	if (iGap < 1000)
		return;
	FLOAT fGap = ((FLOAT)iGap) * 0.00001f;
	this->m_fCenterRot += fGap * 0.03f;
	fGap += 0.05f;

	if (this->m_bIsUp == true)
	{
		if (this->m_fParticleHeight < -3.0f)
		{
			this->m_fParticleHeight = -3.0f;
		}
		this->m_fParticleHeight += fGap;
		if (this->m_fParticleHeight > 3.0f)
		{
			this->m_bIsUp = false;
		}
	}
	else
	{
		if (this->m_fParticleHeight > 3.0f)
		{
			this->m_fParticleHeight = 3.0f;
		}
		this->m_fParticleHeight -= fGap;
		if (this->m_fParticleHeight < -3.0f)
		{
			this->m_bIsUp = true;
		}
	}
	D3DXVECTOR3 vecPos = D3DXVECTOR3(0.0f, this->m_fParticleHeight, 0.0f);
	this->m_pCenterObj->SetPos(vecPos);
	FLOAT fSpeed = 60.0f;
	iGap = iNowSoundHeight - this->m_pSound->GetData(iIndex - 1);
	if (iGap < 0)
		iGap = -iGap;
	if (iNowSoundHeight < 0)
		iNowSoundHeight = -iNowSoundHeight;
	FLOAT fTmp = ((FLOAT)iNowSoundHeight * 0.3f + iGap * 3.0f) / 131072.0f;
	fSpeed += 60 * fTmp;
	int iAmount = (int)(fSpeed * 0.2f);
	iAmount -= 10;
	CParticleSpreadIntoCricle* pParticle = CParticleSpreadIntoCricle::Create(this->m_pDevice, this->m_fParticleHeight + 0.2f, fSpeed, iAmount);
	if (pParticle != NULL)
	{
		this->m_particleList.push_back(pParticle);
	}
	iAmount += rand() % 3 + 1;
	pParticle = CParticleSpreadIntoCricle::Create(this->m_pDevice, this->m_fParticleHeight, fSpeed, iAmount);
	if (pParticle != NULL)
	{
		this->m_particleList.push_back(pParticle);
	}
	iAmount += rand() % 3 + 1;
	pParticle = CParticleSpreadIntoCricle::Create(this->m_pDevice, this->m_fParticleHeight - 0.2f, fSpeed, iAmount);
	if (pParticle != NULL)
	{
		this->m_particleList.push_back(pParticle);
	}
	this->m_iWaveHeight = iNowSoundHeight;
}