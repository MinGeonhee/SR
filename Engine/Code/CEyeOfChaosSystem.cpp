#include "CEyeOfChaosSystem.h"
#include "CEventCube.h"
#include "CEventBus.h"
#include "CEyeOfChaos.h"
#include "CEventObject.h"
#include "CCollisionManager.h"
#include "CUIManager.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CEyeOfChaosSystem)


CEyeOfChaosSystem::CEyeOfChaosSystem()
{
}

CEyeOfChaosSystem::~CEyeOfChaosSystem()
{
	Free();
}

_int CEyeOfChaosSystem::System_Update(const float& fTimeDelta)
{
	return 0;
}

void CEyeOfChaosSystem::System_LateUpdate(const float& fTimeDelta)
{
	Apply_EventBus();
}

void CEyeOfChaosSystem::Apply_EventBus()
{
	EVENT event, Trash;

	auto bus = CEventBus::GetInstance();

	if (bus->Subscribe(CHANNEL::EYEOFCHAOS, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		// 나중에 부셔지는 타입이 추가된다면 부셔지는건 E로 상호작용을 하지않는다.

		CCollisionManager::GetInstance()->FireRaycast(RAY_TYPE::EYEOFCHAOS_RAY, OBJTYPE::OBJ_MAPEVENT);
	}


	if (bus->Subscribe(CHANNEL::UI, TOPIC::STAGE_EXIT, PHASE::PHASE_BEGIN, Trash, false))
	{
		// 레이를 발사했을때 충돌되어이쏙 Global 이벤트로 E상호작용 키를 눌렀을 떄 플레이어와 상호작용한상태
		if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Trash, false))
		{
			EyeOfChaos_Event(event.targetPtr);
		}
	}


}

void CEyeOfChaosSystem::EyeOfChaos_Event(CGameObject* pGameObject)
{
	if (pGameObject->Get_OBJTYPE() != OBJTYPE::OBJ_MAPEVENT) return;
	if (static_cast<CEventObject*>(pGameObject)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::CHAOSOFEYE) return;


	CEyeOfChaos* pChaos = static_cast<CEyeOfChaos*>(pGameObject);
	CSoundManager::GetInstance()->SoundPlay(L"chaos-sac-explode.wav", EVENT_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
	CUIManager::GetInstance()->SetCategoryTag(L"PlayerTex", L"Clear");

	pChaos->EyeEvent();
}

void CEyeOfChaosSystem::Free()
{

}
