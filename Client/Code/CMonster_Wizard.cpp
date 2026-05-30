#include "pch.h"
#include "CGameObject.h"
#include "CMonster_Wizard.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CEventBus.h"

CMonster_Wizard::CMonster_Wizard(const CMonster_Wizard& rhs)
	:CMonster(rhs)
{
}

CMonster_Wizard::CMonster_Wizard(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Wizard::~CMonster_Wizard()
{
}

HRESULT CMonster_Wizard::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_MONSTER_WIZARD;
	Set_MonsterType(MONSTER_WIZARD);
	Set_MonsterRank(MONSTERRANK::RANK_COMMON);

	m_fSpeed = 2.f;
	m_fPlayerRecog_Distance = 10.f;
	m_fAttack_Distance = 3.f;

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,0.5f,0.f });


	m_eMonsterStatus = MONSTER_STATUS_SPAWN;

	Set_Animation();

	m_eMonsterStatus = MONSTER_STATUS_IDLE;


	Set_Animation();

	return S_OK;
}

_int CMonster_Wizard::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	// 상태 데드 or 아이스 상태면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS_DEAD 
		|| m_eMonsterStatus == MONSTER_STATUS_ICE)
	{
		return 0;
	}

	// 아이스 상태에서 죽을때

	if (m_eMonsterStatus == MONSTER_STATUS_ICE_DYING)
	{
		CSoundManager::GetInstance()->SoundPlay(L"ice_break.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
		Play_IceParticleSystem();
		if (this->Get_OriginType() == ORIGIN_DYNAMIC)
		{
			CObjectPoolingManager::GetInstance()->Return(this);
		}
		else
		{
			this->Set_Active(false);
		}
	}

	// 애니메이션 끝났는지 감지해서 자동으로 IDLE로 바꿔주기
	if (m_pAnimationCom->Get_IsAnimationEnd())
	{
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
	}


	return 0;
}

void CMonster_Wizard::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CMonster::LateUpdate_GameObject(fTimeDelta);

	// 상태 데드 or 아이스 상태면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DEAD
		|| m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ICE_DYING)
	{
		return;
	}

	// 노말하게 죽었을때
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DYING)
	{
		if (m_pBloodParticleSystem == nullptr)
		{
			Play_BloodParticleSystem();
		}
	}

	// 혹시라도 외부에서 상태 바꿧을수도 있으니 애니메이션 적용
	Set_Animation();

	

	switch (m_eCurrAttack)
	{
	case CMonster_Wizard::WIZARD_ATTACK_FIREBALL:
		Change_Attack_To_Buff(fTimeDelta);
		// 플레이어 따라다니게
		Chase_Player(fTimeDelta);
		// 공격 상태 감지하고 공격하게
		Attack_Fireball(fTimeDelta);
		break;
	case CMonster_Wizard::WIZARD_ATTACK_BUFF:
		Attack_Buff(fTimeDelta);
		break;
	case CMonster_Wizard::WIZARD_ATTACK_END:
		break;
	default:
		break;
	}

	
}

void CMonster_Wizard::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Wizard_Animation")->Set_Texture();
	m_pAnimationCom->Render_Buffer();

}

void CMonster_Wizard::Chase_Player(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_IDLE ||
		m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_MOVE ||
		m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK)
	{
		if (m_pTarget == NULL)
			return;

		// 일단 플레이어 컴포넌트 들고와
		m_pPlayerTransCom = m_pTarget->Get_Transform();

		// 플레이어 위치랑 몬스터 위치로 방향 벡터 만들기
		_vec3 monsterPos, PlayerPos, vDir;
		m_pTransform->Get_Info(INFO_POS, &monsterPos);
		monsterPos = { monsterPos.x, 0.f, monsterPos.z };
		m_pPlayerTransCom->Get_Info(INFO_POS, &PlayerPos);
		PlayerPos = { PlayerPos.x, 0.f, PlayerPos.z };

		vDir = PlayerPos - monsterPos;

		// 방향벡터의 길이를 구한다
		_float fLength = D3DXVec3Length(&vDir);

		// 플레이어가 너무 멀면 이동하지 않음
		if (fLength > m_fPlayerRecog_Distance)
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
			Set_Animation();
			return;
		}
		// 플레이어가 감지 거리 안에 들어오면
		else if (fLength <= m_fPlayerRecog_Distance)
		{
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_eMonsterStatus = MONSTER_STATUS_ATTACK;
				Set_Animation();
				return;
			}
			if (m_eMonsterStatus == MONSTER_STATUS_ATTACK)
			{
				return;
			}
			// 공격 사거리 안에 들어오면 이동하지 않고 공격
			if (fLength < m_fAttack_Distance)
			{
				return;
			}
			// 플레이어 방향으로 이동
			else
			{
				//m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE;
				//Set_Animation();
				//D3DXVec3Normalize(&vDir, &vDir); // 정규화
				//m_pTransform->Move_Pos(&vDir, m_fSpeed, fTimeDelta);
			}
		}
	}
}

void CMonster_Wizard::Set_Animation()
{
	// 상태 안바뀌었으면 걍 리턴
	if (m_eMonsterStatus == m_ePrevMonsterStatus)
	{
		return;
	}

	switch (m_eMonsterStatus)
	{
	case(MONSTER_STATUS::MONSTER_STATUS_SPAWN):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(0);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_IDLE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(0);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_MOVE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(4, 0.2f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_BUFF):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(7);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.2f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_ATTACK):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(2);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.11f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
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
		m_pTransform->Set_Scale(_vec3{ 0.7f, 0.7f, 0.7f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_ICE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(4);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(1, 0.15f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_DYING):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(5);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(6, 0.15f);	// 몇 열이고 출력 속도 몇인지.
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DEAD;
		break;
	}
	default:
		break;
	}

	m_ePrevMonsterStatus = m_eMonsterStatus; // 이전 상태로 변경
}

void CMonster_Wizard::Attack_Fireball(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS_ATTACK)
	{
		if (m_pAnimationCom->Get_CurrFrame() == 4
			&& m_bIsBulletFired == false)
		{
			m_pWeaponCom->Fire();
			m_bIsBulletFired = true;
		}
		if (m_pAnimationCom->Get_CurrFrame() == 5)
		{
			m_bIsBulletFired = false;
		}
		if (m_pAnimationCom->Get_CurrFrame() == 7)
		{
			m_fAttackTimer = 0.f;
		}

		if (m_bMonsterSpawn == false)
		{
			EVENT event
			{
				 PHASE::PHASE_BEGIN,
				 TOPIC::MONSTERSPAWN,
				 CHANNEL::Monster,
				 this, nullptr, m_tHandle, {}
			};
				CEventBus::GetInstance()->publish(event);
				m_bMonsterSpawn = true;
		}
	}
	
}

HRESULT CMonster_Wizard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWeaponCom->Set_MyOwner(this);
	m_pWeaponCom->Weapon_Inintialize();
	m_pWeaponCom->Change_Weapon(WEAPON_WIZARD);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon",pComponent });


	return S_OK;
}

CMonster_Wizard* CMonster_Wizard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Wizard* pMonster = new CMonster_Wizard(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Goblin Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Wizard::Free()
{
	auto pool = CObjectPoolingManager::GetInstance();
	pool->Return(m_pBloodParticleSystem);
	pool->Return(m_pIceParticleSystem);
	Engine::CMonster::Free();
}


void CMonster_Wizard::Change_Attack_To_Buff(const _float& fTimeDelta)
{
	m_fAttackChangeTimer += fTimeDelta;

	if (m_fAttackChangeTimer > m_fAttackChangeInterval)
	{
		m_fAttackChangeTimer = 0.f;
		m_eCurrAttack = WIZARD_ATTACK_BUFF;
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_BUFF;
		Set_Animation();

		_vec3 pPlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
		m_fMaxPosY = pPlayerPos.y - 0.5f + 0.7f;
		m_fMinPosY = pPlayerPos.y - 0.5f;
	}
}

void CMonster_Wizard::Attack_Buff(const _float& fTimeDelta)
{
	m_fAttackChangeTimer += fTimeDelta;
	if (!m_bIsUp)
	{
		Move_Up(fTimeDelta);
	}
	
	if (m_fAttackChangeTimer > m_fAttackChangeInterval)
	{
		Move_Down(fTimeDelta);
	}
}

void CMonster_Wizard::Move_Up(const _float& fTimeDelta)
{
	if (m_pTransform->GetInfoPos()->y < m_fMaxPosY)
	{
		_vec3 Dir = { 0.f, 1.f, 0.f };
		m_pTransform->Move_Pos(&Dir, m_fSpeed_UpDown, fTimeDelta);
		CSoundManager::GetInstance()->SoundPlay(L"lich-summon-start.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
	}
	else
	{
		m_bIsUp = true;

		// 몬스터한테 버프 먹이기
		BuffMonsters();
		return;
	}
	
}

void CMonster_Wizard::Move_Down(const _float& fTimeDelta)
{
	if (m_pTransform->GetInfoPos()->y >= m_fMinPosY)
	{
		CSoundManager::GetInstance()->SoundPlay(L"lich-summon.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
		_vec3 DirDown = { 0.f, -1.f, 0.f };
		m_pTransform->Move_Pos(&DirDown, m_fSpeed_UpDown, fTimeDelta);
	}
	else
	{
		m_eCurrAttack = WIZARD_ATTACK_FIREBALL;
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
		Set_Animation();
		m_fAttackChangeTimer = 0.f;
		m_bIsUp = false;

		DebuffMonsters();
		return;
	}
}

void CMonster_Wizard::BuffMonsters()
{
	vector TmpVec_fb = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_GOBLIN_FIREBALL);
	for (auto mon : TmpVec_fb)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_gh = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_GOBLIN_HAMMER);
	for (auto mon : TmpVec_gh)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_oa = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_ORGE_AXE);
	for (auto mon : TmpVec_oa)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_rat = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_RAT);
	for (auto mon : TmpVec_rat)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_ss = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_SKELETON_SWORD);
	for (auto mon : TmpVec_ss)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	// ============================ 풀링도 참여할게요 =================================

	vector TmpVec_fb1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_GOBLIN_FIREBALL);
	for (auto mon : TmpVec_fb1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_gh1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_GOBLIN_HAMMER);
	for (auto mon : TmpVec_gh1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_oa1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_ORGE_AXE);
	for (auto mon : TmpVec_oa1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_rat1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_RAT);
	for (auto mon : TmpVec_rat1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}

	vector TmpVec_ss1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_SKELETON_SWORD);
	for (auto mon : TmpVec_ss1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(true);
	}
}

void CMonster_Wizard::DebuffMonsters()
{
	vector TmpVec_fb = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_GOBLIN_FIREBALL);
	for (auto mon : TmpVec_fb)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_gh = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_GOBLIN_HAMMER);
	for (auto mon : TmpVec_gh)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_oa = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_ORGE_AXE);
	for (auto mon : TmpVec_oa)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_rat = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_RAT);
	for (auto mon : TmpVec_rat)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_ss = CObjectManager::GetInstance()->Get_VisibleObjectList(OBJ_MONSTER_SKELETON_SWORD);
	for (auto mon : TmpVec_ss)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}


	// ===================== 풀링도 참여할게요 ========================


	vector TmpVec_fb1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_GOBLIN_FIREBALL);
	for (auto mon : TmpVec_fb1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_gh1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_GOBLIN_HAMMER);
	for (auto mon : TmpVec_gh1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_oa1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_ORGE_AXE);
	for (auto mon : TmpVec_oa1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_rat1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_RAT);
	for (auto mon : TmpVec_rat1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}

	vector TmpVec_ss1 = CObjectPoolingManager::GetInstance()->Get_DynamicObjectList(OBJ_MONSTER_SKELETON_SWORD);
	for (auto mon : TmpVec_ss1)
	{
		static_cast<CMonster*>(mon)->Set_IsBuffed(false);
	}


}

void CMonster_Wizard::Play_BloodParticleSystem()
{
	m_pBloodParticleSystem = dynamic_cast<CParticleSystem_Blood*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BLOOD));
	m_pBloodParticleSystem->Set_Target(this);
	m_pBloodParticleSystem->Activate_ParticleSystem();
	m_pBloodParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CMonster_Wizard::Play_IceParticleSystem()
{
	m_pIceParticleSystem = dynamic_cast<CParticleSystem_Ice*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_ICE));
	m_pIceParticleSystem->Set_Target(this);
	m_pIceParticleSystem->Activate_ParticleSystem();
	m_pIceParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}
