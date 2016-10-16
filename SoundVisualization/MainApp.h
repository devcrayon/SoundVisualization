#pragma once
#include "MyObj.h"
#include "Dx9Device.h"
#include "MainScene.h"
class CMainApp :
	public CMyObj
{
private:
	explicit CMainApp();
	explicit CMainApp(const CMainApp& rApp);
	CMainApp& operator = (const CMainApp& rApp);
	virtual ~CMainApp();

public:
	static CMainApp* Create();
	HRESULT Initialize();
	int Update();
	void Render();
	void OnResetDeviceDevice();
	void SetSound(std::wstring wstrFilePath);
	void StopSound();
	void SetColor(int iR, int iG, int iB);

protected:
	virtual void Free();

private:
	bool m_bIsRestoring;
	bool m_bIsLostDevice;
	LPDIRECT3DDEVICE9 m_pDevice;
	CMainScene* m_pMainScene;
};

