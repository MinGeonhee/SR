#pragma once
#include "CBase.h"
#include "CFrame.h"

BEGIN(Engine)

class ENGINE_DLL CFrameManager : public CBase
{
	DECLARE_SINGLETON(CFrameManager)

private:
	explicit CFrameManager();
	virtual ~CFrameManager();

public:
	_bool	IsPermit_Call(const wstring& pFrameTag, const _float& fTimeDelta);
	HRESULT Ready_Frame(const wstring& pFrameTag, const _float& fCallLimit);

private:
	CFrame* Find_Frame(const wstring& pFrameTag);

private:
	map<wstring, CFrame*>			m_mapFrame;

public:
	virtual void	Free();
};

END