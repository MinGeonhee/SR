#include "CBossSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CCameraManager.h"
#include "CCamera.h"
#include "CWeaponController.h"
#include "CSceneManager.h"
#include "CBarrier.h"
#include "CTriggerEventManager.h"
#include "CTransform.h"

IMPLEMENT_SINGLETON(CBossSystem)

CBossSystem::CBossSystem()
{

}

CBossSystem::~CBossSystem()
{
	Free();
}

_int CBossSystem::System_Update(const float& fTimeDelta)
{
	if (m_pBossBarrier == NULL)
	{
		auto obj = CObjectManager::GetInstance()->Get(OBJ_BARRIER); if (obj == NULL) return 0;
		m_pBossBarrier = obj;
	}

	return 0;
}

void CBossSystem::System_LateUpdate(const float& fTimeDelta)
{
	Boss_Room_Enter();
	First_Phase();
	Second_Phase();
	//Third_Phase();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CBossSystem::First_Phase()	// Camera
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Gimmik, PHASE::PHASE_BEGIN, event, false))
	{
		auto barrier = m_pBossBarrier->Get_Component(L"Com_Barrier", ID_DYNAMIC); if (barrier == NULL) return;
		CBarrier* Cbar = static_cast<CBarrier*>(barrier);
		Cbar->Set_BarrierCountDown();

		if (Cbar->Get_BarrierCount() <= 0) CTriggerEventManager::GetInstance()->Set_Boss_gimmikSuccessSceneActive(true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Gimmik, PHASE::PHASE_END, event, false))
	{
		m_eCurBossPhase = BOSS_PHASE_ONE;
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss1Phaseto2, PHASE::PHASE_BEGIN, event, false))
	{
		m_eCurBossPhase = BOSS_PHASE_CUTSCENE_1_TO_2;

		CGameObject* boss = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);	if (boss == NULL) return;
		CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(true);
		CTransform* Trans = static_cast<CTransform*>(boss->Get_Component(L"Com_Transform", ID_DYNAMIC));	if (Trans == NULL) return;
		Trans->Set_Pos(26.5f, 2.f, 42.5f);
	}

}

void CBossSystem::Second_Phase()	// Camera
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss2Phase, PHASE::PHASE_BEGIN, event, false))
	{
		m_eCurBossPhase = BOSS_PHASE_TWO;
		CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS2);
	}

}

void CBossSystem::Third_Phase()	// Camera
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

}

void CBossSystem::Boss_Room_Enter()
{
	auto bus = CEventBus::GetInstance();
	EVENT event;

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::DOOR_OPEN, PHASE::PHASE_BEGIN, event, false))
	{
		m_eCurBossPhase = BOSS_PHASE_INTRO;
	}

}

void CBossSystem::Free()
{

}