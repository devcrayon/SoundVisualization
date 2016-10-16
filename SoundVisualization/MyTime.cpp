#include "stdafx.h"
#include "MyTime.h"

FLOAT CMyTime::deltaTime = 0.0f;

LARGE_INTEGER CMyTime::frameTime;
LARGE_INTEGER CMyTime::fixTime;
LARGE_INTEGER CMyTime::lastTime;
LARGE_INTEGER CMyTime::cpuTick;

CMyTime::CMyTime()
{
}


CMyTime::~CMyTime()
{
}

void CMyTime::InitTime()
{
	QueryPerformanceCounter(&frameTime);
	QueryPerformanceCounter(&fixTime);
	QueryPerformanceCounter(&lastTime);
	QueryPerformanceFrequency(&cpuTick);
}

void CMyTime::SetTime()
{
	QueryPerformanceCounter(&frameTime);

	if (frameTime.QuadPart - lastTime.QuadPart > cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&cpuTick);

		lastTime.QuadPart = frameTime.QuadPart;
	}

	deltaTime = float(frameTime.QuadPart - fixTime.QuadPart) / cpuTick.QuadPart;

	fixTime = frameTime;
}
