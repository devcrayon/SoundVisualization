#include "stdafx.h"
#include "WAVReader.h"



WAVReader::WAVReader()
	: m_pLowData(NULL)
	, m_pFirstSoundData(NULL)
{
}

WAVReader::WAVReader(const WAVReader & rObj)
{
}

WAVReader & WAVReader::operator=(const WAVReader & rObj)
{
	// TODO: 여기에 반환 구문을 삽입합니다
	return *this;
}


WAVReader::~WAVReader()
{
}

WAVReader * WAVReader::Create(std::wstring wstrFilePath)
{
	WAVReader* pWavReader = new WAVReader();
	if (pWavReader->SetFile(wstrFilePath))
	{
		return pWavReader;
	}
	delete pWavReader;
	return NULL;
}

bool WAVReader::SetFile(std::wstring wstrFilePath)
{
	FILE* pF = NULL;
	_wfopen_s(&pF, wstrFilePath.c_str(), L"rb");

	this->m_wavHeader.ReadRIFF(pF);
	this->m_wavHeader.ReadFMT(pF);
	this->m_wavHeader.ReadData(pF);

	std::cout << "DATA_ChunkSize : " << this->m_wavHeader.DATA_ChunkSize << std::endl;
	int iSize = 0;
	iSize = this->m_wavHeader.DATA_ChunkSize
		/ (this->m_wavHeader.FMT_BlockAlign);
	std::cout << iSize << std::endl;
	iSize = this->m_wavHeader.DATA_ChunkSize
		/ (this->m_wavHeader.FMT_ByteRate);
	std::cout << iSize << std::endl;
	this->m_pLowData = new char[this->m_wavHeader.DATA_ChunkSize];
	fread(this->m_pLowData, sizeof(char), this->m_wavHeader.DATA_ChunkSize, pF);
	fclose(pF);
	if (this->m_wavHeader.FMT_BitsPerSample != 16)
	{
		return false;
	}
	this->DataToNumber();
	this->m_wstrFileName = wstrFilePath;
	return true;
}

const WAVHeader & WAVReader::GetWavHeader() const
{
	return this->m_wavHeader;
}

const char * WAVReader::GetLowData() const
{
	return this->m_pLowData;
}

const unsigned short * WAVReader::GetSoundData() const
{
	return this->m_pFirstSoundData;
}

const int WAVReader::GetSize()
{
	return this->m_iSize;
}

void WAVReader::DataToNumber()
{
	this->m_iSize = this->m_wavHeader.DATA_ChunkSize
		/ (this->m_wavHeader.FMT_BlockAlign);

	this->m_pFirstSoundData = new unsigned short[this->m_iSize];
	char* pData = this->m_pLowData;
	int iSampleSize = this->m_wavHeader.FMT_SampleRate >> 3;
	for (int i = 0; i < this->m_iSize; ++i)
	{
		memcpy(&this->m_pFirstSoundData[i], pData, sizeof(short));
		pData += this->m_wavHeader.FMT_BlockAlign;
	}
	int aa = 0;
}

void WAVReader::Free()
{
	if (this->m_pLowData != NULL)
		delete[] this->m_pLowData;
	if (this->m_pFirstSoundData != NULL)
		delete[] this->m_pFirstSoundData;
}
