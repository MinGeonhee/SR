#include "CTimeSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCameraManager.h"
#include "CCamera.h"
#include "CTimeManager.h"

IMPLEMENT_SINGLETON(CTimeSystem)

CTimeSystem::CTimeSystem()
{

}

CTimeSystem::~CTimeSystem()
{
	Free();
}

_int CTimeSystem::System_Update(const float& fTimeDelta)
{

	return 0;
}

void CTimeSystem::System_LateUpdate(const float& fTimeDelta)
{
	Time();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CTimeSystem::Time()	// Camera
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	// 전투가 일어나서 누군가가 데미지를 입었다면, 카메라 쉐이킹.
	if (bus->Subscribe(CHANNEL::Global, TOPIC::TIME_STOP, PHASE::PHASE_BEGIN, event, false))
	{
		CTimeManager::GetInstance()->The_World(true);
	}

	if (bus->Subscribe(CHANNEL::Global, TOPIC::TIME_RESUME, PHASE::PHASE_BEGIN, event, false))
	{
		CTimeManager::GetInstance()->The_World(false);
	}
}

void CTimeSystem::Free()
{

}