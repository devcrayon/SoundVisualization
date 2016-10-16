#include "stdafx.h"
#include "MyCamera.h"


CMyCamera::CMyCamera()
{
	memset(this->m_vecEye, 0, sizeof(D3DXVECTOR3));
	memset(this->m_vecLookAt, 0, sizeof(D3DXVECTOR3));
	memset(this->m_vecUp, 0, sizeof(D3DXVECTOR3));
	memset(this->m_vecCenter, 0, sizeof(D3DXVECTOR3));
	memset(this->m_matView, 0, sizeof(D3DXMATRIX));
	memset(this->m_matProj, 0, sizeof(D3DXMATRIX));
	memset(this->m_matViewProj, 0, sizeof(D3DXMATRIX));
	this->m_vecEye.x = 60.0f;
	this->m_vecEye.y = 40.0f;
	this->m_vecEye.z = 0.0f;
	this->m_vecUp.y = 1.0f;
	this->m_fFovY = D3DXToRadian(45.f);
	this->m_fNear = 1.0f;
	this->m_fFar = 1000.0f;
	this->m_fDistance = 5.0f;
}

CMyCamera::CMyCamera(const CMyCamera & rCamera)
{
}

CMyCamera & CMyCamera::operator=(const CMyCamera & rCamera)
{
	return *this;
}


CMyCamera::~CMyCamera()
{
}

CMyCamera * CMyCamera::Create()
{
	CMyCamera* pCamera = new CMyCamera();

	return pCamera;
}

void CMyCamera::SetViewProj(LPD3DXEFFECT pShader, const char* pViewMatKey, const char* pProjMatKey, const char* pViewProjMatKey)
{
	this->m_fAspect = ((FLOAT)::g_iWinCX) / ((FLOAT)::g_iWinCY);
	D3DXMatrixLookAtLH(&this->m_matView, &this->m_vecEye, &this->m_vecLookAt, &this->m_vecUp);
	D3DXMatrixPerspectiveFovLH(&this->m_matProj, this->m_fFovY, this->m_fAspect, this->m_fNear, this->m_fFar);
	D3DXMatrixMultiply(&this->m_matViewProj, &this->m_matView, &this->m_matProj);
	pShader->SetMatrix(pViewMatKey, &this->m_matView);
	pShader->SetMatrix(pProjMatKey, &this->m_matProj);
	pShader->SetMatrix(pViewProjMatKey, &this->m_matViewProj);
	D3DXVECTOR3 vecTmp;
	memcpy(&vecTmp, &this->m_vecEye, sizeof(D3DXVECTOR3));
	vecTmp -= this->m_vecLookAt;
	this->m_fDistance = D3DXVec3Length(&vecTmp);
}

void CMyCamera::Free()
{
}

