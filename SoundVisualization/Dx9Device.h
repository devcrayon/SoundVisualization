#ifndef  __DX9DEVICE_H__
#define __DX9DEVICE_H__ 0
#include "MyObj.h"

class CDx9Device :
	public CMyObj
{
private:
	explicit CDx9Device();
	explicit CDx9Device(const CDx9Device& rDevice);
	CDx9Device& operator = (const CDx9Device& rDevice);
	virtual ~CDx9Device();

public:
	static CDx9Device* GetInstance();
	static unsigned long DestroyInstance();
	static bool LoadShader(TCHAR* pFileName, LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT& pEffect);
	HRESULT Initialize(HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	inline LPDIRECT3DDEVICE9 GetDevice();
	void ReSize(const WORD& wSizeX, const WORD& wSizeY, bool bDeviceLost);

protected:
	virtual void Free();
	
private:
	static CDx9Device* pInstance;
	bool m_bIsInit;
	LPDIRECT3D9 m_p3D;
	LPDIRECT3DDEVICE9 m_pDevice;	
};


inline LPDIRECT3DDEVICE9 CDx9Device::GetDevice()
{
	return this->m_pDevice;
}

#endif // __DX9DEVICE_H__

