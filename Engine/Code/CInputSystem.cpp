#include "CInputSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCollisionManager.h"
#include "CDoor_Component.h"
#include "CInputManager.h"

IMPLEMENT_SINGLETON(CInputSystem)

CInputSystem::CInputSystem()
{

}

CInputSystem::~CInputSystem()
{
	Free();
}

_int CInputSystem::System_Update(const float& fTimeDelta)
{
	KeyInput_Publish();
	return 0;
}

void CInputSystem::System_LateUpdate(const float& fTimeDelta)
{

}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CInputSystem::KeyInput_Publish()	// 문이랑 충돌하면 UI 상호작용 출력되게 하는 함수
{
	auto input = CInputManager::GetInstance();
	auto ev = CEventBus::GetInstance();

	EVENT event;

	if (input->GetButtonDown(KEY_TYPE::E))
	{
		event.phase = PHASE::PHASE_BEGIN;
		event.topic = TOPIC::E_KEY;
		event.channel = CHANNEL::Global;

		ev->publish(event);
	}
}

void CInputSystem::Free()
{

}