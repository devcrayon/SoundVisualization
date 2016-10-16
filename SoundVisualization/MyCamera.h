#pragma once
#include "MyObj.h"
#include "Dx9Device.h"
class CMyCamera :
	public CMyObj
{
protected:
	explicit CMyCamera();
	explicit CMyCamera(const CMyCamera& rCamera);
	CMyCamera& operator = (const CMyCamera& rCamera);
	virtual ~CMyCamera();

public:
	static CMyCamera* Create();
	void SetViewProj(LPD3DXEFFECT pShader, const char* pViewMatKey, const char* pProjMatKey, const char* pViewProjMatKey);
	inline void SetLookAt(FLOAT fX, FLOAT fY, FLOAT fZ);
	inline void SetLookAt(const D3DXVECTOR3& vecLookAt);
	inline void SetEye(FLOAT fX, FLOAT fY, FLOAT fZ);
	inline void SetEye(const D3DXVECTOR3& vecEye);
	inline void SetUp(FLOAT fX, FLOAT fY, FLOAT fZ);
	inline void SetUp(const D3DXVECTOR3& vecUp);
	inline D3DXVECTOR3& GetLookAt();
	inline D3DXVECTOR3& GetEye();
	inline D3DXVECTOR3& GetUp();
	inline D3DXMATRIX& GetViewMatrix();
	inline D3DXMATRIX& GetProjMatrix();
	inline D3DXMATRIX& GetViewProjMatrix();

protected:
	virtual void Free();

protected:
	FLOAT m_fFovY;
	FLOAT m_fAspect;
	FLOAT m_fNear;
	FLOAT m_fFar;
	FLOAT m_fDistance;
	D3DXVECTOR3	m_vecEye;
	D3DXVECTOR3 m_vecLookAt;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecCenter;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matViewProj;
};

void CMyCamera::SetLookAt(FLOAT fX, FLOAT fY, FLOAT fZ)
{
	this->m_vecLookAt.x = fX;
	this->m_vecLookAt.y = fY;
	this->m_vecLookAt.z = fZ;
}

void CMyCamera::SetLookAt(const D3DXVECTOR3& vecLookAt)
{
	memcpy(&this->m_vecLookAt, &vecLookAt, sizeof(D3DXVECTOR3));
}

void CMyCamera::SetEye(FLOAT fX, FLOAT fY, FLOAT fZ)
{
	this->m_vecEye.x = fX;
	this->m_vecEye.y = fY;
	this->m_vecEye.z = fZ;
}

void CMyCamera::SetEye(const D3DXVECTOR3& vecEye)
{
	memcpy(&this->m_vecEye, &vecEye, sizeof(D3DXVECTOR3));
}

void CMyCamera::SetUp(FLOAT fX, FLOAT fY, FLOAT fZ)
{
	this->m_vecUp.x = fX;
	this->m_vecUp.y = fY;
	this->m_vecUp.z = fZ;
}

void CMyCamera::SetUp(const D3DXVECTOR3& vecUp)
{
	memcpy(&this->m_vecUp, &vecUp, sizeof(D3DXVECTOR3));
}

D3DXVECTOR3& CMyCamera::GetLookAt()
{
	return this->m_vecLookAt;
}
D3DXVECTOR3& CMyCamera::GetEye()
{
	return this->m_vecEye;
}
D3DXVECTOR3& CMyCamera::GetUp()
{
	return this->m_vecUp;
}

inline D3DXMATRIX & CMyCamera::GetViewMatrix()
{
	return this->m_matView;
}

inline D3DXMATRIX & CMyCamera::GetProjMatrix()
{
	return this->m_matProj;
}

inline D3DXMATRIX & CMyCamera::GetViewProjMatrix()
{
	return this->m_matViewProj;
}

