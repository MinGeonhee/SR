#include "CMonsterSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCameraManager.h"
#include "CMonster.h"
#include "CObjectPoolingManager.h"
#include "CUIManager.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CMonsterSystem)

CMonsterSystem::CMonsterSystem()
{

}

CMonsterSystem::~CMonsterSystem()
{
	Free();
}

_int CMonsterSystem::System_Update(const float& fTimeDelta)
{
	if(m_bMonsterTrapActive) Make_Monster(fTimeDelta);
	if (m_bSpawnSoundActive) PlayingSpawnSound(fTimeDelta);

	return 0;
}

void CMonsterSystem::System_LateUpdate(const float& fTimeDelta)
{
	Change_Monster_Status();
	Dynamic_Monster_Spawn();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CMonsterSystem::Change_Monster_Status()	// Camera
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Combat, TOPIC::FROZEN, PHASE::PHASE_BEGIN, event, false))
	{
		if (event.sourcePtr != nullptr)
		{
			static_cast<CMonster*>(event.sourcePtr)->Set_MonsterStatus(MONSTER_STATUS_ICE);
		}
	}
	
	if (bus->Subscribe(CHANNEL::Combat, TOPIC::FROZEN, PHASE::PHASE_END, event, false))
	{
		if (event.sourcePtr != nullptr)
		{
			if (static_cast<CMonster*>(event.sourcePtr)->Get_MonsterStatus() == MONSTER_STATUS_ICE)
			{
				static_cast<CMonster*>(event.sourcePtr)->Set_MonsterStatus(MONSTER_STATUS_IDLE);
			}
			
		}
	}
}

void CMonsterSystem::Dynamic_Monster_Spawn()
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Monster, TOPIC::MONSTERSPAWN, PHASE::PHASE_BEGIN, event, false))
	{
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.sourcePtr == NULL) return;
		auto objectType = event.sourcePtr->Get_OBJTYPE(); 
		_vec3 objectPos = *event.sourcePtr->Get_Transform()->GetInfoPos();

		switch (objectType)
		{
			case OBJTYPE::OBJ_MAPEVENT:
				m_tMonsterSpawner.iCount = 20;
				m_tMonsterSpawner.iRange = 3;
				m_tMonsterSpawner.Spawntime = 0.5f;
				m_tMonsterSpawner.vPos = objectPos;
				m_bSpawnSoundActive = true;
				m_bMonsterTrapActive = false;
				break;

			case OBJTYPE::OBJ_MONSTER_BOSS:
				m_tMonsterSpawner.iCount = 30;
				m_tMonsterSpawner.iRange = 5;
				m_tMonsterSpawner.Spawntime = 1.5f;
				m_tMonsterSpawner.vPos = objectPos;
				m_bSpawnSoundActive = false;
				m_bMonsterTrapActive = true;
				break;

			case OBJTYPE::OBJ_MONSTER_WIZARD:
				m_tMonsterSpawner.iCount = 10;
				m_tMonsterSpawner.iRange = 2;
				m_tMonsterSpawner.Spawntime = 0.5f;
				m_tMonsterSpawner.vPos = objectPos;
				m_bSpawnSoundActive = false;
				m_bMonsterTrapActive = true;
				break;
		}
	}
}

void CMonsterSystem::PlayingSpawnSound(const float& fTimeDelta)
{
	m_fSpawnSoundElapsedTime += fTimeDelta;

	if (m_fSpawnSoundElapsedTime >= 2.f)
	{
		CUIManager::GetInstance()->SetCategoryTag(L"PlayerStatPanel_UI", L"Hit_Effect");
		CUIManager::GetInstance()->ActivateUI(L"PlayerStatPanel_UI");
		m_fSpawnSoundElapsedTime = 0.f;
		m_iSpawnCount++;
	}

	if (m_iSpawnCount >= m_iSpawnSoundCount)
	{
		m_iSpawnCount = 0;
		m_bMonsterTrapActive = true;
		m_bSpawnSoundActive = false;
		m_fSpawnSoundElapsedTime = 0.f;
	}

}

void CMonsterSystem::Make_Monster(const float& fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;
	if (m_fElapsedTime >= m_tMonsterSpawner.Spawntime)
	{
		mt19937 rng{ random_device{}() };                  // 엔진 + 시드
		uniform_int_distribution<int> d6(static_cast<uint8_t>(OBJTYPE::OBJ_MONSTER_GOBLIN_HAMMER), static_cast<uint8_t>(OBJTYPE::OBJ_MONSTER_ORGE_AXE));            // 랜덤 몬스터 스폰
		OBJTYPE type = static_cast<OBJTYPE>(d6(rng));

		uniform_real_distribution<float> randposX(-m_tMonsterSpawner.iRange, m_tMonsterSpawner.iRange);            // 랜덤 pos
		uniform_real_distribution<float> randposZ(-m_tMonsterSpawner.iRange, m_tMonsterSpawner.iRange);            // 랜덤 pos
		float vPosX = randposX(rng); 		float vPosZ = randposZ(rng);
		_vec3 offset;		offset.x = vPosX;		offset.y = 0.f;		 offset.z = vPosZ;
		offset += m_tMonsterSpawner.vPos;

		CMonster* monster = static_cast<CMonster*>(CObjectPoolingManager::GetInstance()->Get(type)); if (monster == NULL) return; 
		monster->Set_MonsterStatus(MONSTER_STATUS_SPAWN);
		static_cast<CStatus*>(monster->Get_Component(L"Com_Status", ID_DYNAMIC))->Reset_Hp();
		monster->Get_Transform()->Set_Pos(offset.x, offset.y, offset.z);
		monster->Set_Target(CObjectManager::GetInstance()->Get(OBJ_PLAYER));

		m_fElapsedTime = 0.f;
		m_iMonsterCount++;
	}
	
	if (m_iMonsterCount >= m_tMonsterSpawner.iCount)
	{
		m_bMonsterTrapActive = false;
		m_iMonsterCount = 0;
		m_fElapsedTime = 0.f;
	}
}

void CMonsterSystem::Free()
{

}