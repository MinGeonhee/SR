#include "CTriggerEventManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CEventBus.h"
#include "CUIManager.h"
#include "CobjectManager.h"
#include "CMonster_Boss.h"

IMPLEMENT_SINGLETON(CTriggerEventManager)

CTriggerEventManager::CTriggerEventManager()
{

}

CTriggerEventManager::~CTriggerEventManager()
{
	Free();
}

_int CTriggerEventManager::Update_Event(const float& fTimeDelta)
{
	Boss_DoorOpen(fTimeDelta);
	Boss_ZoomCutScene(fTimeDelta);
	Boss_AppearScene(fTimeDelta);
	Boss_GimmikScene(fTimeDelta);
	Boss_GimmikSCeneEnd(fTimeDelta);
	Boss_1Phaseto2CutsceneBegin(fTimeDelta);
	Boss_1Phaseto2CutsceneUpdate(fTimeDelta);
	Boss_2PhaseEnter(fTimeDelta);
	return 0;
}

void CTriggerEventManager::Late_Update(const float& fTimeDelta)
{

}

void CTriggerEventManager::Boss_DoorOpen(const float& fTimeDelta)
{
	if (!m_bBossDoorOpen) return;

	m_fBossTimeElapsed += fTimeDelta;
	if (m_fBossTimeElapsed >= 3.f/*5.f*/)
	{
		EVENT event;
		event.channel = CHANNEL::Boss;
		event.topic = TOPIC::DOOR_OPEN;
		event.phase = PHASE::PHASE_UPDATE;

		m_fBossTimeElapsed = 0.f;
		m_bBossDoorOpen = false;
		CEventBus::GetInstance()->publish(event);
	}
}

void CTriggerEventManager::Boss_ZoomCutScene(const float& fTimeDelta)
{
	if (!m_bBossZoomCutSceneActive) return;

	m_fBossTimeElapsed += fTimeDelta;
	if (m_fBossTimeElapsed >= 9.f/*9.f*/)
	{
		EVENT event;
		event.channel = CHANNEL::Boss;
		event.topic = TOPIC::DOOR_OPEN;
		event.phase = PHASE::PHASE_END;

		m_fBossTimeElapsed = 0.f;
		m_bBossZoomCutSceneActive = false;
		CEventBus::GetInstance()->publish(event);
	}
}


void CTriggerEventManager::Boss_AppearScene(const float& fTimeDelta)
{
	if (!m_bBossAppearSceneActive) return;

	m_fBossTimeElapsed += fTimeDelta;
	if (m_fBossTimeElapsed >= 7.f/* 10.f*/)
	{
		EVENT event;
		event.channel = CHANNEL::Boss;
		event.topic = TOPIC::Boss_Appear;
		event.phase = PHASE::PHASE_BEGIN;

		m_fBossTimeElapsed = 0.f;
		m_bBossAppearSceneActive = false;
		CEventBus::GetInstance()->publish(event);
	}
}

void CTriggerEventManager::Boss_GimmikScene(const float& fTimeDelta)
{
	if (!m_bBossgimmikSuccessSceneActive) return;

	m_fBossTimeElapsed += fTimeDelta;

	if (m_fBossTimeElapsed >= 2.f/* 10.f*/)
	{
		EVENT gimmik
		{
			PHASE::PHASE_UPDATE,
			TOPIC::Boss_Gimmik,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		m_fBossTimeElapsed = 0.f;
		m_bBossgimmikSuccessSceneActive = false;
		CEventBus::GetInstance()->publish(gimmik);
	}
}

void CTriggerEventManager::Boss_GimmikSCeneEnd(const float& fTimeDelta)
{
	if (!m_bBoss_GimmikSceneEnd) return;

	m_fBossTimeElapsed += fTimeDelta;

	
	if (m_fBossTimeElapsed >= 1.5f && m_fBossAppeareffectdeletecount < 4)
	{
		CMonster_Boss* pBoss = static_cast<CMonster_Boss*>(CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS));

		pBoss->Get_TurnOffAppearEffect_Trigger(true);
		m_fBossTimeElapsed = 0.f;
		m_fBossAppeareffectdeletecount++;
	}

	CObjectManager::GetInstance()->Get(OBJ_BARRIER)->Set_Active(false);

	if (m_fBossTimeElapsed >= 3.f)
	{
		EVENT gimmik
		{
			PHASE::PHASE_END,
			TOPIC::Boss_Gimmik,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		m_fBossTimeElapsed = 0.f;
		m_bBoss_GimmikSceneEnd = false;
		CEventBus::GetInstance()->publish(gimmik);
		CMonster_Boss* pBoss = static_cast<CMonster_Boss*>(CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS));
		pBoss->Set_MonsterStatus(MONSTER_STATUS_TELEPORT);
	}

}

void CTriggerEventManager::Boss_1Phaseto2CutsceneBegin(const float& fTimeDelta)
{
	if (!m_bBoss_1Phaseto2CutsceneBegin) return;

	CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(false);

	m_fBossTimeElapsed += fTimeDelta;

	if (m_fBossTimeElapsed >= 2.f/* 10.f*/)
	{
		EVENT phasechange
		{
			PHASE::PHASE_UPDATE,
			TOPIC::Boss1Phaseto2,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		m_fBossTimeElapsed = 0.f;
		m_bBoss_1Phaseto2CutsceneBegin = false;
		CEventBus::GetInstance()->publish(phasechange);
	}
}

void CTriggerEventManager::Boss_1Phaseto2CutsceneUpdate(const float& fTimeDelta)
{
	if (!m_bBoss_1Phaseto2CutsceneUpdate) return;

	m_fBossTimeElapsed += fTimeDelta;

	if (m_fBossTimeElapsed >= 10.f/* 10.f*/)
	{
		EVENT phasechange
		{
			PHASE::PHASE_END,
			TOPIC::Boss1Phaseto2,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		m_fBossTimeElapsed = 0.f;
		m_bBoss_1Phaseto2CutsceneUpdate = false;
		CEventBus::GetInstance()->publish(phasechange);
	}
}

void CTriggerEventManager::Boss_2PhaseEnter(const float& fTimeDelta)
{
	if (!m_bBoss_2PhaseBegin) return;

	m_fBossTimeElapsed += fTimeDelta;

	if (m_fBossTimeElapsed >= 5.f/* 10.f*/)
	{
		EVENT phasechange
		{
			PHASE::PHASE_BEGIN,
			TOPIC::Boss2Phase,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		m_fBossTimeElapsed = 0.f;
		m_bBoss_2PhaseBegin = false;
		CEventBus::GetInstance()->publish(phasechange);
	}
}

void CTriggerEventManager::Free()
{

}