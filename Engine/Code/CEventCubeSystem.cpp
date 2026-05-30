#include "CEventCubeSystem.h"
#include "CEventCube.h"
#include "CEventBus.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CEventCubeSystem)



CEventCubeSystem::CEventCubeSystem()
{
}

CEventCubeSystem::~CEventCubeSystem()
{
	Free();
}


_int		CEventCubeSystem::System_Update(const float& fTimeDelta)
{

	return 0;
}


void		CEventCubeSystem::System_LateUpdate(const float& fTimeDelta)
{
	ApplyEventCube_EventBus();
	return;
}


void CEventCubeSystem::ApplyEventCube_EventBus()
{

	EVENT event, Trash;

	auto bus = CEventBus::GetInstance();

 	if (bus->Subscribe(CHANNEL::EVENTCUBE, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		// 나중에 부셔지는 타입이 추가된다면 부셔지는건 E로 상호작용을 하지않는다.

		CCollisionManager::GetInstance()->FireRaycast(RAY_TYPE::EVENT_CUBE_RAY, OBJTYPE::OBJ_EVENTCUBE);
	}

	if (bus->Subscribe(CHANNEL::EVENTCUBE, TOPIC::RAYCOLLISION, PHASE::PHASE_BEGIN, Trash, false))
	{
		// 레이를 발사했을때 충돌되어이쏙 Global 이벤트로 E상호작용 키를 눌렀을 떄 플레이어와 상호작용한상태
		if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Trash, false))
		{
			CSoundManager::GetInstance()->SoundPlay(L"creature-walk-humanoid-light-armor.wav", EVENT_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 0.5);
			EventCube_InteractionEvent_ByPlayer(event.sourcePtr, event.targetPtr);
		}
	}

	if (bus->Subscribe(CHANNEL::EVENTCUBE, TOPIC::ONDAMAGED, PHASE::PHASE_BEGIN, event, false))
		EventCube_InteractionEvent_ByWeapon(event.sourcePtr, event.targetPtr);
}

void CEventCubeSystem::EventCube_InteractionEvent_ByPlayer(CGameObject* pSource, CGameObject* pTarget)
{
	auto Source_OBJTYPE = pSource->Get_OBJTYPE(); auto Target_OBJTYPE = pTarget->Get_OBJTYPE();

	//플레이어와 충돌처리를 할테니까
	CGameObject* pPlayerObj = nullptr;
	CEventCube* pEventCube = nullptr;

	if (Source_OBJTYPE == OBJTYPE::OBJ_PLAYER)                pPlayerObj = pSource;
	else if (Target_OBJTYPE == OBJTYPE::OBJ_PLAYER)           pPlayerObj = pTarget;
	else                                                      return;

	if (Source_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)             pEventCube = static_cast<CEventCube*>(pSource);
	else if (Target_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)        pEventCube = static_cast<CEventCube*>(pTarget);
	else                                                      return;


	pEventCube->PlayEvent();

}

void CEventCubeSystem::EventCube_InteractionEvent_ByWeapon(CGameObject* pSource, CGameObject* pTarget)
{
	auto Source_OBJTYPE = pSource->Get_OBJTYPE(); auto Target_OBJTYPE = pTarget->Get_OBJTYPE();

	//플레이어와 충돌처리를 할테니까
	CGameObject* pWeaponObj = nullptr;
	CEventCube* pEventCube = nullptr;

	//아마 메이슨이 플레이어 타입으로들어올예정
	if (Source_OBJTYPE == OBJTYPE::OBJ_PLAYER)                pWeaponObj = pSource;
	else if (Target_OBJTYPE == OBJTYPE::OBJ_PLAYER)           pWeaponObj = pTarget;
	else if (Source_OBJTYPE == OBJTYPE::OBJ_BULLET)           pWeaponObj = pSource;
	else if (Target_OBJTYPE == OBJTYPE::OBJ_BULLET)           pWeaponObj = pTarget;
	else												      return;

	
	if (Source_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)             pEventCube = static_cast<CEventCube*>(pSource);
	else if (Target_OBJTYPE == OBJTYPE::OBJ_EVENTCUBE)        pEventCube = static_cast<CEventCube*>(pTarget);
	else													  return;

	if (pEventCube->GetCubeEventType() != CUBE_EVENT_TYPE::DESTRUCTIBLE_ATTACK)	return;


	pEventCube->PlayEvent();

}

void	CEventCubeSystem::Free()
{
	
 }




