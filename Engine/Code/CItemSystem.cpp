#include "CItemSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCollisionManager.h"
#include "CDoor_Component.h"
#include "CObjectPoolingManager.h"
#include "CItem.h"
#include "CUIManager.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CItemSystem)

CItemSystem::CItemSystem()
{

}

CItemSystem::~CItemSystem()
{
	Free();
}

_int CItemSystem::System_Update(const float& fTimeDelta)
{


	return 0;
}

void CItemSystem::System_LateUpdate(const float& fTimeDelta)
{
	ItemCollisionEvent();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CItemSystem::ItemCollisionEvent()	// 문이랑 충돌하면 UI 상호작용 출력되게 하는 함수
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::ITEM, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		if (!event.sourcePtr || !event.targetPtr) return;

		switch (event.targetHandle.m_eOriginType)
		{
			case ORIGIN_STATIC: Pickup_StaticItem(event); break;

			case ORIGIN_DYNAMIC: Pickup_DynamicItem(event); break;
		}
	}
}

void CItemSystem::Pickup_StaticItem(EVENT event)
{
	auto item = event.targetPtr;
	auto Player = event.sourcePtr;
	auto PlayerStatus = event.sourcePtr->Get_Component(L"Com_Status", ID_DYNAMIC);
	auto itemtype = static_cast<CItem*>(item)->Get_ItemType();

	switch (itemtype)
	{
		// 회복 아이템
	case ITEMTYPE::ITEM_BERRIES: HealItem(event); break;
	case ITEMTYPE::ITEM_HEALTH_SHARD: HealItem(event);break;
	case ITEMTYPE::ITEM_ARMOR_SHARD: HealItem(event); break;
	case ITEMTYPE::ITEM_GREATER_POTION: HealItem(event); break;

		// 돈 자루들
	case ITEMTYPE::ITEM_BROZE_COINS:AddMoney(event, 1500); break;
	case ITEMTYPE::ITEM_SILVER_COINS: AddMoney(event, 2500); break;
	case ITEMTYPE::ITEM_GOLD_COINS:AddMoney(event, 5000); break;
	case ITEMTYPE::ITEM_COIN_PURSE: AddMoney(event, 4500); break;
	case ITEMTYPE::ITEM_GOLD_CHALICE: AddMoney(event, 7500); break;
		// 무기 아이템
	case ITEMTYPE::ITEM_FIREBALL_RING: RegisterWeaponType(event, ITEM_FIREBALL_RING); break;
	case ITEMTYPE::ITEM_ICEWAND: RegisterWeaponType(event, ITEM_ICEWAND); break;
	case ITEMTYPE::ITEM_GUN: RegisterWeaponType(event, ITEM_GUN); break;
		// 총알류
	case ITEMTYPE::ITEM_LOW_MANA: BulletItem(event, ITEM_LOW_MANA, 12); break;
	case ITEMTYPE::ITEM_FIRE_SHARD: BulletItem(event, ITEM_FIRE_SHARD, 12); break;
	case ITEMTYPE::ITEM_ICE_SHARD: BulletItem(event, ITEM_ICE_SHARD, 12); break;
	case ITEMTYPE::ITEM_GUN_BULLET: BulletItem(event, ITEM_GUN_BULLET, 12); break;
		// 열쇠 아이템류
	case ITEMTYPE::ITEM_BRONZE_KEY: RegisterKeyType(event, ITEM_BRONZE_KEY); break;
	case ITEMTYPE::ITEM_SILVER_KEY: RegisterKeyType(event, ITEM_SILVER_KEY); break;
	case ITEMTYPE::ITEM_GOLD_KEY: RegisterKeyType(event, ITEM_GOLD_KEY); break;
	case ITEMTYPE::ITEM_TREASURE_KEY: RegisterKeyType(event, ITEM_TREASURE_KEY); break;

	}
}

void CItemSystem::Pickup_DynamicItem(EVENT event)
{
	auto item = CObjectPoolingManager::GetInstance()->Resolve(event.targetHandle);
	
	if (!item) return;

	event.targetPtr = item;	// 타깃 아이템 포인터 넣어주고.
	Pickup_StaticItem(event);	// static Item 받아준다.
}

void CItemSystem::HealItem(EVENT event)
{
	auto itemObject = event.targetPtr;
	auto itemType = static_cast<CItem*>(itemObject)->Get_ItemType();

	auto Player = event.sourcePtr;
	auto PlayerStatus = static_cast<CStatus*>(event.sourcePtr->Get_Component(L"Com_Status", ID_DYNAMIC))->Get_Status();

	auto CurrentHP = PlayerStatus.fCurrentHp;
	auto MaxHP = PlayerStatus.fMaxHp;

	// 현재 내 체력이 Max Hp 보다 높다면 그냥 return
	if (MaxHP <= CurrentHP && itemType != ITEMTYPE::ITEM_ARMOR_SHARD) return;
	else
	{
		switch (itemType)
		{
		case Engine::ITEM_BERRIES:
			CUIManager::GetInstance()->PushTag(L"BERRIES");
			CSoundManager::GetInstance()->SoundPlay(L"get_berry.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		case Engine::ITEM_HEALTH_SHARD:
			CSoundManager::GetInstance()->SoundPlay(L"get_shard_health.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			CUIManager::GetInstance()->PushTag(L"HEALTH SHARD");
			break;
		case Engine::ITEM_ARMOR_SHARD:
			CUIManager::GetInstance()->PushTag(L"ARMOR SHARD");
			CSoundManager::GetInstance()->SoundPlay(L"get_armor_shard.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		case Engine::ITEM_GREATER_POTION:
			CUIManager::GetInstance()->PushTag(L"GREATER POTION");
			CSoundManager::GetInstance()->SoundPlay(L"get_potion.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		default:
			break;
		}

		event.phase = PHASE::PHASE_BEGIN;
		event.topic = TOPIC::ONHEALED;
		event.channel = CHANNEL::Combat;

		CEventBus::GetInstance()->publish(event);
	}
}

void CItemSystem::AddMoney(EVENT event, int count)
{
	auto itemObject = event.targetPtr;
	auto itemType = static_cast<CItem*>(itemObject)->Get_ItemType();

	const wchar_t* Coin =
		(itemType == ITEM_BROZE_COINS) ? L"BRONZE COIN" :
		(itemType == ITEM_SILVER_COINS) ? L"SILVER COIN" :
		(itemType == ITEM_GOLD_COINS) ? L"GOLD COIN" :
		(itemType == ITEM_COIN_PURSE) ? L"COIN PURSE" :
		(itemType == ITEM_GOLD_CHALICE) ? L"GOLD CHALICE" : L"UNKNOWN";
	
	CSoundManager::GetInstance()->SoundPlay(L"get_coin_purse.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
	CUIManager::GetInstance()->PushTag(Coin);

	event.phase = PHASE::PHASE_BEGIN;
	event.topic = TOPIC::Get_Effect;
	event.channel = CHANNEL::UI;
	CEventBus::GetInstance()->publish(event);

	auto playerwallet = static_cast<CWallet*>(event.sourcePtr->Get_Component(L"Com_Wallet", ID_STATIC));

	playerwallet->ADD_Money(count);
	event.targetPtr->Set_Active(false);
}

void CItemSystem::BulletItem(EVENT event, ITEMTYPE type, int count)
{
	auto Player = event.sourcePtr;
	auto PlayerWeapon = static_cast<CWeaponController*>(event.sourcePtr->Get_Component(L"Com_Weapon", ID_DYNAMIC));

	if (PlayerWeapon->CanPickUpBullet(type))
	{
		PlayerWeapon->Add_WeaponBullet(type, count);
		event.targetPtr->Set_Active(false);

		switch (type)
		{
		case Engine::ITEM_LOW_MANA:
			CUIManager::GetInstance()->PushTag(L"LOW MANA");
			CSoundManager::GetInstance()->SoundPlay(L"get_berry.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		case Engine::ITEM_FIRE_SHARD:
			CSoundManager::GetInstance()->SoundPlay(L"get_fire_mana.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			CUIManager::GetInstance()->PushTag(L"FIRE SHARD");
			break;
		case Engine::ITEM_ICE_SHARD:
			CUIManager::GetInstance()->PushTag(L"ICE SHARD");
			CSoundManager::GetInstance()->SoundPlay(L"get_ice_shards.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		case Engine::ITEM_GUN_BULLET:
			CUIManager::GetInstance()->PushTag(L"GUN BULLET");
			CSoundManager::GetInstance()->SoundPlay(L"get_arcane_rocks.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			break;
		default:
			break;
		}

		event.phase = PHASE::PHASE_BEGIN;
		event.topic = TOPIC::Get_Effect;
		event.channel = CHANNEL::UI;
		CEventBus::GetInstance()->publish(event);
	}
}

void CItemSystem::RegisterWeaponType(EVENT event, ITEMTYPE type)
{
	auto item = event.targetPtr;
	auto pitem= static_cast<CItem*>(item);

	switch (type)
	{
	case ITEMTYPE::ITEM_FIREBALL_RING:
		{
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_FIREBALL_RING)].size() == 0)
		{
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_FIREBALL_RING)].push(pitem);
			item->Set_Active(false);
			CUIManager::GetInstance()->SetCategoryTag(L"WeaponGet_UI", L"Fire Rings");
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Get_Weapon;
			event.channel = CHANNEL::UI;

			CEventBus::GetInstance()->publish(event);
		}

		else
			BulletItem(event, ITEMTYPE::ITEM_FIRE_SHARD, 12);


		}
		break;

		case ITEMTYPE::ITEM_ICEWAND:
		{
			if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_ICEWAND)].size() == 0)
			{
				m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_ICEWAND)].push(pitem);
				item->Set_Active(false);
				CUIManager::GetInstance()->SetCategoryTag(L"WeaponGet_UI", L"FrostWeaver");
				event.phase = PHASE::PHASE_BEGIN;
				event.topic = TOPIC::Get_Weapon;
				event.channel = CHANNEL::UI;

				CEventBus::GetInstance()->publish(event);
			}

			else
				BulletItem(event, ITEMTYPE::ITEM_ICE_SHARD, 12);
		}
			break;

		case ITEMTYPE::ITEM_GUN:
		{
			if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GUN)].size() == 0)
			{
				m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GUN)].push(pitem);
				item->Set_Active(false);
				CUIManager::GetInstance()->SetCategoryTag(L"WeaponGet_UI", L"Spellstriker");
				event.phase = PHASE::PHASE_BEGIN;
				event.topic = TOPIC::Get_Weapon;
				event.channel = CHANNEL::UI;

				CEventBus::GetInstance()->publish(event);
			}


			else
				BulletItem(event, ITEMTYPE::ITEM_GUN_BULLET, 12);
		}
			break;
	}
}

void CItemSystem::RegisterKeyType(EVENT event, ITEMTYPE type)
{
	auto item = event.targetPtr;
	auto pitem = static_cast<CItem*>(item);

	switch (type)
	{
	case ITEMTYPE::ITEM_BRONZE_KEY:
	
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_BRONZE_KEY)].size() == 0)
		{
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_BRONZE_KEY)].push(pitem);
			item->Set_Active(false);

			CUIManager::GetInstance()->PushTag(L"BRONZE KEY");
			CSoundManager::GetInstance()->SoundPlay(L"get_key.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Get_Effect;
			event.channel = CHANNEL::UI;
			CEventBus::GetInstance()->publish(event);
		}
		break;

	case ITEMTYPE::ITEM_SILVER_KEY:

		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_SILVER_KEY)].size() == 0)
		{
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_SILVER_KEY)].push(pitem);
			item->Set_Active(false);

			CUIManager::GetInstance()->PushTag(L"SILVER KEY");
			CSoundManager::GetInstance()->SoundPlay(L"get_key.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Get_Effect;
			event.channel = CHANNEL::UI;
			CEventBus::GetInstance()->publish(event);
		}
			break;

	case ITEMTYPE::ITEM_GOLD_KEY:
	
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GOLD_KEY)].size() == 0)
		{
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GOLD_KEY)].push(pitem);
			item->Set_Active(false);

			CUIManager::GetInstance()->PushTag(L"GOLD KEY");
			CSoundManager::GetInstance()->SoundPlay(L"get_key.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Get_Effect;
			event.channel = CHANNEL::UI;
			CEventBus::GetInstance()->publish(event);
		}
			break;

	case ITEMTYPE::ITEM_TREASURE_KEY:

		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_TREASURE_KEY)].size() == 0)
		{
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_TREASURE_KEY)].push(pitem);
			item->Set_Active(false);

			CUIManager::GetInstance()->PushTag(L"TREASURE KEY");
			CSoundManager::GetInstance()->SoundPlay(L"get_key.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
			event.phase = PHASE::PHASE_BEGIN;
			event.topic = TOPIC::Get_Effect;
			event.channel = CHANNEL::UI;
			CEventBus::GetInstance()->publish(event);
		}
			break;
	}
}

bool CItemSystem::IsWeaponEquipped(WEAPONSTATE Type)
{
	switch (Type)
	{
	case WEAPONSTATE::WEAPON_FIRERING:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_FIREBALL_RING)].size() == 1;
		break;

	case WEAPONSTATE::WEAPON_ICEWAND:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_ICEWAND)].size() == 1;
		break;

	case WEAPONSTATE::WEAPON_GUN:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GUN)].size() == 1;
		break;

	}
}


bool CItemSystem::HasKey(ITEMTYPE Type)
{
	switch (Type)
	{
	case ITEMTYPE::ITEM_BRONZE_KEY:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_BRONZE_KEY)].size() == 1;
		break;

	case ITEMTYPE::ITEM_SILVER_KEY:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_SILVER_KEY)].size() == 1;
		break;

	case ITEMTYPE::ITEM_GOLD_KEY:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GOLD_KEY)].size() == 1;
		break;

	case ITEMTYPE::ITEM_TREASURE_KEY:

		return m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_TREASURE_KEY)].size() == 1;
		break;
	}
}

void CItemSystem::UseKey(ITEMTYPE type)
{
	switch (type)
	{
	case ITEMTYPE::ITEM_BRONZE_KEY:
		if(m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_BRONZE_KEY)].size() == 1)
		m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_BRONZE_KEY)].pop();
		break;

	case ITEMTYPE::ITEM_SILVER_KEY:
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_SILVER_KEY)].size() == 1)
		m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_SILVER_KEY)].pop();
		break;

	case ITEMTYPE::ITEM_GOLD_KEY:
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GOLD_KEY)].size() == 1)
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_GOLD_KEY)].pop();
		break;

	case ITEMTYPE::ITEM_TREASURE_KEY:
		if (m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_TREASURE_KEY)].size() == 1)
			m_pItemList[static_cast<size_t>(ITEMTYPE::ITEM_TREASURE_KEY)].pop();
		break;
	}
}

void CItemSystem::Free()
{
	for (int i = 0; i < static_cast<size_t>(ITEMCOUNT); ++i)
	{
		if(m_pItemList[i].size() != 0)
		m_pItemList[i].pop();
	}
}