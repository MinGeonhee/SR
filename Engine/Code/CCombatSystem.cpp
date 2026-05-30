#include "CCombatSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CStatus.h"
#include "CObjectPoolingManager.h"
#include "CInteractable.h"
#include "CMonster.h"
#include "CITem.h"
#include "CBullet.h"
#include "CEventCube.h"
#include "CParticleSystem_Bush.h"
#include "CEventObject.h"
#include "CParticleSystem_Barrel.h"
#include "CParticleSystem_Blood.h"
#include "CParticleSystem_Book.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CCombatSystem)

CCombatSystem::CCombatSystem()
{

}

CCombatSystem::~CCombatSystem()
{
	Free();
}

_int CCombatSystem::System_Update(const float& fTimeDelta)
{

	return 0;
}

void CCombatSystem::System_LateUpdate(const float& fTimeDelta)
{
	ApplyStatusChanges();	// 전투관련 처리

	ApplyStatusEffect();	// 상태이상 처리
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CCombatSystem::ApplyStatusChanges()	// 문이랑 충돌하면 UI 상호작용 출력되게 하는 함수
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	// 문 앞에 단순 충돌이라고 한다면.
	// 채널 : 전투 / 주제 : 데미지 입음. / 처리 시간 : Phase 중간.
	if(bus->Subscribe(CHANNEL::Combat, TOPIC::ONDAMAGED, PHASE::PHASE_BEGIN, event, false)) // 뒤에 true는 이벤트 읽자마자 삭제할건지.
	{
		// 여기서 미리 Origin_Dynamic 검사를 하자...
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.targetHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.targetPtr = CObjectPoolingManager::GetInstance()->Resolve(event.targetHandle);

		// 검사가 끝났다면.
		if (event.sourcePtr && event.targetPtr)
		{
			if (( event.sourcePtr->Get_OBJTYPE() == OBJ_PLAYER || event.sourcePtr->Get_OBJTYPE() == OBJ_BULLET ) && event.targetPtr->Get_OBJTYPE() == OBJ_MAPEVENT)
				InteractionCombatEvent(&event);

			if ((event.sourcePtr->Get_OBJTYPE() == OBJ_BULLET) && (event.targetPtr->Get_OBJTYPE() == OBJ_BARRIER))
				ReflectBulletfromBarrierEVENT(&event);

			else if (event.sourcePtr->Get_OBJTYPE() != OBJ_BULLET)
			{
				auto sourceStatus = static_cast<CStatus*>(event.sourcePtr->Get_Component(L"Com_Status", ID_DYNAMIC));
				auto targetStatus = static_cast<CStatus*>(event.targetPtr->Get_Component(L"Com_Status", ID_DYNAMIC));

				if (targetStatus == NULL) return;

				targetStatus->OnDamaged(sourceStatus->Get_Status().fAttack);

				if (event.targetPtr->Get_OBJTYPE() != OBJ_PLAYER)
				{
					auto monster = static_cast<CMonster*>(event.targetPtr);

					if (monster->Get_MonsterStatus() != MONSTER_STATUS_ICE)
						monster->Hit();
				}
			}

			else
			{
				// Bullet은 Object Pooling으로 만든 것이기 때문에 반드시 이곳으로 와서 검사 받아야함.
				auto TargetType = event.targetHandle.m_eType;

				if (TargetType == OBJ_CUBE || TargetType == OBJ_TILE || TargetType == OBJ_EVENTCUBE) return;	// 큐브와 타일이면 Status가 없음. 바로 반환.

				auto sourceStatus = static_cast<CStatus*>(event.sourcePtr->Get_Component(L"Com_Status", ID_DYNAMIC));
				auto targetStatus = static_cast<CStatus*>(event.targetPtr->Get_Component(L"Com_Status", ID_DYNAMIC));

				if (targetStatus == NULL) return;

				targetStatus->OnDamaged(sourceStatus->Get_Status().fAttack);

				// 타겟이 Player가 아니라면.. 그냥 Monster라는 소리.
				if (event.targetPtr->Get_OBJTYPE() != OBJ_PLAYER)
				{
					auto monster = static_cast<CMonster*>(event.targetPtr);
					
					// 몬스터가 빙결 상태가 아니라면 Hit animation 출력.
					if (monster->Get_MonsterStatus() != MONSTER_STATUS_ICE)
						monster->Hit();
				}
			}
		}
		// 전투가 발생했다면 사운드 System한테 보내주면 좋겠지. ->이건 CollsionEvent에서 발행하자.
	}

	// 내가 힐을 받았다면. ex) 우물과 상호작용을 했거나, 아이템을 먹었거나.
	if(bus->Subscribe(CHANNEL::Combat, TOPIC::ONHEALED, PHASE::PHASE_BEGIN, event, false))
	{
		// 여기서 미리 다 검사.
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.targetHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.targetPtr = CObjectPoolingManager::GetInstance()->Resolve(event.targetHandle);

		// 검사가 끝났다면.

		if (event.sourcePtr && event.targetPtr)
		{
			auto sourceStatus = static_cast<CStatus*>(event.sourcePtr->Get_Component(L"Com_Status", ID_DYNAMIC));
			auto targetStatus = static_cast<CStatus*>(event.targetPtr->Get_Component(L"Com_Status", ID_DYNAMIC));

			if (targetStatus == NULL) return;

			auto Itemtype = static_cast<CItem*>(event.targetPtr)->Get_ItemType();

			// 힐 어떻게 시켜주지. 
			// Target Status의 CurrenHp 흡수하는걸로 하자.
			if (Itemtype != ITEMTYPE::ITEM_ARMOR_SHARD)
				sourceStatus->OnHealed(targetStatus->Get_Status().fCurrentHp);

			else
				sourceStatus->Add_Armor(targetStatus->Get_Status().fCurrentHp);
			 
			event.targetPtr->Set_Active(false);
		}
	}

}
//
void CCombatSystem::InteractionCombatEvent(EVENT* event)
{
 	auto SourceType = event->sourceHandle.m_eType;
	auto TargetType = event->targetHandle.m_eType;


	if (SourceType != OBJTYPE::OBJ_PLAYER && SourceType != OBJTYPE::OBJ_BULLET) return;

	if (TargetType != OBJTYPE::OBJ_MAPEVENT) return;
	if (static_cast<CEventObject*>(event->targetPtr)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT) return;
	CInteractable* pInteraction = static_cast<CInteractable*>(event->targetPtr);

	auto Type = pInteraction->GetInteractType();

	switch (Type)
	{ //생성할 것들 나머지는 return
	case Engine::INTERACT_TYPE::ANTENAE:
	case Engine::INTERACT_TYPE::BARREL:
	case Engine::INTERACT_TYPE::BUSH:

	case Engine::INTERACT_TYPE::FLOWERBUSH:
	case Engine::INTERACT_TYPE::FLOWERPOT:
	case Engine::INTERACT_TYPE::LONGPOT:
	case Engine::INTERACT_TYPE::FATPOT:
	case Engine::INTERACT_TYPE::DUMMY:
	case Engine::INTERACT_TYPE::CORPSE:
	case Engine::INTERACT_TYPE::BOOK:
	case Engine::INTERACT_TYPE::POUCH:
	case Engine::INTERACT_TYPE::BROKEWALL:
	case Engine::INTERACT_TYPE::ORB:
		break;
	case Engine::INTERACT_TYPE::ORBFIELD:
		OrbFieldCombatEvent(event->sourcePtr, pInteraction);
		break;
		// 아닌것들은 리턴 때려주고
	default:
		return;
	}

	//미리 Get_IsCanBreak 인애들은 리턴때려준다..
	if (!pInteraction->Get_IsCanBreak())
	{
		return;
	}

	//알아서 내부적으로 히트 카운트가 0이하면 Get_IsCanBreak -> True 에서 false
	pInteraction->Minus_HitToBreak();

	//여기서 들어오는거는 부셔지고 난뒤 이벤트로 처리  이게바로 false 로 바뀐거면 부셔졌다는 뜻
	if (!pInteraction->Get_IsCanBreak())
	{
		BrokenEvent(event->sourcePtr, pInteraction);
	}
}
//w
void CCombatSystem::OrbFieldCombatEvent(CGameObject* pHiter, CInteractable* pInteraction)
{
	auto HiterType = pHiter->Get_OBJTYPE();

	if (HiterType == OBJTYPE::OBJ_BULLET)
	{
		auto Player = pHiter->Get_ObjectOwner();	if (Player == nullptr) return;
		auto Player_Status = Player->Get_Status();	if (Player_Status == nullptr) return;

		CStatus* Bullet_Status = pHiter->Get_Status();
		if (Bullet_Status == nullptr) return;
		

		//자기 자신의 공격력 만큼 체력 감소
		Player_Status->OnDamaged(Bullet_Status->Get_Status().fAttack);

		Player_Status->ClampCurrentHp();
	}

	else if (HiterType == OBJTYPE::OBJ_PLAYER)
	{
		auto Player_Status = pHiter->Get_Status();
		if (Player_Status == nullptr) return;

		CStatus* Bullet_Status = pHiter->Get_Status();
		if (Bullet_Status == nullptr) return;


		//자기 자신의 공격력 만큼 체력 감소
		Player_Status->OnDamaged(Bullet_Status->Get_Status().fAttack);

		Player_Status->ClampCurrentHp();
	}
}
//
void CCombatSystem::BrokenEvent(CGameObject* pHiter, CInteractable* pInteraction)
{
	auto Type = pInteraction->GetInteractType();
	const wchar_t* soundKey = nullptr;
	switch (Type)
	{
		//특별한 상호작용이 더있는애들
		//오브 
	case Engine::INTERACT_TYPE::ORB:
		OrbBrokenEvent(pInteraction);
		break;

	case Engine::INTERACT_TYPE::ANTENAE:
		AntenaeBrokenEvent(pInteraction);

		break;
		//통나무 시체만 부셨을때 아이템을 떨궈준다...
	case Engine::INTERACT_TYPE::BARREL:
		soundKey = L"barrel-explode.wav";
		ItemDropBrokenEvent(pInteraction);
		Play_Particle_Barrel(pInteraction);
		break;
	case Engine::INTERACT_TYPE::CORPSE:
		soundKey = L"open_corpse.wav";
		ItemDropBrokenEvent(pInteraction);
		Play_Particle_Blood(pInteraction);
		break;
		//단순 부서지기만 하는애들 책 파우치 더미 가벽  플라워부쉬는 예외처리할예정 아이템은 안뿌리는 애들
		// 플라워부쉬는 부시면 쳬력회복해준다.
	case Engine::INTERACT_TYPE::BUSH:
		soundKey = L"bush-destroy.wav";
		SimpleBrokenEvent(pInteraction);
		Play_Particle_Bush(pInteraction);
		break;
	case Engine::INTERACT_TYPE::BOOK:
		soundKey = L"bush-destroy.wav";
		SimpleBrokenEvent(pInteraction);
		Play_Particle_Book(pInteraction);
		break;
	case Engine::INTERACT_TYPE::FLOWERPOT:
	case Engine::INTERACT_TYPE::LONGPOT:
	case Engine::INTERACT_TYPE::FATPOT:
		soundKey = L"pottery-break-2.wav";
		SimpleBrokenEvent(pInteraction);
		break;
	case Engine::INTERACT_TYPE::POUCH:
	case Engine::INTERACT_TYPE::DUMMY:
	case Engine::INTERACT_TYPE::BROKEWALL:
		soundKey = L"wall-collapse.wav";
		SimpleBrokenEvent(pInteraction);
		break;
	case Engine::INTERACT_TYPE::FLOWERBUSH:
		soundKey = L"bush-destroy.wav";
		FLowerBushBrokenEvent(pHiter, pInteraction);
		Play_Particle_Bush(pInteraction);
		break;
	default:
		return;
	}
	if (soundKey && *soundKey)
	{
		CSoundManager::GetInstance()->SoundPlay(soundKey, INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
	}
}

//
void CCombatSystem::SimpleBrokenEvent(CInteractable* pInteraction)
{
	pInteraction->Change_SpriteToLast_ByCollision();
}
//
void CCombatSystem::ItemDropBrokenEvent(CInteractable* pInteraction)
{
	//아이템 테이블이 나올려나?
	//아이템 뿌리는 함수? 일단 부셔지는지나 보자..


	pInteraction->Change_SpriteToLast_ByCollision();
}
//
void CCombatSystem::OrbBrokenEvent(CInteractable* pInteraction)
{
	auto Type = pInteraction->GetInteractType();

	if (Type != INTERACT_TYPE::ORB)
		return;

	//오브가 부셔지게 되면 오브필드를 없애버리는 이벤트를 발송해야한다

	vector<CGameObject*> Objlist = CObjectManager::GetInstance()->Get_EventIndex_ObjectVec(OBJTYPE::OBJ_MAPEVENT,pInteraction->Get_HandleEventIndex());

	CInteractable* pOrbField = nullptr;

	for (auto Obj : Objlist)
	{
		if (Obj->Get_OBJTYPE() != OBJ_MAPEVENT)
			continue;
		
		if (static_cast<CEventObject*>(Obj)->GetObject_Event_Type() != OBJECT_EVENT_TYPE::INTERECT)
			continue;

		CInteractable* CheckObj = static_cast<CInteractable*>(Obj);
		if (CheckObj->GetInteractType() == INTERACT_TYPE::ORBFIELD)
			pOrbField = CheckObj;
		else
			continue;
	}

	pInteraction->Change_SpriteToLast_ByCollision();

	if (pOrbField == nullptr)
		return;

	pOrbField->Minus_OrbCount();		//Minust_OrbCount

	if (pOrbField->Get_OrbCount() <= 0)
	{
		pOrbField->Set_Active(false);
		pOrbField->Get_Collide()->Set_ColliderType(COLLIDER_NONE);
	}

}
//
void CCombatSystem::FLowerBushBrokenEvent(CGameObject* pHiter, CInteractable* pInteraction)
{
	auto HiterType = pHiter->Get_OBJTYPE();
	
	if (HiterType == OBJTYPE::OBJ_BULLET)
	{
		auto Type = pInteraction->GetInteractType();

		if (Type != INTERACT_TYPE::FLOWERBUSH) return;

		auto Player = pHiter->Get_ObjectOwner();
		if (Player->Get_OBJTYPE() != OBJ_PLAYER) return;

		auto Player_Status = Player->Get_Status();
		if (Player_Status == nullptr) return;

		//힐을해준다
		Player_Status->OnHealed(pInteraction->Get_HeadlAmount());
		//최대 체력을 넘어가게되면 다시 최대 체력으로 복구
		Player_Status->ClampCurrentHp();
	}

	else if (HiterType == OBJTYPE::OBJ_PLAYER)
	{
		auto Type = pInteraction->GetInteractType();

		if (Type != INTERACT_TYPE::FLOWERBUSH) return;

		auto Player_Status = pHiter->Get_Status();
		if (Player_Status == nullptr) return;

		//힐을해준다
		Player_Status->OnHealed(pInteraction->Get_HeadlAmount());
		//최대 체력을 넘어가게되면 다시 최대 체력으로 복구
		Player_Status->ClampCurrentHp();
	}

	pInteraction->Change_SpriteToLast_ByCollision();

}

void CCombatSystem::AntenaeBrokenEvent(CInteractable* pInteraction)
{
	auto bus = CEventBus::GetInstance();

	if (pInteraction->GetInteractType() != INTERACT_TYPE::ANTENAE) return;
	
	pInteraction->Change_SpriteToLast_ByCollision();

	vector<CGameObject*> EventCubeVec = CObjectManager::GetInstance()->Get_EventIndex_ObjectVec(OBJTYPE::OBJ_EVENTCUBE, pInteraction->Get_HandleEventIndex());

	for (auto EventCube : EventCubeVec)
	{
		if ( pInteraction->Get_HandleEventIndex() == EventCube->Get_HandleEventIndex())
		{
			CEventCube* pEvent = static_cast<CEventCube*>(EventCube);
			if (pEvent->GetCubeEventType() == CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC)
			{
				pEvent->PlayEvent();
				pEvent->Play_BrokenCubeParticleSystem();
			}
		}
	}

	// 부숴졌으니까 이벤트 발송
	EVENT event
	{
		PHASE::PHASE_BEGIN,
		TOPIC::Boss_Gimmik,
		CHANNEL::Boss,
		nullptr, nullptr, {}, {}
	};
	bus->publish(event);

}


	// 상태이상 이벤트 처리하는 함수.
void CCombatSystem::ApplyStatusEffect()
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Combat, TOPIC::FROZEN, PHASE::PHASE_BEGIN, event, false))
	{
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.sourcePtr)
		{
			// 얼어있는 놈 있으면 리스트안에 넣어둠.
 			m_lFrozenObjectList.push_back(event.sourcePtr);
		}
	}

	if (bus->Subscribe(CHANNEL::Combat, TOPIC::FROZEN, PHASE::PHASE_UPDATE, event, false))
	{ 
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		for (auto monster : m_lFrozenObjectList)
		{
			static_cast<CMonster*>(monster)->Set_MonsterStatus(MONSTER_STATUS_ICE_DYING);
		}

		m_lFrozenObjectList.clear();
	}

	if (bus->Subscribe(CHANNEL::Combat, TOPIC::FROZEN, PHASE::PHASE_END, event, false))
	{
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.sourcePtr)
		{
			m_lFrozenObjectList.remove_if([&](CGameObject* pObject)
				{
					// 얼음 풀렸으면 리스트 안에서 해제
					return (pObject == event.sourcePtr);
				});
		}
	}
}

void CCombatSystem::ReflectBulletfromBarrierEVENT(EVENT* event)
{
	CBullet* bullet = static_cast<CBullet*>(event->sourcePtr); if (bullet == NULL) return;
	CBarrier* BarrierComponent = static_cast<CBarrier*>(event->targetPtr->Get_Component(L"Com_Barrier", ID_DYNAMIC)); 	if (BarrierComponent == NULL) return;

	_vec3 bulletDir = bullet->Get_BulletDir();
	_vec3 bulletPos = *bullet->Get_Transform()->GetInfoPos();
	bullet->Set_ObjectOwner(event->targetPtr);
	bullet->Set_BulletDir(BarrierComponent->Get_ReflectDir(bulletPos, bulletDir));
	bullet->Reset_ElapsedTime();

}

void CCombatSystem::ApplyCombatRules()	// 자 룰 적용 한번 해볼까.
{

}

void CCombatSystem::Free()
{

}

void CCombatSystem::Play_Particle_Bush(CInteractable* pInteraction)
{
	CParticleSystem_Bush* p;
	p = dynamic_cast<CParticleSystem_Bush*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BUSH));
	p->Set_Target(pInteraction);
	p->Activate_ParticleSystem();
	p->Make_And_Apply_Values_To_Particle();
}

void CCombatSystem::Play_Particle_Barrel(CInteractable* pInteraction)
{
	CParticleSystem_Barrel* p;
	p = dynamic_cast<CParticleSystem_Barrel*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BARREL));
	p->Set_Target(pInteraction);
	p->Activate_ParticleSystem();
	p->Make_And_Apply_Values_To_Particle();
}

void CCombatSystem::Play_Particle_Blood(CInteractable* pInteraction)
{
	CParticleSystem_Blood* p;
	p = dynamic_cast<CParticleSystem_Blood*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BLOOD));
	p->Set_Target(pInteraction);
	p->Activate_ParticleSystem();
	p->Make_And_Apply_Values_To_Particle();
}

void CCombatSystem::Play_Particle_Book(CInteractable* pInteraction)
{
	CParticleSystem_Book* p;
	p = dynamic_cast<CParticleSystem_Book*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BOOK));
	p->Set_Target(pInteraction);
	p->Activate_ParticleSystem();
	p->Make_And_Apply_Values_To_Particle();
}
