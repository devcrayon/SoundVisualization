#pragma once
#include "MyObj.h"
#include "RenderObj.h"
#include "MyCamera.h"
#include "MySound.h"
#include "CenterObj.h"
#include "GroundRect.h"
class CMainScene :
	public CMyObj
{
public:
	enum eSHADER_TYPE
	{
		SHADER_TPYE_CENTER_OBJ = 0,
		SHADER_TPYE_COLOR_PARTICLE,
		SHADER_TPYE_GROUND_RECT_TEXTURE,
		SHADER_TPYE_END,
	};
protected:
	explicit CMainScene();
	explicit CMainScene(const CMainScene& rScene);
	CMainScene& operator = (const CMainScene& rScene);
	virtual ~CMainScene();

public:
	static CMainScene* Create(LPDIRECT3DDEVICE9 pDevice);
	HRESULT Initialize(LPDIRECT3DDEVICE9 pDevice);
	int Update();
	void Render();
	void OnLostDevice();
	HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	void SetSound(std::wstring wstrFilePath);
	void StopSound();
	void SetColor(int iR, int iG, int iB);

protected:
	virtual void Free();

private:
	HRESULT LoadShader();
	HRESULT LoadTexture();
	HRESULT MakeRenderTarget();
	HRESULT MakeObj();
	void MakeParticle();

private:
	bool m_bIsUp;
	INT m_iColorR;
	INT m_iColorG;
	INT m_iColorB;
	INT m_iWaveHeight;
	FLOAT m_fTime;
	FLOAT m_fParticleHeight;
	FLOAT m_fCenterRot;
	D3DXMATRIX m_matGroundRectView;
	D3DXMATRIX m_matGroundRectViewProj;

	CMyCamera* m_pCamera;
	CMySound* m_pSound;
	CCenterObj* m_pCenterObj;
	CGroundRect* m_pGroundRect;

	LPD3DXEFFECT m_pShader[SHADER_TPYE_END];
	std::list<CRenderObj*> m_particleList;

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;

	LPDIRECT3DTEXTURE9	m_pGroundRectTarget;
	LPDIRECT3DSURFACE9	m_pGroundRectSurface;
	LPDIRECT3DSURFACE9	m_pOriBackBuffer;
};

