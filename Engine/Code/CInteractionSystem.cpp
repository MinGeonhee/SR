#include "CInteractionSystem.h"
#include "CInteractable.h"
#include "CStatus.h"
#include "CEventBus.h"
#include "CItem.h"
#include "CCollisionManager.h"
#include "CObjectPoolingManager.h"
#include "CUIManager.h"
#include "CObjectManager.h"
#include "CParticleSystem_Eat.h"
#include "CEventCube.h"
#include "CSoundManager.h"
IMPLEMENT_SINGLETON(CInteractionSystem)

CInteractionSystem::CInteractionSystem()
{
}

CInteractionSystem::~CInteractionSystem()
{
	Free();
}

_int CInteractionSystem::System_Update(const float& fTimeDelta)
{
	return 0;
}

void CInteractionSystem::System_LateUpdate(const float& fTimeDelta)
{
	// 단순 Interaction 타입과 sphere 충돌처리로 들어온 애들일 거다.

	InteractionCollisionEvent();

	return;
}

void CInteractionSystem::InteractionCollisionEvent()
{
	auto bus = CEventBus::GetInstance();

	EVENT	event , trash;

	if (bus->Subscribe(CHANNEL::INTERACTION, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		if (!static_cast<CInteractable*>(event.targetPtr)->Get_IsCanInteraction())
			return;

		FireInterationRayCast();
	}


	if (bus->Subscribe(CHANNEL::UI, TOPIC::INTERACTNOTICE, PHASE::PHASE_BEGIN, trash, false))
	{
		INTERACT_TYPE Type = static_cast<CInteractable*>(trash.targetPtr)->GetInteractType();

		switch (Type)
		{
			//EAT
		case Engine::INTERACT_TYPE::BEERTABLE:
		case Engine::INTERACT_TYPE::CHICKENTABLE:
		case Engine::INTERACT_TYPE::FLOWERBUSH:
		case Engine::INTERACT_TYPE::WELL:
			CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Heal");
			InteractionEatEvent(event);

			break;
			// USE
		case Engine::INTERACT_TYPE::SWITCH:
		case Engine::INTERACT_TYPE::PHOTOSWITCH:
		case Engine::INTERACT_TYPE::SKULLSWITCH:
			CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Trigger");
			InteractionUseEvent(event);

			break;
			//Search
		case Engine::INTERACT_TYPE::CORPSE:
		case Engine::INTERACT_TYPE::CHEST:
		case Engine::INTERACT_TYPE::GOLDCHEST:
		case Engine::INTERACT_TYPE::WALLCHEST:
			CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Dead Body");
			InteractionSearchEvent(event);

			break;

		case Engine::INTERACT_TYPE::ORB:
			CSoundManager::GetInstance()->SoundPlay(L"magic-orb-explode.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		default:
			return;
		}

		event.phase = PHASE::PHASE_BEGIN;
		event.topic = TOPIC::INTERACTNOTICE;
		event.channel = CHANNEL::UI;

		CEventBus::GetInstance()->publish(event);
		return;
	}

}

void CInteractionSystem::InteractionEatEvent(EVENT& event)
{
	auto bus = CEventBus::GetInstance();

	EVENT Trash;

	if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Trash, false))
	{
		if (!event.sourcePtr || !event.targetPtr) return;

		if (event.sourcePtr->Get_OBJTYPE() != OBJTYPE::OBJ_PLAYER)
			return;

		//단순 플레이어와 상호작용이 가능한 애들로 들어왔을 거다... 아마도?  _right -> CInteraction 클래스 일거다..
		if (event.targetHandle.m_eType != OBJ_MAPEVENT)
			return;

		if (static_cast<CEventObject*>(event.targetPtr)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT)
			return;

		CInteractable* pInteraction = static_cast<CInteractable*>(event.targetPtr);
		INTERACT_TYPE Type = static_cast<CInteractable*>(event.targetPtr)->GetInteractType();

		auto pStatus = event.sourcePtr->Get_Status();
		auto Stat = pStatus->Get_Status();
		if (Stat.fMaxHp <= Stat.fCurrentHp)
		{
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::ONHEALEDFULL;
			event.channel = CHANNEL::UI;

			CEventBus::GetInstance()->publish(event);
			return;
		}

		{
			const wchar_t* soundKey = nullptr;

			if (Type == Engine::INTERACT_TYPE::BEERTABLE || Type == Engine::INTERACT_TYPE::WELL)  soundKey = L"get_drink.wav";
			if (Type == Engine::INTERACT_TYPE::CHICKENTABLE) soundKey = L"get_pig.wav";
			if (Type == Engine::INTERACT_TYPE::FLOWERBUSH)   soundKey = L"get_berry.wav";
			if (soundKey)
				CSoundManager::GetInstance()->SoundPlay(soundKey, ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

			CUIManager::GetInstance()->SetCategoryTag(L"PlayerStatPanel_UI", L"Eat_Effect");
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Eat_Effect;
			event.channel = CHANNEL::UI;

			CEventBus::GetInstance()->publish(event);
		}

		Play_Particle_Eat(pInteraction);

		pStatus->OnHealed(pInteraction->Get_HeadlAmount());
		//스프라이트 교체 해주고
		if (pInteraction->GetInteractType() == INTERACT_TYPE::FLOWERBUSH)
		{
			pInteraction->Set_InteractType(INTERACT_TYPE::BUSH);
			pInteraction->Setting_Sprite();
			pInteraction->Change_UVBuffer(pInteraction->GetFirstUVRect());
		}
		else {
			pInteraction->Change_SpriteToLast_ByIneraction();
		}
		//이제 상호작용 못하게 막아버리기..
		pInteraction->Set_IsCanInteraction(false); //아마콜라이더를 막아줘도? 그럼안돼..아마도 부셔질수도있는애들이있어서 콜라이더는 켜줘야해
	}
}

void CInteractionSystem::InteractionUseEvent(EVENT& event)
{
	auto bus = CEventBus::GetInstance();

	EVENT Trash;


	if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Trash, false))
	{
		if (!event.sourcePtr || !event.targetPtr) return;
		if (event.targetHandle.m_eType != OBJ_MAPEVENT) return;
		if (static_cast<CEventObject*>(event.targetPtr)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT)	return;

		CInteractable* pInteraction = static_cast<CInteractable*>(event.targetPtr);
		INTERACT_TYPE Type = pInteraction->GetInteractType();
		pInteraction->Change_SpriteToLast_ByIneraction();

		switch (Type)
		{
			case INTERACT_TYPE::SWITCH:
			case INTERACT_TYPE::PHOTOSWITCH:
			case INTERACT_TYPE::SKULLSWITCH:
				auto Index = event.targetPtr->Get_HandleEventIndex() + 1000; // 400 
				auto object = CObjectManager::GetInstance()->Get_EventIndex_Object(OBJTYPE::OBJ_DOOR, Index);	if (!object) return;
				auto objectDoor = object->Get_Component(L"Com_Door", ID_DYNAMIC);	if (!objectDoor) return;
				auto DoorComponent = static_cast<CDoor_Component*>(objectDoor);
				// 스위치 카운트를 내리고.
				CUIManager::GetInstance()->SetCategoryTag(L"UI_NoticeBanner", L"Door_Open");
				CUIManager::GetInstance()->ActivateUI(L"UI_NoticeBanner");

				CSoundManager::GetInstance()->SoundPlay(L"switch-on.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
				DoorComponent->Decrease_Switchcount();
				if (DoorComponent->Get_SwitchCount() <= 0) DoorComponent->Set_DoorState(DOORSTATE::DOOR_OPEN);
		}

	}
}

void CInteractionSystem::InteractionSearchEvent(EVENT& event)
{
	auto bus = CEventBus::GetInstance();

	EVENT Trash;

	if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Trash, false))
	{
		if (event.targetHandle.m_eType != OBJ_MAPEVENT)																	return;
		if (static_cast<CEventObject*>(event.targetPtr)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT)			return;

		CInteractable* pInterest = static_cast<CInteractable*>(event.targetPtr);

		CInteractable* pInteraction = static_cast<CInteractable*>(event.targetPtr);
		INTERACT_TYPE Type = pInteraction->GetInteractType();
		auto eventIndex = pInteraction->Get_HandleEventIndex();

		if (Type == INTERACT_TYPE::CHEST && eventIndex == 77) SpawnMonsterGimmik(event);	// 몬스터 스폰
		else if ((Type == INTERACT_TYPE::CHEST || Type == INTERACT_TYPE::GOLDCHEST )&& (401 <= eventIndex && eventIndex <= 500))
		{
			SpawnMonsterAndBOOMGimmik(event);	// 몬스터 스폰
		}
		else InteractionDropItem(pInterest,pInteraction->Get_DropItemAmount());

		const wchar_t* soundKey = nullptr;

		if (Type == Engine::INTERACT_TYPE::CORPSE) soundKey = L"open_corpse.wav";
		if (Type == Engine::INTERACT_TYPE::CHEST || Type == Engine::INTERACT_TYPE::WALLCHEST) soundKey = L"open_chest.wav";
		if (Type == Engine::INTERACT_TYPE::GOLDCHEST) soundKey = L"open_chest_gold.wav";

		if (soundKey)
			CSoundManager::GetInstance()->SoundPlay(soundKey, ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		if (Type == Engine::INTERACT_TYPE::CORPSE)
		{
			pInteraction->Set_IsCanInteraction(false);
			pInteraction->Remove_Particle_Sparkle();
		}

		else
		{
			pInteraction->Change_SpriteToLast_ByIneraction();
			pInteraction->Get_Collide()->Set_ColliderType(COLLIDER_NONE);
			pInteraction->Set_IsCanInteraction(false);
		}
	}
}



void CInteractionSystem::FireInterationRayCast()
{
	CCollisionManager::GetInstance()->FireRaycast(RAY_TYPE::INTERACTION_RAY, OBJTYPE::OBJ_MAPEVENT);
}

void CInteractionSystem::InteractionDropItem(CGameObject* Object , const _int& DropAmount)
{
	if (Object == nullptr) return;
	if (DropAmount == 0) return;
	if (Object->Get_Transform() == nullptr) return;

	auto Transform = Object->Get_Transform();

	for (int i = 0; i < DropAmount; ++i)
	{
		CItem* pItem = static_cast<CItem*>(CObjectPoolingManager::GetInstance()->Get(OBJ_ITEM));
		pItem->Set_IsDropItem(true);
		pItem->Set_TargetTransform(Transform);
	}

	return;
}

void CInteractionSystem::SpawnMonsterGimmik(EVENT& event)
{
	EVENT gimmik;
	gimmik.channel = CHANNEL::Monster;
	gimmik.phase = PHASE::PHASE_BEGIN;
	gimmik.topic = TOPIC::MONSTERSPAWN;
	gimmik.sourcePtr = event.targetPtr; // 상자가 주체가 되니까.
	gimmik.targetPtr = nullptr;
	gimmik.sourceHandle = event.targetHandle;
	gimmik.targetHandle = {};
	CEventBus::GetInstance()->publish(gimmik);
}

void CInteractionSystem::SpawnMonsterAndBOOMGimmik(EVENT& event)
{

	vector<CGameObject*> EventCubeVec = CObjectManager::GetInstance()->Get_EventIndex_ObjectVec(OBJTYPE::OBJ_EVENTCUBE, event.targetHandle.m_sEventIndex);


	bool isPlay = false;

	for (auto EventCube : EventCubeVec)
	{

		if (event.targetHandle.m_sEventIndex == EventCube->Get_HandleEventIndex())
		{
			CEventCube* pEventCube = static_cast<CEventCube*>(EventCube);
			if (pEventCube->GetCubeEventType() == CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC)
			{
				pEventCube->PlayEvent();

				{
					pEventCube->Play_BrokenCubeParticleSystem();
				}
			}
		}
	}
	

		

	auto bus = CEventBus::GetInstance();




	EVENT gimmik;
	gimmik.channel = CHANNEL::Monster;
	gimmik.phase = PHASE::PHASE_BEGIN;
	gimmik.topic = TOPIC::MONSTERSPAWN;
	gimmik.sourcePtr = event.targetPtr; // 상자가 주체가 되니까.
	gimmik.targetPtr = nullptr;
	gimmik.sourceHandle = event.targetHandle;
	gimmik.targetHandle = {};
	CEventBus::GetInstance()->publish(gimmik);


}


void CInteractionSystem::Play_Particle_Eat(CGameObject* pInteraction)
{
	CParticleSystem_Eat* p;
	p = dynamic_cast<CParticleSystem_Eat*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_EAT));
	p->Set_Target(pInteraction);
	p->Activate_ParticleSystem();
	p->Make_And_Apply_Values_To_Particle();
}


void	CInteractionSystem::Free()
{

}