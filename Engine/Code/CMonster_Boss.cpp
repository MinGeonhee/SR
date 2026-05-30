#include "CGameObject.h"
#include "CMonster_Boss.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CItem.h"
#include "CMonster_Clone_Boss.h"
#include "CSceneManager.h"
#include "CEventBus.h"
#include "CSoundManager.h"
#include "CActionCamera_Helper.h"

CMonster_Boss::CMonster_Boss(const CMonster_Boss& rhs)
	:CMonster(rhs)
{
}

CMonster_Boss::CMonster_Boss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Boss::~CMonster_Boss()
{
}

HRESULT CMonster_Boss::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_tHandle.m_eType = OBJ_MONSTER_BOSS;
	Set_MonsterType(MONSTER_BOSS);
	Set_MonsterRank(MONSTERRANK::RANK_BOSS);

	m_fSpeed = 5.f;
	m_fPlayerRecog_Distance = 10.f;
	m_fAttack_Distance = 2.5f;

	
	m_pTransform->Set_Scale(_vec3{ 2.f, 2.f, 2.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,1.5f,0.f });
	
	// 체력 200으로 바꿔놓음
	STATUS st = { 200.f,200.f, 10.f, 3.f, 0.f };	// MaxHp, CurHP, Attack, Defence, Speed
	m_pStatusCom->Set_Status(st);

	m_iCurrCloneBossNum = m_iMaxCloneBossNum;

	Set_Animation();

	// ==================== 등장 이펙트 초기화 ====================
	for (int i = 0; i < m_iAppearEffectNum; ++i)
	{
		CBossAppearEffect* pEffect = CBossAppearEffect::Create(m_pGraphicDev);
		pEffect->Get_Transform()->Set_Angle(0.f, (45 + (90.f * i)) * (3.14f / 180.f), 0.f);
		_vec3 vPos = { 26.5f, 2.f, 42.5f };
		switch (i)
		{
		case 0:
			pEffect->Get_Transform()->Set_Pos(vPos.x + 0.1f, -1.f, vPos.z + 0.1f);
			break;
		case 1:
			pEffect->Get_Transform()->Set_Pos(vPos.x + 0.1f, -1.f, vPos.z - 0.1f);
			break;
		case 2:
			pEffect->Get_Transform()->Set_Pos(vPos.x - 0.1f, -1.f, vPos.z - 0.1f);
			break;
		case 3:
			pEffect->Get_Transform()->Set_Pos(vPos.x - 0.1f, -1.f, vPos.z + 0.1f);
			break;
		default:
			break;
		}
		

		m_vecBossAppearEffect.push_back(pEffect);
	}

	return S_OK;
}

_int CMonster_Boss::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);
	Phase_Update();

	// 상태 데드면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS_DEAD)
	{
		return 0;
	}

	// 애니메이션 끝났는지 감지해서 자동으로 IDLE로 바꿔주기
	if (m_pAnimationCom->Get_IsAnimationEnd())
	{
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
	}

	// ==================== 등장 이펙트 업데이트 ==================== 
	if (m_eCurrPhase == BOSS_PHASE_ONE ||
		m_eCurrPhase == BOSS_PHASE_INTRO)
	{
		for (int i = 0; i < m_iAppearEffectNum; i++)
		{
			if (m_vecBossAppearEffect[i]->Get_Active())
			{
				m_vecBossAppearEffect[i]->Update_GameObject(fTimeDelta);
			}
		}
	}
	

	return 0;
}

void CMonster_Boss::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CMonster::LateUpdate_GameObject(fTimeDelta);

	
	// 상태 데드면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DEAD)
	{
		return;
	}

	// 노말하게 죽었을때
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DYING)
	{
		Set_Animation();
		if (m_pBloodParticleSystem == nullptr)
		{
			Play_BloodParticleSystem();
		}
		Drop_Item(fTimeDelta);
		return;
	}

	// 공격 맞으면 hit 상태만 유지하게
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_HIT)
	{
		Set_Animation();
		return;
	}
	
	// 현재 페이즈 체크
	Phase_Check();

	// 보스 페이즈별로 공격 함수 부르기
	switch (m_eCurrPhase)
	{
	case BOSS_PHASE_INTRO:
		break;

	case BOSS_PHASE_ONE:
		Attack_Phase_One(fTimeDelta);
		break;

	case BOSS_PHASE_CUTSCENE_1_TO_2:
		break;

	case BOSS_PHASE_TWO:
		Attack_Phase_Two(fTimeDelta);
		break;
	default:
		break;
	}
	
	// ==================== 등장 이펙트 업데이트 ====================
	if (m_eCurrPhase == BOSS_PHASE_ONE || 
		m_eCurrPhase == BOSS_PHASE_INTRO)
	{
		for (int i = 0; i < m_iAppearEffectNum; i++)
		{
			if (m_vecBossAppearEffect[i]->Get_Active())
			{
				m_vecBossAppearEffect[i]->LateUpdate_GameObject(fTimeDelta);
			}
		}
		if (m_bTurnOffAppearEffect_Trigger)
		{
			TurnOff_AppearEffect(fTimeDelta);
		}
	}
}

void CMonster_Boss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Boss_Animation")->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

void CMonster_Boss::Phase_Check()
{
	// 페이즈 두번째로 넘어가면 그냥 체크 안하게
	if(m_eCurrPhase == BOSS_PHASE_TWO)
	{
		return;
	}
	// 현재 보스 몬스터 체력이 50% 이하면 다음 페이즈로
	if ((m_pStatusCom->Get_CurrentHp() < m_pStatusCom->Get_Status().fMaxHp / 2.f) && m_bEventpublish == false)
	{
		EVENT phaseChange
		{
			PHASE::PHASE_BEGIN,
			TOPIC::Boss1Phaseto2,
			CHANNEL::Boss,
			nullptr, nullptr, {}, {}
		};
		CEventBus::GetInstance()->publish(phaseChange);
		m_bEventpublish = true;
	}
}


void CMonster_Boss::Change_Attack()
{
	int i = static_cast<int>(m_eCurrBossAttack);

	switch (m_eCurrPhase)
	{
	// 페이즈 1이면 
	// 0부터 BOSS_ATTACK_ONE_END 까지의 공격 패턴만 돌게 하기
	case BOSS_PHASE_ONE:
		if (i < static_cast<int>(BOSS_ATTACK_ONE_END)-1)
		{
			i++;
		}
		else
		{
			i = 0;
		}
		break;

	// 페이즈 2이면 
	// BOSS_ATTACK_TELEPORT부터 BOSS_ATTACK_TWO_END 까지의 공격 패턴만 돌게 하기
	case BOSS_PHASE_TWO:
		if (i < static_cast<int>(BOSS_ATTACK_TELEPORT))
		{
			i = BOSS_ATTACK_TELEPORT;
		}


		if (i < static_cast<int>(BOSS_ATTACK_TWO_END) - 1)
		{
			i++;
		}
		else
		{
			i = BOSS_ATTACK_TELEPORT;
		}
		break;
	}
	
	m_eCurrBossAttack = static_cast<BOSS_ATTACK>(i);
}

void CMonster_Boss::Attack_Phase_One(const _float& fTimeDelta)
{
	//m_eMonsterStatus = MONSTER_STATUS_ATTACK;

	

	Teleport_Attack(fTimeDelta);
	
	Set_Animation();

	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK)
	{
		m_fBossAttackChangeTimer += fTimeDelta;
		if (m_fBossAttackChangeTimer > m_fBossAttackChangeInterval)
		{
			Change_Attack();
			m_fBossAttackChangeTimer = 0.f;
			m_fAttackTimer = 0.f;
		}

		switch (m_eCurrBossAttack)
		{
		//case CMonster_Boss::BOSS_ATTACK_ICEBALL:
		//	m_fAttackTimer += fTimeDelta;
		//	if (m_fAttackTimer > m_fAttackTime)
		//	{
		//		m_pIceBallWeaponCom->Fire();
		//		m_fAttackTimer = 0.f;
		//	}
		//	break;
		case CMonster_Boss::BOSS_ATTACK_THUNDER_DROP:
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_pThunderDropWeaponCom->Fire();
				m_fAttackTimer = 0.f;
			}
			break;
		case CMonster_Boss::BOSS_ATTACK_WIND:
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_pWindWeaponCom->Fire();
				m_fAttackTimer = 0.f;
			}
			break;
		case CMonster_Boss::BOSS_ATTACK_ICEPILLAR:
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_pIcePillarWeaponCom->Fire();
				m_fAttackTimer = 0.f;
			}
			break;

		default:
			break;
		}
		
	}
}

void CMonster_Boss::Attack_Phase_Two(const _float& fTimeDelta)
{
	if (m_eMonsterStatus != MONSTER_STATUS_TELEPORT)
	{
		m_eMonsterStatus = MONSTER_STATUS_ATTACK;
	}
	if (m_eCurrBossAttack <= BOSS_ATTACK_ONE_END)
	{
		m_eCurrBossAttack = BOSS_ATTACK_TELEPORT;
	}

	Set_Animation();

	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK ||
		m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_TELEPORT)
	{
		switch (m_eCurrBossAttack)
		{
		case CMonster_Boss::BOSS_ATTACK_TELEPORT:
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_pThunderDropWeaponCom->Fire();
				m_fAttackTimer = 0.f;
			}
			Teleport_Attack(fTimeDelta);
			break;
		case CMonster_Boss::BOSS_ATTACK_CLONE:
			Clone_Attack(fTimeDelta);
			break;
		default:
			break;
		}
	}
}

void CMonster_Boss::Teleport_Attack(const _float& fTimeDelta)
{
	m_fBossAttackChangeTimer += fTimeDelta;
	int iTeleportChangeInterval = 12;
	if (m_fBossAttackChangeTimer > iTeleportChangeInterval)
	{
		Change_Attack();
		m_fBossAttackChangeTimer = 0.f;
		m_fAttackTimer = 0.f;
	}


	m_fTeleportTimer += fTimeDelta;
	if (m_fTeleportTimer > m_fTeleportInterval)
	{
		m_eMonsterStatus = MONSTER_STATUS_TELEPORT;
		CSoundManager::GetInstance()->SoundPlay(L"cultist-teleport-in.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 0.5);
		Set_Animation();
		m_fTeleportTimer = 0.f;
		m_fAttackTimer = 0.f;
	}

	if (m_eMonsterStatus == MONSTER_STATUS_TELEPORT)
	{
		if (m_pAnimationCom->Get_CurrFrame() == 7)
		{
			m_eMonsterStatus = MONSTER_STATUS_ATTACK;
			Set_Animation();
			Move_To_Random_Pos();
		}
	}

	if (m_eMonsterStatus == MONSTER_STATUS_ATTACK)
	{
		m_fAttackTimer += fTimeDelta;
		if (m_fAttackTimer > m_fAttackTime)
		{
			m_pIceBallWeaponCom->Fire();
			m_fAttackTimer = 0.f;
		}
	}
}

void CMonster_Boss::Clone_Attack(const _float& fTimeDelta)
{
	m_fAttackTimer += fTimeDelta;
	if (m_fAttackTimer > m_fAttackTime)
	{
		m_pWindWeaponCom->Fire();
		m_fAttackTimer = 0.f;
	}

	if (m_bIsClonedBoss == false)
	{
		for (int i = 0; i < m_iMaxCloneBossNum; i++)
		{
			CGameObject* p = CObjectPoolingManager::GetInstance()->Get(OBJ_MONSTER_BOSS_CLONE);
			static_cast<CMonster_Clone_Boss*>(p)->Move_To_Random_Pos();
			static_cast<CMonster_Clone_Boss*>(p)->Set_Target(CObjectManager::GetInstance()->Get(OBJ_PLAYER));
			static_cast<CMonster_Clone_Boss*>(p)->Set_TargetBoss(this);
			static_cast<CMonster_Clone_Boss*>(p)->Set_MonsterStatus(MONSTER_STATUS_TELEPORT);
			static_cast<CMonster_Clone_Boss*>(p)->Set_Animation();
		}
		m_bIsClonedBoss = true;
	}
	else
	{
		if (m_iCurrCloneBossNum <= 0)
		{
			Change_Attack();
			m_fBossAttackChangeTimer = 0.f;
			m_fAttackTimer = 0.f;
			m_iCurrCloneBossNum = m_iMaxCloneBossNum;
			m_bIsClonedBoss = false;
		}
	}
}

void CMonster_Boss::Move_To_Random_Pos()
{
	// 시드 생성 (시간 기반으로 고유하게 생성)
	unsigned seed = static_cast<unsigned>(
		chrono::high_resolution_clock::now().time_since_epoch().count());
	mt19937 rng(seed);  // 난수 생성기
	float xpos = 0.f; float ypos = 0.f; float zpos = 0.f;

	// 랜덤 사이즈 범위 지정
	if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS)
	{
		xpos = 26.5f; ypos = 2.3f; zpos = 42.5f;
	}

	else if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS2)
	{
		xpos = 48.5; ypos = 3.3f; zpos = 49.5;
	}

	
	uniform_real_distribution<float> rd_X(xpos - m_vRandomPosRange_XZ.x / 2, xpos + m_vRandomPosRange_XZ.x/2);
	uniform_real_distribution<float> rd_Z(zpos - m_vRandomPosRange_XZ.y / 2, zpos + m_vRandomPosRange_XZ.y/2);

	// 랜덤 사이즈 생성
	float fXPos = rd_X(rng);
	float fZPos = rd_Z(rng);

	m_pTransform->Set_Pos(fXPos, ypos, fZPos);
}



void CMonster_Boss::Set_Animation()
{
	// 상태 안바뀌었으면 걍 리턴
	if (m_eMonsterStatus == m_ePrevMonsterStatus)
	{
		return;
	}

	switch (m_eMonsterStatus)
	{
	case(MONSTER_STATUS::MONSTER_STATUS_IDLE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(0);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(6, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pTransform->Set_Scale(_vec3{ 2.8f, 2.8f, 2.8f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_TELEPORT):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(5);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 2.8f, 2.8f, 2.8f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_ATTACK):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(2);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(6, 0.15f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pTransform->Set_Scale(_vec3{ 2.8f, 2.8f, 2.8f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_HIT):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(3);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(1, 0.3f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 2.7f, 2.7f, 2.7f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_DYING):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(4);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(3, 0.15f);	// 몇 열이고 출력 속도 몇인지.
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 2.8f, 2.8f, 2.8f });
		
		break;
	}
	default:
		break;
	}

	m_ePrevMonsterStatus = m_eMonsterStatus; // 이전 상태로 변경
}

HRESULT CMonster_Boss::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pIceBallWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pIceBallWeaponCom->Set_MyOwner(this);
	m_pIceBallWeaponCom->Weapon_Inintialize();
	m_pIceBallWeaponCom->Change_Weapon(WEAPON_BOSS_ICEBALL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon1",pComponent });



	pComponent = m_pIcePillarWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pIcePillarWeaponCom->Set_MyOwner(this);
	m_pIcePillarWeaponCom->Weapon_Inintialize();
	m_pIcePillarWeaponCom->Change_Weapon(WEAPON_BOSS_ICEPILLAR);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon2",pComponent });


	pComponent = m_pThunderDropWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pThunderDropWeaponCom->Set_MyOwner(this);
	m_pThunderDropWeaponCom->Weapon_Inintialize();
	m_pThunderDropWeaponCom->Change_Weapon(WEAPON_BOSS_THUNDER_DROP);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon3",pComponent });


	pComponent = m_pWindWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWindWeaponCom->Set_MyOwner(this);
	m_pWindWeaponCom->Weapon_Inintialize();
	m_pWindWeaponCom->Change_Weapon(WEAPON_BOSS_WIND);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon4",pComponent });
}

CMonster_Boss* CMonster_Boss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Boss* pMonster = new CMonster_Boss(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Boss Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Boss::Free()
{
	auto pool = CObjectPoolingManager::GetInstance();
	pool->Return(m_pBloodParticleSystem);

	for (auto& effect : m_vecBossAppearEffect)
		Safe_Release(effect);
	
	m_vecBossAppearEffect.clear();

	Engine::CMonster::Free();
}



void CMonster_Boss::Play_BloodParticleSystem()
{
	m_pBloodParticleSystem = dynamic_cast<CParticleSystem_Blood*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BLOOD));
	m_pBloodParticleSystem->Set_Target(this);
	m_pBloodParticleSystem->Activate_ParticleSystem();
	m_pBloodParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CMonster_Boss::Drop_Item(const _float& fTimeDelta)
{
	m_fItemTimer += fTimeDelta;
	if(m_fItemTimer > m_fItemSpawnTimeInterval)
	{
		if (m_iItemSpawnCurrNum <= m_iItemSpawnNum)
		{
			CItem* pItem = static_cast<CItem*>(CObjectPoolingManager::GetInstance()->Get(OBJ_ITEM));
			pItem->Set_IsDropItem(true);
			pItem->Set_TargetTransform(m_pTransform);
			
			m_fItemTimer = 0.f;
			m_iItemSpawnCurrNum++;
		}
		else
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DEAD;
		}
	}
}

void CMonster_Boss::TurnOff_AppearEffect(const _float& fTimeDelta)
{
	if (m_iCurrTurnOffAppearEffect < m_iAppearEffectNum)
	{
		m_fTurnOffTimer += fTimeDelta;
		if (m_fTurnOffTimer > m_fTurnOffInterval)
		{
			m_vecBossAppearEffect[m_iCurrTurnOffAppearEffect]->Set_Active(false);
			m_fTurnOffTimer = 0.f;
			m_iCurrTurnOffAppearEffect++;
		}
	}
	else
	{
		return;
	}
}



void CMonster_Boss::Phase_Update()
{
	m_eCurrPhase = CBossSystem::GetInstance()->Get_BossPhase();
}
