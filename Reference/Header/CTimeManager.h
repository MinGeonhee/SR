#pragma once

#include "CBase.h"
#include "CTime.h"
#include "CEventBus.h"

BEGIN(Engine)

enum TimeType { TIME_NONE, TIME_SLOW, TIME_FAST, TIME_STOP, TIME_END };

struct TimeEventSlot
{
	EVENT						event;
	TimeType					CurTimeType = TimeType::TIME_NONE;
	float						Duration = 0.f;
	float						TimeScale = 1.f;
	bool						JobActive = false;
};


class ENGINE_DLL CTimeManager: public CBase
{
	DECLARE_SINGLETON(CTimeManager)

private:
	explicit CTimeManager();
	virtual ~CTimeManager();

public:

	_float			Get_TimeDelta(const wstring& pTimerTag);
	void			Set_TimeDelta(const wstring& pTimerTag);

public:
	HRESULT			Ready_Timer(const wstring& pTimerTag);
	void			The_World(bool type) { m_bTheWorld = type; }

	// 시간 값에 따른 이벤트를 발송하고 싶다. 임시로 여기에 둠.
	void			TimeEventJob_Resist(EVENT event, TimeType type, float fduration, float fSlow) 
	{
		TimeEventSlot slot = { event, type, fduration, fSlow , true};
		m_qTimeJobs.push(slot);
	}

	bool			Get_TimeType() { return m_tSlot.CurTimeType;}
private:
	CTime*			Find_Timer(const wstring& pTimerTag);

private:
	map<wstring, CTime*>		m_mapTimer;
	queue<TimeEventSlot>		m_qTimeJobs;
	TimeEventSlot				m_tSlot;

	bool						m_bTheWorld = false;

	float						m_fElapsedTime = 0.f;

private:
	virtual void		Free();
};

END