#include "CTime.h"

CTime::CTime() : m_fTimeDelta(0.f)
{
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTime::~CTime()
{
}

HRESULT CTime::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);			// 1077
	QueryPerformanceCounter(&m_LastTime);			// 1085
	QueryPerformanceCounter(&m_FixTime);			// 1090

	QueryPerformanceFrequency(&m_CpuTick);		// cpu tick 값을 얻어오는 함수

	return S_OK;
}

void CTime::Update_Timer()
{
	//QueryPerformanceCounter(&m_FrameTime);			// 1500

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	// 실제 경과 시간(초)
	double dt = static_cast<double>(now.QuadPart - m_LastTime.QuadPart)
		/ static_cast<double>(m_CpuTick.QuadPart);

	// 드라이버 전환/창 이동 등으로 음수/이상치 방지(선택)
	if (dt < 0.0) dt = 0.0;
	// 필요하면 상한(예: 100ms)도 줄 수 있음: dt = min(dt, 0.1);

	m_fTimeDelta = static_cast<float>(dt);
	m_LastTime = now;
}

CTime* CTime::Create()
{
	CTime* pInstance = new CTime;

	if (FAILED(pInstance->Ready_Timer()))
	{
		Engine::Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTime::Free()
{
}



