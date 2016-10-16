#include "stdafx.h"
#include "CenterObj.h"


CCenterObj::CCenterObj()
	: m_vecColor(D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f))
	, m_vecPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_vecScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	, m_vecRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_pVB(NULL)
	, m_pIB(NULL)
	, m_pVertex(NULL)
	, m_pIndex(NULL)
	, m_dwVtxSize(sizeof(VTXCOL))
	, m_dwVtxCnt(6)
	, m_dwVtxFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0)
	, m_dwTriCnt(8)
	, m_dwIdxSize(sizeof(INDEX16))
	, m_IdxFmt(D3DFMT_INDEX16)
{
	D3DXMatrixIdentity(&this->m_matWorld);
}

CCenterObj::CCenterObj(const CCenterObj & rObj)
{
}

CCenterObj & CCenterObj::operator=(const CCenterObj & rObj)
{
	return *this;
}


CCenterObj::~CCenterObj()
{
}

CCenterObj * CCenterObj::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCenterObj* pCenterObj = new CCenterObj();
	pCenterObj->SetDevice(pDevice);
	if (FAILED(pCenterObj->Initialize()))
	{
		pCenterObj->Release();
		return null;
	}
	return pCenterObj;
}

HRESULT CCenterObj::Initialize()
{
	HRESULT hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(this->m_dwVtxSize * this->m_dwVtxCnt, 0, this->m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	hr = m_pDevice->CreateIndexBuffer(this->m_dwIdxSize * this->m_dwTriCnt, 0, this->m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	this->m_pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&this->m_pVertex, 0);

	this->m_pVertex[0].vPos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	this->m_pVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[1].vPos = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	this->m_pVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[2].vPos = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	this->m_pVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[3].vPos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	this->m_pVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[4].vPos = D3DXVECTOR3(0.0f, -2.5f, 0.0f);
	this->m_pVertex[4].dwColor = D3DCOLOR_ARGB(255, 69, 248, 145);
	this->m_pVertex[5].vPos = D3DXVECTOR3(0.0f, 2.5f, 0.0f);
	this->m_pVertex[5].dwColor = D3DCOLOR_ARGB(255, 69, 248, 145);

	m_pVB->Unlock();

	this->m_pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&this->m_pIndex, 0);

	
	this->m_pIndex[0]._1 = 0;
	this->m_pIndex[0]._2 = 1;
	this->m_pIndex[0]._3 = 4;

	this->m_pIndex[1]._1 = 1;
	this->m_pIndex[1]._2 = 2;
	this->m_pIndex[1]._3 = 4;

	this->m_pIndex[2]._1 = 2;
	this->m_pIndex[2]._2 = 3;
	this->m_pIndex[2]._3 = 4;

	this->m_pIndex[3]._1 = 3;
	this->m_pIndex[3]._2 = 0;
	this->m_pIndex[3]._3 = 4;

	this->m_pIndex[4]._1 = 0;
	this->m_pIndex[4]._2 = 5;
	this->m_pIndex[4]._3 = 1;

	this->m_pIndex[5]._1 = 1;
	this->m_pIndex[5]._2 = 5;
	this->m_pIndex[5]._3 = 2;

	this->m_pIndex[6]._1 = 2;
	this->m_pIndex[6]._2 = 5;
	this->m_pIndex[6]._3 = 3;

	this->m_pIndex[7]._1 = 3;
	this->m_pIndex[7]._2 = 5;
	this->m_pIndex[7]._3 = 0;

	m_pIB->Unlock();

	return S_OK;
}

void CCenterObj::SetColor(int iR, int iG, int iB)
{
	this->m_pVB->Lock(0, 0, (void**)&this->m_pVertex, 0);

	this->m_pVertex[4].dwColor = D3DCOLOR_ARGB(255, iR, iG, iB);
	this->m_pVertex[5].dwColor = D3DCOLOR_ARGB(255, iR, iG, iB);

	this->m_pVB->Unlock();

	this->m_iColorR = iR;
	this->m_iColorG = iG;
	this->m_iColorB = iB;

}

void CCenterObj::Render(LPD3DXEFFECT pShader)
{
	UINT numPasses = 0;
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMatrixTranslation(&matTrans, this->m_vecPos.x, this->m_vecPos.y, this->m_vecPos.z);
	D3DXMatrixScaling(&matScale, this->m_vecScale.x, this->m_vecScale.y, this->m_vecScale.z);
	D3DXMatrixRotationX(&matRotX, this->m_vecRot.x);
	D3DXMatrixRotationY(&matRotY, this->m_vecRot.y);
	D3DXMatrixRotationZ(&matRotZ, this->m_vecRot.z);
	D3DXMatrixMultiply(&matRotX, &matRotX, &matRotY);
	D3DXMatrixMultiply(&matRotX, &matRotX, &matRotZ);
	D3DXMatrixMultiply(&this->m_matWorld, &matScale, &matRotX);
	D3DXMatrixMultiply(&this->m_matWorld, &this->m_matWorld, &matTrans);

	pShader->SetMatrix("gWorldMatrix", &this->m_matWorld);

	pShader->Begin(&numPasses, NULL);
	pShader->BeginPass(0);
	
	m_pDevice->SetStreamSource(0, m_pVB, 0, this->m_dwVtxSize);
	m_pDevice->SetFVF(this->m_dwVtxFVF);

	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, this->m_dwVtxCnt, 0, this->m_dwTriCnt);

	pShader->EndPass();
	pShader->End();
}


void CCenterObj::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderObj::OnResetDevice(pDevice);
	HRESULT hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(this->m_dwVtxSize * this->m_dwVtxCnt, 0, this->m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	if (FAILED(hr))
	{
		return;
	}

	hr = m_pDevice->CreateIndexBuffer(this->m_dwIdxSize * this->m_dwTriCnt, 0, this->m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	if (FAILED(hr))
	{
		return;
	}
	this->m_pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&this->m_pVertex, 0);

	this->m_pVertex[0].vPos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	this->m_pVertex[0].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[1].vPos = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	this->m_pVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[2].vPos = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	this->m_pVertex[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[3].vPos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	this->m_pVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	this->m_pVertex[4].vPos = D3DXVECTOR3(0.0f, -2.5f, 0.0f);
	this->m_pVertex[4].dwColor = D3DCOLOR_ARGB(255, this->m_iColorR, this->m_iColorG, this->m_iColorB);
	this->m_pVertex[5].vPos = D3DXVECTOR3(0.0f, 2.5f, 0.0f);
	this->m_pVertex[5].dwColor = D3DCOLOR_ARGB(255, this->m_iColorR, this->m_iColorG, this->m_iColorB);
	m_pVB->Unlock();

	this->m_pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&this->m_pIndex, 0);


	this->m_pIndex[0]._1 = 0;
	this->m_pIndex[0]._2 = 1;
	this->m_pIndex[0]._3 = 4;

	this->m_pIndex[1]._1 = 1;
	this->m_pIndex[1]._2 = 2;
	this->m_pIndex[1]._3 = 4;

	this->m_pIndex[2]._1 = 2;
	this->m_pIndex[2]._2 = 3;
	this->m_pIndex[2]._3 = 4;

	this->m_pIndex[3]._1 = 3;
	this->m_pIndex[3]._2 = 0;
	this->m_pIndex[3]._3 = 4;

	this->m_pIndex[4]._1 = 0;
	this->m_pIndex[4]._2 = 5;
	this->m_pIndex[4]._3 = 1;

	this->m_pIndex[5]._1 = 1;
	this->m_pIndex[5]._2 = 5;
	this->m_pIndex[5]._3 = 2;

	this->m_pIndex[6]._1 = 2;
	this->m_pIndex[6]._2 = 5;
	this->m_pIndex[6]._3 = 3;

	this->m_pIndex[7]._1 = 3;
	this->m_pIndex[7]._2 = 5;
	this->m_pIndex[7]._3 = 0;

	m_pIB->Unlock();
}

void CCenterObj::OnLostDevice()
{
	CRenderObj::OnLostDevice();
	::Safe_Release(this->m_pIB);
	::Safe_Release(this->m_pVB);
}

void CCenterObj::Free()
{
	CRenderObj::Free();
	::Safe_Release(this->m_pIB);
	::Safe_Release(this->m_pVB);
}