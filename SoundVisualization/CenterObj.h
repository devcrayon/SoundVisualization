#pragma once
#include "RenderObj.h"
class CCenterObj :
	public CRenderObj
{
protected:
	explicit CCenterObj();
	explicit CCenterObj(const CCenterObj& rObj);
	CCenterObj& operator = (const CCenterObj& rObj);
	virtual ~CCenterObj();

public:
	static CCenterObj * Create(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Initialize();
	void SetColor(int iR, int iG, int iB);
	virtual void Render(LPD3DXEFFECT pShader);
	inline void SetPos(D3DXVECTOR3& vecPos);
	inline void SetRot(D3DXVECTOR3& m_vecRot);
	virtual void OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void OnLostDevice();

protected:
	virtual void Free();

private:
	D3DXVECTOR4 m_vecColor;
	D3DXVECTOR3 m_vecPos;
	D3DXVECTOR3 m_vecScale;
	D3DXVECTOR3 m_vecRot;
	D3DXMATRIX m_matWorld;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	VTXCOL* m_pVertex;
	INDEX16* m_pIndex;
	DWORD m_dwVtxSize;
	DWORD m_dwVtxCnt;
	DWORD m_dwVtxFVF;
	DWORD m_dwTriCnt;
	DWORD m_dwIdxSize;
	D3DFORMAT m_IdxFmt;

	int m_iColorR;
	int m_iColorG;
	int m_iColorB;
};


inline void CCenterObj::SetPos(D3DXVECTOR3 & vecPos)
{
	memcpy(&this->m_vecPos, &vecPos, sizeof(D3DXVECTOR3));
}

inline void CCenterObj::SetRot(D3DXVECTOR3 & m_vecRot)
{
	memcpy(&this->m_vecRot, &m_vecRot, sizeof(D3DXVECTOR3));
}