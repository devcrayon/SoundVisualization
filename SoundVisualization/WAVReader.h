#ifndef __WAV_READER_H__
#define __WAV_READER_H__ 0
#include "MyObj.h"

#define Byte4 unsigned int
#define Byte2 unsigned short
#define Byte unsigned char

inline void LittleToBigEndian(Byte4& num)
{
	Byte4 tmpNum = num;
	num = ((tmpNum >> 24) & 0x000000ff)
			| ((tmpNum >> 8) & 0x0000ff00)
			| ((tmpNum << 8) & 0x00ff0000)
			| ((tmpNum << 24) & 0xff000000);
}
inline void LittleToBigEndian(Byte2& num)
{
	Byte2 tmpNum = num;
	num = ((tmpNum >> 8) & 0x00ff) | ((tmpNum << 8) & 0xff00);
}
inline void ConsoleTextOut(Byte4& num)
{
	char cArr[5] = { 0 };
	memcpy(&cArr, &num, sizeof(num));
	std::cout << cArr << std::endl;
}
inline void ConsoleTextOut(Byte2& num)
{
	char cArr[3] = { 0 };
	memcpy(&cArr, &num, sizeof(num));
	std::cout << cArr << std::endl;
}

typedef struct _tagWAVHeader
{
	
	_tagWAVHeader()
	{
		memset(this, 0, sizeof(_tagWAVHeader));
	}
	~_tagWAVHeader()
	{
	}
	bool ReadRIFF(FILE* pF)
	{
		fread(RIFF_ChunkID, 1, sizeof(RIFF_ChunkID), pF);
		fread(&RIFF_ChunkSize, sizeof(RIFF_ChunkSize), 1, pF);
		fread(RIFF_Format, 1, sizeof(RIFF_Format), pF);
		//::LittleToBigEndian(RIFF_ChunkSize);

		return true;
	}
	bool ReadFMT(FILE* pF)
	{
		fread(FMT_ChunkID, 1, sizeof(FMT_ChunkID), pF);
		fread(&FMT_ChunkSize, sizeof(FMT_ChunkSize), 1, pF);
		fread(&FMT_AudioFormat, sizeof(FMT_AudioFormat), 1, pF);
		fread(&FMT_NumChannels, sizeof(FMT_NumChannels), 1, pF);
		fread(&FMT_SampleRate, sizeof(FMT_SampleRate), 1, pF);
		fread(&FMT_ByteRate, sizeof(FMT_ByteRate), 1, pF);
		fread(&FMT_BlockAlign, sizeof(FMT_BlockAlign), 1, pF);
		fread(&FMT_BitsPerSample, sizeof(FMT_BitsPerSample), 1, pF);
		//::LittleToBigEndian(FMT_ChunkSize);
		//::LittleToBigEndian(FMT_AudioFormat);
		//::LittleToBigEndian(FMT_NumChannels);
		//::LittleToBigEndian(FMT_SampleRate);
		//::LittleToBigEndian(FMT_ByteRate);
		//::LittleToBigEndian(FMT_BlockAlign);
		//::LittleToBigEndian(FMT_BitsPerSample);
		return true;
	}
	bool ReadData(FILE* pF)
	{
		bool bIsReadData = false;
		while (bIsReadData == false)
		{
			fread(&DATA_ChunkID[0], sizeof(char), 1, pF);
			while (this->DATA_ChunkID[0] != 'D' && this->DATA_ChunkID[0] != 'd')
			{
				fread(&DATA_ChunkID[0], 1, sizeof(char), pF);
			}
			fread(&DATA_ChunkID[1], sizeof(char), 1, pF);
			fread(&DATA_ChunkID[2], sizeof(char), 1, pF);
			fread(&DATA_ChunkID[3], sizeof(char), 1, pF);
			bIsReadData = (DATA_ChunkID[0] == 'D' || DATA_ChunkID[0] == 'd')
				& (DATA_ChunkID[1] == 'A' || DATA_ChunkID[1] == 'a')
				& (DATA_ChunkID[2] == 'T' || DATA_ChunkID[2] == 't')
				& (DATA_ChunkID[3] == 'A' || DATA_ChunkID[3] == 'a');
		}
		//fread(&DATA_ChunkID, 1, sizeof(DATA_ChunkID), pF);
		fread(&DATA_ChunkSize, sizeof(DATA_ChunkSize), 1, pF);
		//::LittleToBigEndian(DATA_ChunkSize);
		return true;
	}
	
	//RIFF : Marks the fiile as a RIFF
	//Byte4 RIFF_ChunkID;
	char RIFF_ChunkID[4];
	Byte4 RIFF_ChunkSize; //Little Endian
	//Byte4 RIFF_Format;
	char RIFF_Format[4];
	//FMT : Format Chunk Marker
	//Byte4 FMT_ChunkID;
	char FMT_ChunkID[4];
	Byte4 FMT_ChunkSize; //Little Endian
	Byte2 FMT_AudioFormat; //Little Endian
	Byte2 FMT_NumChannels; //Little Endian
	Byte4 FMT_SampleRate; //Little Endian
	Byte4 FMT_ByteRate; //Little Endian
	Byte2 FMT_BlockAlign; //Little Endian
	Byte2 FMT_BitsPerSample; //Little Endian
	//DATA : Data Chunk
	//Byte4 DATA_ChunkID;
	char DATA_ChunkID[4];
	Byte4 DATA_ChunkSize; //Little Endian
}WAVHeader;
#undef Byte4
#undef Byte2
#undef Byte

class WAVReader
	: public CMyObj
{
private:
	explicit WAVReader();
	explicit WAVReader(const WAVReader& rObj);
	WAVReader& operator = (const WAVReader& rObj);
	~WAVReader();

public:
	static WAVReader* Create(std::wstring wstrFilePath);
	bool SetFile(std::wstring wstrFilePath);
	const WAVHeader& GetWavHeader() const;
	const char* GetLowData() const;
	const unsigned short* GetSoundData() const;
	const int GetSize();

private:
	void DataToNumber();
	virtual void Free();
private:
	std::wstring m_wstrFileName;
	WAVHeader m_wavHeader;
	char* m_pLowData;
	unsigned short* m_pFirstSoundData;
	int m_iSize;
};


#endif // !__WAV_READER_H__

