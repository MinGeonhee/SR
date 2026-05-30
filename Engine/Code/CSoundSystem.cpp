#include "CSoundSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCollisionManager.h"
#include "CDoor_Component.h"
#include "CItemSystem.h"
#include "CWeaponController.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CSoundSystem)

CSoundSystem::CSoundSystem()
{

}

CSoundSystem::~CSoundSystem()
{
	Free();
}

_int CSoundSystem::System_Update(const float& fTimeDelta)
{
	FireSound();
	return 0;
}

void CSoundSystem::System_LateUpdate(const float& fTimeDelta)
{
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CSoundSystem::FireSound()	// 문이랑 충돌하면 UI 상호작용 출력되게 하는 함수
{
	auto bus = CEventBus::GetInstance();
	EVENT event;
	// 처맞았을 때 사운드 소리 

	if (bus->Subscribe(CHANNEL::Sound, TOPIC::Fire, PHASE::PHASE_BEGIN, event, false))
	{
		// 무기 컴포넌트를 만들어서 무기 컴포넌트의 값에 따라 TOPIC을 나누면 될 것 같다.
		Engine::CWeaponController* weapon = static_cast<CWeaponController*>(event.sourcePtr->Get_Component(L"Com_Weapon", ID_DYNAMIC));

		if (weapon == NULL) return;

		auto Weapontype = weapon->Get_WeaponType();

		switch (Weapontype)
		{
			// 플레이어 무기
		case WEAPONSTATE::WEAPON_MACE:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_FIREWAND:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_GUN:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_ICEWAND:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_FIRERING:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		// 몬스터 무기 

		case WEAPONSTATE::WEAPON_GOBLIN_FIREBALL:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_SKELLETON_SILVER_ARROW:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;

		case WEAPONSTATE::WEAPON_SKELLETON_GOLD_ARROW:
		{
			CSoundManager::GetInstance()->SoundPlay(L"Sound", SOUNDCHANNEL::BULLET_SOUND, 1.f);
		}
		break;
		}

	}
}

void CSoundSystem::Free()
{

}