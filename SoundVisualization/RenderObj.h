#pragma once
#include "MyObj.h"
class CRenderObj :
	public CMyObj
{
protected:
	CRenderObj();
	virtual ~CRenderObj();

public:
	virtual int Update();
	virtual void Render(LPD3DXEFFECT pShader);
	inline void SetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();

protected:
	virtual void Free();

protected:
	LPDIRECT3DDEVICE9	m_pDevice;
};

inline void CRenderObj::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	if (this->m_pDevice != NULL)
		::Safe_Release(this->m_pDevice);
	this->m_pDevice = pDevice;
	this->m_pDevice->AddRef();
}