#include "stdafx.h"
#include "RenderObj.h"


CRenderObj::CRenderObj()
	: m_pDevice(NULL)
{
}


CRenderObj::~CRenderObj()
{
}

int CRenderObj::Update()
{
	return 0;
}

void CRenderObj::Render(LPD3DXEFFECT pShader)
{
}

void CRenderObj::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	this->SetDevice(pDevice);
}

void CRenderObj::OnLostDevice()
{
	//::Safe_Release(this->m_pDevice);
}

void CRenderObj::Free()
{
	::Safe_Release(this->m_pDevice);
}

