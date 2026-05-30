#include "CTimeManager.h"

IMPLEMENT_SINGLETON(CTimeManager)

CTimeManager::CTimeManager()
{
}

CTimeManager::~CTimeManager()
{
	Free();
}

_float CTimeManager::Get_TimeDelta(const wstring& pTimerTag)
{
	CTime* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	if (m_qTimeJobs.size() != 0 && !m_tSlot.JobActive)
	{ m_tSlot = m_qTimeJobs.front(); m_qTimeJobs.pop();}

	if (m_tSlot.JobActive) 
	{ 
		m_fElapsedTime += pTimer->Get_TimeDelta();

		if (m_fElapsedTime >= m_tSlot.Duration)
		{
			CEventBus::GetInstance()->publish(m_tSlot.event);
			m_tSlot.JobActive = false;
			m_tSlot.TimeScale = 1.f;
			m_tSlot.Duration = 0.f;
			m_tSlot.event = {};
			m_fElapsedTime = 0.f;
		}
	}

	if (!m_bTheWorld)
		return pTimer->Get_TimeDelta() * m_tSlot.TimeScale;

	else
		return 0;
}

void CTimeManager::Set_TimeDelta(const wstring& pTimerTag)
{
	CTime* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

HRESULT CTimeManager::Ready_Timer(const wstring& pTimerTag)
{
	CTime* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTime::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimer.insert({ pTimerTag, pTimer });

	return S_OK;
}

CTime* CTimeManager::Find_Timer(const wstring& pTimerTag)
{
	auto		iter = find_if(m_mapTimer.begin(), m_mapTimer.end(),
		[&pTimerTag](const auto& pair) {return pTimerTag == pair.first; });

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void CTimeManager::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}
