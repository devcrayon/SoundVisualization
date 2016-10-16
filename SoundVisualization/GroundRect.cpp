#include "stdafx.h"
#include "GroundRect.h"


CGroundRect::CGroundRect()
	: m_pVB(NULL)
	, m_pIB(NULL)
	, m_pVertex(NULL)
	, m_pIndex(NULL)
	, m_dwVtxSize(sizeof(VTXTEX))
	, m_dwVtxCnt(4)
	, m_dwVtxFVF(D3DFVF_XYZ | D3DFVF_TEX1)
	, m_dwTriCnt(2)
	, m_dwIdxSize(sizeof(INDEX16))
	, m_IdxFmt(D3DFMT_INDEX16)
{
}

CGroundRect::CGroundRect(const CGroundRect & rObj)
{
}

CGroundRect & CGroundRect::operator=(const CGroundRect & rObj)
{
	return *this;
}


CGroundRect::~CGroundRect()
{
}

CGroundRect * CGroundRect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGroundRect* pGroundRect = new CGroundRect();
	pGroundRect->SetDevice(pDevice);
	if (FAILED(pGroundRect->Initialize()))
	{
		pGroundRect->Release();
		return null;
	}
	return pGroundRect;
}

HRESULT CGroundRect::Initialize()
{
	HRESULT		hr = NULL;
	hr = this->m_pDevice->CreateVertexBuffer(this->m_dwVtxSize * this->m_dwVtxCnt, 0, this->m_dwVtxFVF, D3DPOOL_MANAGED, &this->m_pVB, NULL);

	m_pVB->Lock(0, 0, (void**)&this->m_pVertex, 0);
	this->m_pVertex[0].vPos = D3DXVECTOR3(-150.0f, -20.0f, -150.0f);
	this->m_pVertex[0].vTexUV = D3DXVECTOR2(0, 0);
	this->m_pVertex[1].vPos = D3DXVECTOR3(-150.0f, -20.0f, 150.0f);
	this->m_pVertex[1].vTexUV = D3DXVECTOR2(1.0f, 0);
	this->m_pVertex[2].vPos = D3DXVECTOR3(150.0f, -20.0f, -150.0f);
	this->m_pVertex[2].vTexUV = D3DXVECTOR2(0, 1.0f);
	this->m_pVertex[3].vPos = D3DXVECTOR3(150.0f, -20.0f, 150.0f);
	this->m_pVertex[3].vTexUV = D3DXVECTOR2(1.0f, 1.0f);
	m_pVB->Unlock();

	hr = this->m_pDevice->CreateIndexBuffer(this->m_dwIdxSize * this->m_dwTriCnt, 0, this->m_IdxFmt, D3DPOOL_MANAGED, &this->m_pIB, NULL);
	m_pIB->Lock(0, 0, (void**)&this->m_pIndex, 0);

	this->m_pIndex[0]._1 = 0;
	this->m_pIndex[0]._2 = 1;
	this->m_pIndex[0]._3 = 2;

	this->m_pIndex[1]._1 = 1;
	this->m_pIndex[1]._2 = 3;
	this->m_pIndex[1]._3 = 2;
	m_pIB->Unlock();
	
	return S_OK;
}

int CGroundRect::Update()
{
	return 0;
}

void CGroundRect::Render(LPD3DXEFFECT pShader)
{
	UINT numPasses = 0;
	pShader->Begin(&numPasses, NULL);
	pShader->BeginPass(0);

	m_pDevice->SetStreamSource(0, m_pVB, 0, this->m_dwVtxSize);
	m_pDevice->SetFVF(this->m_dwVtxFVF);

	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->m_dwVtxCnt, 0, this->m_dwTriCnt);

	pShader->EndPass();
	pShader->End();
}

void CGroundRect::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderObj::OnResetDevice(pDevice);
	HRESULT		hr = NULL;
	hr = this->m_pDevice->CreateVertexBuffer(this->m_dwVtxSize * this->m_dwVtxCnt, 0, this->m_dwVtxFVF, D3DPOOL_MANAGED, &this->m_pVB, NULL);

	m_pVB->Lock(0, 0, (void**)&this->m_pVertex, 0);
	this->m_pVertex[0].vPos = D3DXVECTOR3(-150.0f, -20.0f, -150.0f);
	this->m_pVertex[0].vTexUV = D3DXVECTOR2(0, 0);
	this->m_pVertex[1].vPos = D3DXVECTOR3(-150.0f, -20.0f, 150.0f);
	this->m_pVertex[1].vTexUV = D3DXVECTOR2(1.0f, 0);
	this->m_pVertex[2].vPos = D3DXVECTOR3(150.0f, -20.0f, -150.0f);
	this->m_pVertex[2].vTexUV = D3DXVECTOR2(0, 1.0f);
	this->m_pVertex[3].vPos = D3DXVECTOR3(150.0f, -20.0f, 150.0f);
	this->m_pVertex[3].vTexUV = D3DXVECTOR2(1.0f, 1.0f);
	m_pVB->Unlock();

	hr = this->m_pDevice->CreateIndexBuffer(this->m_dwIdxSize * this->m_dwTriCnt, 0, this->m_IdxFmt, D3DPOOL_MANAGED, &this->m_pIB, NULL);
	m_pIB->Lock(0, 0, (void**)&this->m_pIndex, 0);

	this->m_pIndex[0]._1 = 0;
	this->m_pIndex[0]._2 = 1;
	this->m_pIndex[0]._3 = 2;

	this->m_pIndex[1]._1 = 1;
	this->m_pIndex[1]._2 = 3;
	this->m_pIndex[1]._3 = 2;
	m_pIB->Unlock();
}

void CGroundRect::OnLostDevice()
{
	CRenderObj::OnLostDevice();
	::Safe_Release(this->m_pIB);
	::Safe_Release(this->m_pVB);
}

void CGroundRect::Free()
{
	CRenderObj::Free();
	::Safe_Release(this->m_pIB);
	::Safe_Release(this->m_pVB);
}