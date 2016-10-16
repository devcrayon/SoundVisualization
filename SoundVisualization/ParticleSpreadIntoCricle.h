#pragma once
#include "Particle.h"
//Spread into Circle
class CParticleSpreadIntoCricle :
	public CParticle
{
public:
	explicit CParticleSpreadIntoCricle();
	explicit CParticleSpreadIntoCricle(const CParticleSpreadIntoCricle& rObj);
	CParticleSpreadIntoCricle& operator = (const CParticleSpreadIntoCricle& rObj);
	virtual ~CParticleSpreadIntoCricle();

public:
	static CParticleSpreadIntoCricle* Create(LPDIRECT3DDEVICE9 pDevice, FLOAT fHeight, FLOAT fSpeed, int iAmount);
	HRESULT Initialize(FLOAT fHeight, FLOAT fSpeed, int iAmount);
	virtual int Update();

	virtual void PreRender();
	virtual void Render(LPD3DXEFFECT pShader);
	virtual void PostRender();
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();

protected:
	virtual void Free();

private:
	FLOAT m_fTime;
	int m_iParticleAmount;
	D3DXVECTOR4 m_vecCenter;
	D3DXVECTOR3* m_pVtx;
	FLOAT m_fHeight;
	FLOAT m_fSpeed;
	FLOAT m_fDistance;
};

