#include "stdafx.h"
#include "MySound.h"


CMySound::CMySound()
	: m_pWavReader(null)
	, m_iLength(0)
	, m_eIsState(STATE_EMPTY)
	, m_iDeviceID(0)
{
	::ZeroMemory(&m_mciOpenParms, sizeof(MCI_OPEN_PARMS));
	::ZeroMemory(&m_mciPlayParms, sizeof(MCI_PLAY_PARMS));
	::ZeroMemory(&m_mciStatus, sizeof(MCI_STATUS_PARMS));
	::ZeroMemory(&m_mciSeekParams, sizeof(MCI_SEEK_PARMS));
}

CMySound::CMySound(const CMySound & rSound)
{
}

CMySound & CMySound::operator=(const CMySound & rSound)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return *this;
}


CMySound::~CMySound()
{
}

CMySound * CMySound::Create(UINT iDeviceID)
{
	CMySound* pSound = new CMySound();
	pSound->SetDeviceID(iDeviceID);
	return pSound;
}

bool CMySound::SetFile(std::wstring wstrFilePath)
{
	this->m_pWavReader = WAVReader::Create(wstrFilePath);
	if (this->m_pWavReader == null)
	{
		this->m_eIsState = STATE_EMPTY;
		MessageBox(::g_hWnd, TEXT("Open Error!! only 16bit"), TEXT("Error"), MB_OK);
		return false;
	}
	this->m_mciOpenParms.lpstrDeviceType = L"WaveAudio";

	//WaveAudio 대신 MPEGVideo를 사용하면 mp3 형식을 재생합니다.

	MCIERROR mciError;
	this->m_mciOpenParms.dwCallback = (DWORD)::g_hWnd;
	this->m_mciOpenParms.lpstrElementName = wstrFilePath.c_str();
	
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&this->m_mciOpenParms);
	if (mciError)
	{
		this->m_eIsState = STATE_EMPTY;
		MessageBox(::g_hWnd, TEXT("Open Error!!"), TEXT("Error"), MB_OK);
		return false;
	}
	this->m_iDeviceID = this->m_mciOpenParms.wDeviceID;

	this->m_mciPlayParms.dwCallback = (DWORD)::g_hWnd;

	MCI_STATUS_PARMS statusParms;
	statusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(this->m_iDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&statusParms);
	this->m_iLength = statusParms.dwReturn;
	this->m_eIsState = STATE_STOP;
	
	return true;
}

int CMySound::GetData(int iIndex)
{
  	if (iIndex > 10)
		return this->m_pWavReader->GetSoundData()[iIndex] - this->m_pWavReader->GetSoundData()[iIndex - 2];
	else
		return -1;
}

int CMySound::GetIndex()
{
	MCI_STATUS_PARMS StatusParms;
	DWORD Position;
	StatusParms.dwItem = MCI_STATUS_POSITION;
	mciSendCommand(this->m_iDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&StatusParms);
	Position = StatusParms.dwReturn;
	if (Position == this->m_iLength)
	{
		return -1;
	}
	FLOAT fRate = ((FLOAT)Position) / ((FLOAT)this->m_iLength);
	int iIndex = (int)(fRate * this->m_pWavReader->GetSize());
	return iIndex;
}

void CMySound::Play()
{
	if (this->m_eIsState == STATE_EMPTY)
		return;
	MCIERROR mciError;
	this->m_mciPlayParms.dwCallback = (DWORD)::g_hWnd;
	if (this->m_eIsState == STATE_PAUSE || this->m_eIsState == STATE_STOP)
	{
		mciError = mciSendCommand(this->m_iDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&this->m_mciPlayParms);
		if (mciError)
		{
			MessageBox(::g_hWnd, TEXT("Play Error!!"), TEXT("Error"), MB_OK);
			return;
		}
		this->m_eIsState = STATE_PLAY;
	}
}

void CMySound::Puase()
{
	if (this->m_eIsState == STATE_EMPTY)
		return;
	if (this->m_eIsState == STATE_PLAY)
	{
		mciSendCommand(this->m_iDeviceID, MCI_PAUSE, 0, NULL);
		this->m_eIsState = STATE_PAUSE;
	}
}

void CMySound::Stop()
{
	MCIERROR mciError;
	if (this->m_eIsState == STATE_EMPTY)
		return;
	if (this->m_eIsState == STATE_PAUSE || this->m_eIsState == STATE_PLAY)
	{
		this->m_eIsState = STATE_STOP;
		mciSendCommand(this->m_iDeviceID, MCI_CLOSE, 0, NULL);
	}
}

void CMySound::Seek(FLOAT fTimePercent)
{
	this->m_mciSeekParams.dwTo = this->m_iLength * fTimePercent;

	mciSendCommand(this->m_iDeviceID, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)(LPVOID)&this->m_mciSeekParams);
	mciSendCommand(this->m_iDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&this->m_mciPlayParms);
}

void CMySound::DeleteData()
{
	if (this->m_eIsState != STATE_STOP)
	{
		this->m_eIsState = STATE_STOP;
		mciSendCommand(this->m_iDeviceID, MCI_CLOSE, 0, NULL);
	}
}


void CMySound::Free()
{
	if (this->m_pWavReader != null)
	{
		this->m_pWavReader->Release();
	}
}
