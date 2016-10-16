#pragma once
#include "RenderObj.h"

class CParticle :
	public CRenderObj
{
protected:
	explicit CParticle();
	explicit CParticle(const CParticle& rObj);
	CParticle& operator = (const CParticle& rObj);
	virtual ~CParticle();

public:
	virtual void PreRender();
	virtual void Render(LPD3DXEFFECT pShader);
	virtual void PostRender();
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();

protected:
	virtual void Free();


};
