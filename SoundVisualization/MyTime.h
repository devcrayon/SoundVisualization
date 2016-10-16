#pragma once
#include "MyObj.h"
class CMyTime :
	public CMyObj
{
private:
	CMyTime();
	virtual ~CMyTime();
public:
	static void InitTime();
	static void SetTime();

public:
	static FLOAT deltaTime;
private:
	static LARGE_INTEGER frameTime;
	static LARGE_INTEGER fixTime;
	static LARGE_INTEGER lastTime;
	static LARGE_INTEGER cpuTick;
};

