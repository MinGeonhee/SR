#include "CDoorSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCollisionManager.h"
#include "CDoor_Component.h"
#include "CItemSystem.h"
#include "CUIManager.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CDoorSystem)

CDoorSystem::CDoorSystem()
{

}

CDoorSystem::~CDoorSystem()
{
	Free();
}

_int CDoorSystem::System_Update(const float& fTimeDelta)
{
	return 0;
}

void CDoorSystem::System_LateUpdate(const float& fTimeDelta)
{
	DoorCollisionShowUINotice();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CDoorSystem::DoorCollisionShowUINotice()	// 문이랑 충돌하면 UI 상호작용 출력되게 하는 함수
{
	auto bus = CEventBus::GetInstance();

	EVENT event, Door;

	// 문 앞에 단순 충돌이라고 한다면. 상호작용 비이이이임 발사

	if (bus->Subscribe(CHANNEL::Door, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		FireInterationRayCast();
	}

	// 상호작용 빔이 문에 맞았고 + 상호작용키 E를 눌렀다면 해당 문 열리기.
	if (bus->Subscribe(CHANNEL::UI, TOPIC::DoorUINotice, PHASE::PHASE_BEGIN, Door, false))
	{
		if (bus->Subscribe(CHANNEL::Global, TOPIC::E_KEY, PHASE::PHASE_BEGIN, Door, false))
		{
			if (event.targetPtr)
			{
				auto door = static_cast<CDoor_Component*>(event.targetPtr->Get_Component(L"Com_Door", ID_DYNAMIC));
				auto state = door->Get_DoorType();
				auto Doorsystem = CItemSystem::GetInstance();
				
				switch (state)
				{
					case DOORTYPE::DEFAULT_WOOD_DOOR:
						CSoundManager::GetInstance()->SoundPlay(L"door-open.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
						door->Set_DoorState(DOORSTATE::DOOR_OPEN);
						break;

					case DOORTYPE::DEFAULT_IRON_DOOR:
						CSoundManager::GetInstance()->SoundPlay(L"door-sewers-open.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
						door->Set_DoorState(DOORSTATE::DOOR_OPEN);
						break;

					case DOORTYPE::BOSS_DOOR:
					{
						CSoundManager::GetInstance()->SoundPlay(L"wall-slide.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx*1.5);
						door->Set_DoorState(DOORSTATE::DOOR_OPEN);

						// 보스 문이 열렸음을 알리는 이벤트.
						EVENT bossevent;
						bossevent.channel = CHANNEL::Boss;
						bossevent.topic = TOPIC::DOOR_OPEN;
						bossevent.phase = PHASE::PHASE_BEGIN;

						CEventBus::GetInstance()->publish(bossevent);
					}

						break;

					// 다른 문이면 열쇠가 있는지 검사해야함.
					case DOORTYPE::BRONZE_DOOR:
					{
						if (Doorsystem->HasKey(ITEMTYPE::ITEM_BRONZE_KEY))
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-sewers-open.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							door->Set_DoorState(DOORSTATE::DOOR_OPEN);
							Doorsystem->UseKey(ITEMTYPE::ITEM_BRONZE_KEY);
						}

						else
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-locked.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Door_NotOpen");
							EVENT doorlock
							{
								PHASE::PHASE_BEGIN,
								TOPIC::LockDoorUINotice,
								CHANNEL::UI,
								nullptr,
								nullptr,
								{},
								{}
							};
							CEventBus::GetInstance()->publish(doorlock);
						}
						break;
					}

					case DOORTYPE::SILVER_DOOR:
					{
						if (Doorsystem->HasKey(ITEMTYPE::ITEM_SILVER_KEY))
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-sewers-open.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							door->Set_DoorState(DOORSTATE::DOOR_OPEN);
							Doorsystem->UseKey(ITEMTYPE::ITEM_SILVER_KEY);
						}

						else
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-locked.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Door_NotOpen");
							EVENT doorlock
							{
								PHASE::PHASE_BEGIN,
								TOPIC::LockDoorUINotice,
								CHANNEL::UI,
								nullptr,
								nullptr,
								{},
								{}
							};
							CEventBus::GetInstance()->publish(doorlock);
						}
						break;
					}

					case DOORTYPE::GOLD_DOOR:
					{
						if (Doorsystem->HasKey(ITEMTYPE::ITEM_GOLD_KEY))
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-sewers-open.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							door->Set_DoorState(DOORSTATE::DOOR_OPEN);
							Doorsystem->UseKey(ITEMTYPE::ITEM_GOLD_KEY);
						}

						else
						{
							CSoundManager::GetInstance()->SoundPlay(L"door-locked.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
							CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Door_NotOpen");
							EVENT doorlock
							{
								PHASE::PHASE_BEGIN,
								TOPIC::LockDoorUINotice,
								CHANNEL::UI,
								nullptr,
								nullptr,
								{},
								{}
							};
							CEventBus::GetInstance()->publish(doorlock);
						}
						break;
					}
					case DOORTYPE::SWITCH_WOOD_DOOR:
					case DOORTYPE::SWITCH_IRON_DOOR:
					{
						CSoundManager::GetInstance()->SoundPlay(L"door-locked.wav", INTERACTION_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
						CUIManager::GetInstance()->SetCategoryTag(L"UI_Interaction", L"Door_NotOpen");
							EVENT doorlock
							{
								PHASE::PHASE_BEGIN,
								TOPIC::LockDoorUINotice,
								CHANNEL::UI,
								nullptr, nullptr,{},{}
							};

						CEventBus::GetInstance()->publish(doorlock);
						break;
					}
				
				}
			}
		}
	}
}

void CDoorSystem::FireInterationRayCast()
{
	CCollisionManager::GetInstance()->FireRaycast(RAY_TYPE::INTERACTION_RAY, OBJTYPE::OBJ_DOOR);
}

void CDoorSystem::Free()
{

}