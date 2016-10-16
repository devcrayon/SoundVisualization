#ifndef __MY_OBJ_H__
#define __MY_OBJ_H__ 0

class CMyObj
{
protected:
	inline explicit CMyObj(void);
	inline virtual ~CMyObj(void);

public:
	inline virtual unsigned long AddRef();
	inline virtual unsigned long Release();

protected:
	virtual void Free() = 0;

protected:
	unsigned long	m_dwRefCnt;
};

CMyObj::CMyObj(void)
	: m_dwRefCnt(0)
{

}

CMyObj::~CMyObj(void)
{

}

unsigned long CMyObj::AddRef(void)
{
	return ++this->m_dwRefCnt;
}

unsigned long CMyObj::Release(void)
{
	if (0 == this->m_dwRefCnt)
	{
		Free();
		delete this;
		return 0;
	}
	else
		return this->m_dwRefCnt--;
}

#endif //__MY_OBJ_H__
