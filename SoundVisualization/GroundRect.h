#pragma once
#include "RenderObj.h"
class CGroundRect :
	public CRenderObj
{
public:
	explicit CGroundRect();
	explicit CGroundRect(const CGroundRect& rObj);
	CGroundRect& operator = (const CGroundRect& rObj);
	virtual ~CGroundRect();

public:
	static CGroundRect * Create(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Initialize();
	virtual int Update();
	virtual void Render(LPD3DXEFFECT pShader);
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();

protected:
	virtual void Free();

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	VTXTEX* m_pVertex;
	INDEX16* m_pIndex;
	DWORD m_dwVtxSize;
	DWORD m_dwVtxCnt;
	DWORD m_dwVtxFVF;
	DWORD m_dwTriCnt;
	DWORD m_dwIdxSize;
	D3DFORMAT m_IdxFmt;
};

