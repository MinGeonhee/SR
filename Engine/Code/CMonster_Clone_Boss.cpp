#include "CGameObject.h"
#include "CMonster_Clone_Boss.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CItem.h"
#include "CMonster_Boss.h"
#include "CSceneManager.h"
#include "CSoundManager.h"

CMonster_Clone_Boss::CMonster_Clone_Boss(const CMonster_Clone_Boss& rhs)
	:CMonster(rhs)
{
}

CMonster_Clone_Boss::CMonster_Clone_Boss(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Clone_Boss::~CMonster_Clone_Boss()
{
}

HRESULT CMonster_Clone_Boss::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_tHandle.m_eType = OBJ_MONSTER_BOSS_CLONE;
	Set_MonsterType(MONSTER_BOSS);
	Set_MonsterRank(MONSTERRANK::RANK_BOSS);

	m_fSpeed = 5.f;
	m_fPlayerRecog_Distance = 10.f;
	m_fAttack_Distance = 2.5f;

	
	m_pTransform->Set_Scale(_vec3{ 2.f, 2.f, 2.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,1.5f,0.f });
	
	// 체력 200으로 바꿔놓음
	STATUS st = { 10.f,10.f, 10.f, 3.f, 0.f };	// MaxHp, CurHP, Attack, Defence, Speed
	m_pStatusCom->Set_Status(st);

	m_eMonsterStatus = MONSTER_STATUS_TELEPORT;
	Move_To_Random_Pos();
	Set_Animation();

	m_pTarget = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);

	return S_OK;
}

_int CMonster_Clone_Boss::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	// 상태 데드면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS_DEAD)
	{
		m_eMonsterStatus = MONSTER_STATUS_IDLE;
		static_cast<CMonster_Boss*>(m_pTargetBoss)->Minus_CurrCloneBossNum();
		CObjectPoolingManager::GetInstance()->Return(this);
		return 0;
	}

	if (static_cast<CMonster_Boss*>(m_pTargetBoss)->Get_MonsterStatus() == MONSTER_STATUS_DYING)
	{
		m_eMonsterStatus = MONSTER_STATUS_IDLE;
		CObjectPoolingManager::GetInstance()->Return(this);
		return 0;
	}

	// 애니메이션 끝났는지 감지해서 자동으로 IDLE로 바꿔주기
	if (m_pAnimationCom->Get_IsAnimationEnd())
	{
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
	}

	
	return 0;
}

void CMonster_Clone_Boss::LateUpdate_GameObject(const _float& fTimeDelta)
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
		CSoundManager::GetInstance()->SoundPlay(L"cultist-teleport-out.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 0.5);
		Set_Animation();
		if (m_pAnimationCom->Get_CurrFrame() == 7)
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DEAD;
		}
		return;
	}

	// 공격 맞으면 hit 상태만 유지하게
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_HIT)
	{
		Set_Animation();
		return;
	}

	if (m_eMonsterStatus != MONSTER_STATUS_TELEPORT)
	{
		m_eMonsterStatus = MONSTER_STATUS_ATTACK;
		Set_Animation();

		if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK)
		{
			m_fAttackTimer += fTimeDelta;
			if (m_fAttackTimer > m_fAttackTime)
			{
				m_pIceBallWeaponCom->Fire();
				m_fAttackTimer = 0.f;
			}
		}
	}
	
	
	
}

void CMonster_Clone_Boss::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Boss_Animation")->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}



void CMonster_Clone_Boss::Move_To_Random_Pos()
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


	uniform_real_distribution<float> rd_X(xpos - m_vRandomPosRange_XZ.x / 2, xpos + m_vRandomPosRange_XZ.x / 2);
	uniform_real_distribution<float> rd_Z(zpos - m_vRandomPosRange_XZ.y / 2, zpos + m_vRandomPosRange_XZ.y / 2);

	// 랜덤 사이즈 생성
	float fXPos = rd_X(rng);
	float fZPos = rd_Z(rng);

	m_pTransform->Set_Pos(fXPos, ypos, fZPos);
}



void CMonster_Clone_Boss::Set_Animation()
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
		m_pAnimationCom->SetRow(6);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(7, 0.08f);	// 몇 열이고 출력 속도 몇인지
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
		m_pAnimationCom->SetRow(5);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 2.8f, 2.8f, 2.8f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	default:
		break;
	}

	m_ePrevMonsterStatus = m_eMonsterStatus; // 이전 상태로 변경
}

HRESULT CMonster_Clone_Boss::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pIceBallWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pIceBallWeaponCom->Set_MyOwner(this);
	m_pIceBallWeaponCom->Weapon_Inintialize();
	m_pIceBallWeaponCom->Change_Weapon(WEAPON_BOSS_ICEBALL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon1",pComponent });

	

	return S_OK;
}

CMonster_Clone_Boss* CMonster_Clone_Boss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Clone_Boss* pMonster = new CMonster_Clone_Boss(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Boss Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Clone_Boss::Free()
{
	Engine::CMonster::Free();
}



