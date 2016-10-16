#include "stdafx.h"
#include "ParticleSpreadIntoCricle.h"
#include "MyTime.h"


CParticleSpreadIntoCricle::CParticleSpreadIntoCricle()
	: m_fTime(0.0f)
	, m_vecCenter(D3DXVECTOR4())
	, m_iParticleAmount(70)
	, m_fDistance(0.0f)
{
}

CParticleSpreadIntoCricle::CParticleSpreadIntoCricle(const CParticleSpreadIntoCricle & rObj)
{
}

CParticleSpreadIntoCricle & CParticleSpreadIntoCricle::operator=(const CParticleSpreadIntoCricle & rObj)
{
	return *this;
}


CParticleSpreadIntoCricle::~CParticleSpreadIntoCricle()
{
}

CParticleSpreadIntoCricle * CParticleSpreadIntoCricle::Create(LPDIRECT3DDEVICE9 pDevice, FLOAT fHeight, FLOAT fSpeed, int iAmount)
{
	CParticleSpreadIntoCricle* pParticle = new CParticleSpreadIntoCricle();
	pParticle->SetDevice(pDevice);
	if (FAILED(pParticle->Initialize(fHeight, fSpeed, iAmount)))
	{
		pParticle->Release();
		return null;
	}
	return pParticle;
}

HRESULT CParticleSpreadIntoCricle::Initialize(FLOAT fHeight, FLOAT fSpeed, int iAmount)
{
	D3DXVECTOR3 *vecPos;
	this->m_iParticleAmount = iAmount;
	this->m_fHeight = fHeight;
	this->m_fSpeed = fSpeed;
	this->m_vecCenter.y = fHeight;
	this->m_pVtx = new D3DXVECTOR3[this->m_iParticleAmount];
	FLOAT fInv = 1.0f / this->m_iParticleAmount;
	FLOAT fAngle = D3DX_PI * 2.0f * ((FLOAT)(rand() % 300 + 1) * fInv);
	for (int i = 0; i < this->m_iParticleAmount; ++i)
	{
		//fAngle = D3DX_PI * 2.0f * i * fInv;
		fAngle += D3DX_PI * 2.0f * ((FLOAT)(rand() % 9 + 1) * fInv);
		this->m_pVtx[i].x = sinf(fAngle) * 0.2f;
		this->m_pVtx[i].y = this->m_fHeight;
		this->m_pVtx[i].z = cosf(fAngle) * 0.2f;
	}
	return S_OK;
}

int CParticleSpreadIntoCricle::Update()
{
	this->m_fTime += CMyTime::deltaTime;
	if (this->m_fTime > 1.0f)
	{
		this->m_fSpeed -= CMyTime::deltaTime * 10.0f;
	}
	else
	{
		this->m_fSpeed -= CMyTime::deltaTime * 60.0f;
	}
	this->m_fDistance += this->m_fSpeed * CMyTime::deltaTime;
	if (this->m_fSpeed < 0.0f)
	{
		return 1;
	}
	if (this->m_fTime > 3.5f)
	{
		return 1;
	}
	return 0;
}

void CParticleSpreadIntoCricle::PreRender()
{
	this->m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	this->m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	this->m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	this->m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CParticleSpreadIntoCricle::Render(LPD3DXEFFECT pShader)
{
	UINT numPasses = 0;
	pShader->SetVector("gCenterPos", &this->m_vecCenter);
	pShader->SetFloat("gParticleSize", 0.8f - (3.5f - this->m_fTime) * 0.03f);
	pShader->SetFloat("gSpeed", this->m_fSpeed);
	pShader->SetFloat("gTime", this->m_fTime);
	pShader->SetFloat("gDistance", this->m_fDistance);
	FLOAT fAlpha = 0.0f;
	if (this->m_fTime > 0.4f && this->m_fDistance > 50.0f)
	{
		fAlpha = (this->m_fTime - 0.4f) / 2.0f;
		if (fAlpha > 0.6f)
		{
			fAlpha = 0.6f;
		}
	}
	pShader->SetFloat("gAlpha", fAlpha);

	pShader->Begin(&numPasses, NULL);
	pShader->BeginPass(0);

	m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX0);
	this->m_pDevice->DrawPrimitiveUP(D3DPT_POINTLIST, this->m_iParticleAmount, this->m_pVtx, sizeof(D3DXVECTOR3));

	pShader->EndPass();
	pShader->End();
}

void CParticleSpreadIntoCricle::PostRender()
{
	this->m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	this->m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
}

void CParticleSpreadIntoCricle::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	CParticle::OnResetDevice(pDevice);
}

void CParticleSpreadIntoCricle::OnLostDevice()
{
	CParticle::OnLostDevice();
}

void CParticleSpreadIntoCricle::Free()
{
	CParticle::Free();
	delete[] this->m_pVtx;
}
