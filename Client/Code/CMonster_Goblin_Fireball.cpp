#include "pch.h"
#include <random> 
#include <chrono>
#include "CGameObject.h"
#include "CMonster_Goblin_Fireball.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
CMonster_Goblin_Fireball::CMonster_Goblin_Fireball(const CMonster_Goblin_Fireball& rhs)
	:CMonster(rhs)
{
}

CMonster_Goblin_Fireball::CMonster_Goblin_Fireball(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Goblin_Fireball::~CMonster_Goblin_Fireball()
{
}

HRESULT CMonster_Goblin_Fireball::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_MONSTER_GOBLIN_FIREBALL;
	Set_MonsterType(MONSTER_GOBLIN_FIREBALL);
	Set_MonsterRank(MONSTERRANK::RANK_COMMON);

	m_fPlayerRecog_Distance = 30.f;
	m_fAttack_Distance = 5.f;

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,0.5f,0.f });

	m_eMonsterStatus = MONSTER_STATUS_SPAWN;

	Set_Animation();

	
	return S_OK;
}

_int CMonster_Goblin_Fireball::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	// 상태 데드 or 아이스 상태면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS_DEAD
		|| m_eMonsterStatus == MONSTER_STATUS_ICE)
	{
		return 0;
	}

	// 스폰 상태 
	if (!m_bIsSpawned)
	{
		m_eMonsterStatus = MONSTER_STATUS_SPAWN;
	}
	if (m_eMonsterStatus == MONSTER_STATUS_SPAWN)
	{
		if (m_pAnimationCom->Get_IsAnimationEnd())
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
			m_bIsSpawned = true;
		}
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


	// 몬스터랑 플레이어 사이 방향벡터 구해줌
	Get_Vector_Between_Player_Monster();

	return 0;
}

void CMonster_Goblin_Fireball::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CMonster::LateUpdate_GameObject(fTimeDelta);

	// 상태 데드 or 아이스 상태면 걍 리턴
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DEAD
		|| m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ICE_DYING)
	{
		return;
	}

	if (m_eMonsterStatus == MONSTER_STATUS_SPAWN)
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

	
	Chase_Player(fTimeDelta);
	
	Manage_Jump(fTimeDelta);

	Attack_Fireball(fTimeDelta);
	
	// 전역적으로 애니메이션 체인지 되는거 체크
	Set_Animation();
}

void CMonster_Goblin_Fireball::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Goblin_Fireball_Animation")->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

void CMonster_Goblin_Fireball::Get_Vector_Between_Player_Monster()
{
	if (m_pTarget == NULL) return; // 방어 코드 작성

	_vec3 vMonsterPos, vPlayerPos;
	m_pTransform->Get_Info(INFO_POS, &vMonsterPos);
	vMonsterPos = { vMonsterPos.x, 0.f, vMonsterPos.z };

	m_pPlayerTransCom = m_pTarget->Get_Transform();
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos = { vPlayerPos.x, 0.f, vPlayerPos.z };

	m_vFrom_Monster_To_Player = vPlayerPos - vMonsterPos;
}

void CMonster_Goblin_Fireball::Chase_Player(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_IDLE ||
		m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_MOVE)
	{
		if (m_pTarget == NULL)
			return;

		Get_Vector_Between_Player_Monster();

		// 몬스터와 플레이어 길이를 구한다
		_float fLength = D3DXVec3Length(&m_vFrom_Monster_To_Player);

		// 플레이어가 너무 멀면 이동하지 않음
		if (fLength > m_fPlayerRecog_Distance)
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
			return;
		}
		// 플레이어가 감지 거리 안에 들어오면
		else if (fLength <= m_fPlayerRecog_Distance)
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE;
			// 다음 함수에서 애니메이션 프레임 갖고 판단하는거 있어서
			// 바뀐 상태값 기준으로 셋 애니메이션 해주기
			Set_Animation();
			D3DXVec3Normalize(&m_vCurrDirVector, &m_vCurrDirVector); // 정규화
			m_pTransform->Move_Pos(&m_vCurrDirVector, m_fSpeed, fTimeDelta);
			_vec3 vPos = *(m_pTransform->GetInfoPos());
			_vec3 vPlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
			m_pTransform->Set_Pos(vPos.x, vPlayerPos.y - 0.5f, vPos.z);
		}
	}
}

void CMonster_Goblin_Fireball::Manage_Jump(const _float& fTimeDelta)
{
	// 이동(점프) 상태일때
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_MOVE)
	{
		// 점프 한번 끝나면
		if (m_pAnimationCom->Get_IsAnimationEnd())
		{
			// 카운트 올려주고 아이들 상태로 바꿔줌 
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE_IDLE;
			Set_Animation();
			m_iCurrJumpCount++;
			return;
		}
	}
	// 무브 아이들 상태라면
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_MOVE_IDLE)
	{
		m_fMovingIdleTimer += fTimeDelta * 1.f;

		// 무브 아이들 타이머 끝나면
		if (m_fMovingIdleTimer > m_fMovingIdleTime)
		{
			// 점프 카운트가 맥스를 넘어가지 않았다면
			if (m_iCurrJumpCount < m_iMaxJumpCount)
			{
				// 무브 상태로 다시 바꿔주기
				m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE;
				Set_Animation();
				m_fMovingIdleTimer = 0.f;
			}
			// 점프 카운트가 맥스를 넘어갔다면
			else
			{
				// 플레이어와 몬스터가 너무 가깝다면
				if (D3DXVec3Length(&m_vFrom_Monster_To_Player) < 4.f)
				{
					// 다음 방향을 뒤로 가게 세팅
					m_vCurrDirVector = -m_vFrom_Monster_To_Player;
				}
				else
				{
					// 랜덤 방향 생성
					Make_New_Dir();
				}
				// 공격 상태로 바꿔주기
				m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_ATTACK;
				Set_Animation();
				m_iCurrJumpCount = 0;
				m_fMovingIdleTimer = 0.f;
			}

			m_fMovingIdleTimer = 0.f;
		}
	}
}

void CMonster_Goblin_Fireball::Make_New_Dir()
{
	// 시드 생성 (시간 기반으로 고유하게 생성)
	unsigned seed = static_cast<unsigned>(
		chrono::high_resolution_clock::now().time_since_epoch().count());
	mt19937 rng(seed);  // 난수 생성기

	// 0~3 으로 범위 지정
	uniform_real_distribution<float> randDir(0, 2.999f);

	// 현재 방향 어디인지 변수에 넣어줌
	int iRandDir = (int)(randDir(rng));
	m_eCurrDir = static_cast<MOVING_DIR>(iRandDir);

	_vec3 vRightDirVec, vUp, vLook;
	switch (m_eCurrDir)
	{
	case MOVING_DIR_FRONT:
		m_vCurrDirVector = m_vFrom_Monster_To_Player;
		break;
	case MOVING_DIR_RIGHT:
		// 룩벡터랑 업벡터 외적으로 오른쪽 벡터 구하기
		vLook = m_vFrom_Monster_To_Player;
		m_pTransform->Get_Info(INFO_UP, &vUp);
		D3DXVec3Cross(&vRightDirVec, &vUp, &vLook);
		m_vCurrDirVector = vRightDirVec;
		break;
	case MOVING_DIR_LEFT:
		vLook = m_vFrom_Monster_To_Player;
		m_pTransform->Get_Info(INFO_UP, &vUp);
		D3DXVec3Cross(&vRightDirVec, &vUp, &vLook);
		m_vCurrDirVector = -vRightDirVec;
		break;
	case MOVING_DIR_BACK:
		m_vCurrDirVector = -m_vFrom_Monster_To_Player;
		break;
	}
}

void CMonster_Goblin_Fireball::Attack_Fireball(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK)
	{
		m_fAttackTimer += fTimeDelta * 1.f;
		if (m_pAnimationCom->Get_CurrFrame() == 5
			&& m_bIsBulletFired == false)
		{
			CSoundManager::GetInstance()->SoundPlay(L"goblin-caster-attack.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
			m_pWeaponCom->Fire();
			m_bIsBulletFired = true;
		}
		if (m_pAnimationCom->Get_CurrFrame() == 6)
		{
			m_bIsBulletFired = false;
		}
	}
}

void CMonster_Goblin_Fireball::Set_Animation()
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
		m_pAnimationCom->SetRow(6);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.11f);	// 몇 열이고 출력 속도 몇인지
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
		m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_MOVE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_MOVE_IDLE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(1, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_ATTACK):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(2);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.15f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
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
		m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pAnimationCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		// 다잉일때는 바로 데드로 바꿔주기
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DEAD;
		break;
	}
	default:
		break;
	}

	m_ePrevMonsterStatus = m_eMonsterStatus;
}

HRESULT CMonster_Goblin_Fireball::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWeaponCom->Set_MyOwner(this);
	m_pWeaponCom->Weapon_Inintialize();
	m_pWeaponCom->Change_Weapon(WEAPON_GOBLIN_FIREBALL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon",pComponent });

	return S_OK;
}

CMonster_Goblin_Fireball* CMonster_Goblin_Fireball::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Goblin_Fireball* pMonster = new CMonster_Goblin_Fireball(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Goblin Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Goblin_Fireball::Free()
{
	auto pool = CObjectPoolingManager::GetInstance();
	//pool->Return(m_pBloodParticleSystem);
	//pool->Return(m_pIceParticleSystem);

	Engine::CMonster::Free();
}

void CMonster_Goblin_Fireball::Play_BloodParticleSystem()
{
	m_pBloodParticleSystem = dynamic_cast<CParticleSystem_Blood*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BLOOD));
	m_pBloodParticleSystem->Set_Target(this);
	m_pBloodParticleSystem->Activate_ParticleSystem();
	m_pBloodParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CMonster_Goblin_Fireball::Play_IceParticleSystem()
{
	m_pIceParticleSystem = dynamic_cast<CParticleSystem_Ice*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_ICE));
	m_pIceParticleSystem->Set_Target(this);
	m_pIceParticleSystem->Activate_ParticleSystem();
	m_pIceParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}


