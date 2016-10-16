#pragma once
#include "MyObj.h"
#include "WAVReader.h"
class CMySound :
	public CMyObj
{
public:
	enum eState
	{
		STATE_PLAY = 0,
		STATE_PAUSE,
		STATE_STOP,
		STATE_EMPTY,
	};
protected:
	explicit CMySound();
	explicit CMySound(const CMySound& rSound);
	CMySound& operator = (const CMySound& rSound);
	virtual ~CMySound();

public:
	static CMySound* Create(UINT iDeviceID);
	bool SetFile(std::wstring wstrFilePath);
	int GetData(int iIndex);
	int GetIndex();
	void Play();
	void Puase();
	void Stop();
	void Seek(FLOAT fTimePercent);
	void DeleteData();
	inline void SetDeviceID(UINT iDeviceID);

private:
	virtual void Free();


protected:
	eState m_eIsState;
	WAVReader* m_pWavReader;
	DWORD m_iLength;
	UINT m_iDeviceID;
	MCI_OPEN_PARMS m_mciOpenParms; //������ �ε�
	MCI_PLAY_PARMS m_mciPlayParms; //������ ���
	MCI_STATUS_PARMS m_mciStatus; //������ ����
	MCI_SEEK_PARMS m_mciSeekParams;

};


inline void CMySound::SetDeviceID(UINT iDeviceID)
{
	this->m_iDeviceID = iDeviceID;
}