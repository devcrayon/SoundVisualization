#include "stdafx.h"
#include "Particle.h"


CParticle::CParticle()
{
}

CParticle::CParticle(const CParticle & rObj)
{
}

CParticle & CParticle::operator=(const CParticle & rObj)
{
	return *this;
}


CParticle::~CParticle()
{
}

void CParticle::PreRender()
{
}

void CParticle::Render(LPD3DXEFFECT pShader)
{
}

void CParticle::PostRender()
{
}

void CParticle::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderObj::OnResetDevice(pDevice);
}

void CParticle::OnLostDevice()
{
	CRenderObj::OnLostDevice();
}


void CParticle::Free()
{
	CRenderObj::Free();
}
