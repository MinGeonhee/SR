#include "CActionCamera_Helper.h"
#include "CCameraManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CTimeManager.h"
#include "CEventBus.h"
#include "CCamera.h"


void CActionCamera_Helper::PlayDeathSequence(CGameObject* object, ObjectHandle TargetHandle)
{
	// 2. 카메라 목표 위치로 이동
	// 1. 시간 0.3초 정지
	// 3. 시간 1초동안 느리게 (0.2배속)

	if (object == NULL) return;

	if (TargetHandle.m_eOriginType == ORIGIN_DYNAMIC) object = CObjectPoolingManager::GetInstance()->Resolve(TargetHandle);
	else if (TargetHandle.m_eOriginType == ORIGIN_DYNAMIC && object == NULL) return;

	EVENT event1 = { PHASE::PHASE_BEGIN, TOPIC::DeathSequenceCAM, CHANNEL::Camera, object, nullptr, TargetHandle, {} };
	CEventBus::GetInstance()->publish(event1);

	EVENT event2 = { PHASE::PHASE_UPDATE, TOPIC::DeathSequenceCAM, CHANNEL::Camera, object, nullptr, TargetHandle, {} };
	CTimeManager::GetInstance()->TimeEventJob_Resist(
		event2,
		TimeType::TIME_STOP,
		/*Duration*/1.f, 
		/*TimeScale*/0.0f);

	EVENT event3 = { PHASE::PHASE_END, TOPIC::DeathSequenceCAM, CHANNEL::Camera, object, nullptr, TargetHandle, {} };
	CTimeManager::GetInstance()->TimeEventJob_Resist(
		event3,
		TimeType::TIME_SLOW,
		5.f,
		0.3f);
}

void CActionCamera_Helper::StartIslandTour(CGameObject* object, ObjectHandle TargetHandle)
{
	if (object == NULL) return;

	if (TargetHandle.m_eOriginType == ORIGIN_DYNAMIC) object = CObjectPoolingManager::GetInstance()->Resolve(TargetHandle);
	else if (TargetHandle.m_eOriginType == ORIGIN_DYNAMIC && object == NULL) return;

	// 아일랜드 투어 첫 시작은 Orbital Camera
	EVENT event1 = { PHASE::PHASE_BEGIN, TOPIC::IslandTourCam, CHANNEL::Camera, object, nullptr, TargetHandle, {} };
	CTimeManager::GetInstance()->TimeEventJob_Resist(
		event1,
		TimeType::TIME_NONE,
		/*Duration*/10.f, 
		/*TimeScale*/1.f);

	// Zoom out Camera
	EVENT event2 = { PHASE::PHASE_UPDATE, TOPIC::IslandTourCam, CHANNEL::Camera, object, nullptr, TargetHandle, {} };
	CTimeManager::GetInstance()->TimeEventJob_Resist(
		event2,
		TimeType::TIME_NONE,
		/*Duration*/10.f,
		/*TimeScale*/1.f);
}
