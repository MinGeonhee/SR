#include "pch.h"
#include <random> 
#include <chrono>
#include "CGameObject.h"
#include "CMonster_Rat.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
CMonster_Rat::CMonster_Rat(const CMonster_Rat& rhs)
	:CMonster(rhs)
{
}

CMonster_Rat::CMonster_Rat(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Rat::~CMonster_Rat()
{
}

HRESULT CMonster_Rat::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_MONSTER_RAT;
	Set_MonsterType(MONSTER_GOBLIN_FIREBALL);
	Set_MonsterRank(MONSTERRANK::RANK_COMMON);

	m_fPlayerRecog_Distance = 10.f;
	m_fAttack_Distance = 0.7f;

	m_fJumpDistance_Max = 8.f;
	m_fJumpDistance_Min = 3.f;

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,0.5f,0.f });

	Set_Animation();

	// 버프 아이콘 생성
	m_pBuff = CBuff::Create(m_pGraphicDev);
	m_pBuff->Set_Target(this);
	m_pBuff->Set_Offset_Y(0.7f);
	
	return S_OK;
}

_int CMonster_Rat::Update_GameObject(const _float& fTimeDelta)
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

	if (m_pAnimationCom->Get_IsAnimationEnd())
	{
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
	}

	Get_Vector_Between_Player_Monster();


	// 버프
	if (m_bIsBuffed)
	{
		m_pBuff->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CMonster_Rat::LateUpdate_GameObject(const _float& fTimeDelta)
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

	

	Chase_Player(fTimeDelta);
	
	Jump(fTimeDelta);

	Attack(fTimeDelta);
	
	// 전역적으로 애니메이션 체인지 되는거 체크
	Set_Animation();

	// 버프
	if (m_bIsBuffed)
	{
		m_pBuff->LateUpdate_GameObject(fTimeDelta);
	}
}

void CMonster_Rat::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

	CResourceManager::GetInstance()->Get<CTexture>(L"Rat_Animation")->Set_Texture();
	m_pAnimationCom->Render_Buffer();

	// 버프
	if (m_bIsBuffed)
	{
		if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DEAD
			|| m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ICE_DYING
			|| m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_DYING)
		{
			return;
		}
		else
		{
			m_pBuff->Render_GameObject();
		}
	}
}

void CMonster_Rat::Get_Vector_Between_Player_Monster()
{
	_vec3 vMonsterPos, vPlayerPos;
	m_pTransform->Get_Info(INFO_POS, &vMonsterPos);
	vMonsterPos = { vMonsterPos.x, 0.f, vMonsterPos.z };

	m_pPlayerTransCom = m_pTarget->Get_Transform();
	m_pPlayerTransCom->Get_Info(INFO_POS, &vPlayerPos);
	vPlayerPos = { vPlayerPos.x, 0.f, vPlayerPos.z };

	m_vFrom_Monster_To_Player = vPlayerPos - vMonsterPos;
}

void CMonster_Rat::Chase_Player(const _float& fTimeDelta)
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
			if (fLength < m_fAttack_Distance)
			{
				m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_ATTACK;
				Set_Animation();
				return;
			}
			else
			{
				// 점프 범위 내로 들어오면 점프 상태로 바꾸기
				if (fLength <= m_fJumpDistance_Max && fLength >= m_fJumpDistance_Min)
				{
					m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_JUMP;
					Set_Animation();
					return;
				}
				// 점프 범위보다 가까우면 그냥 걷게
				else if (fLength < m_fJumpDistance_Min)
				{
					m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE;
					Set_Animation();
					D3DXVec3Normalize(&m_vFrom_Monster_To_Player, &m_vFrom_Monster_To_Player); // 정규화
					m_pTransform->Move_Pos(&m_vFrom_Monster_To_Player, m_fSpeed, fTimeDelta);
					_vec3 vPos = *(m_pTransform->GetInfoPos());
					_vec3 vPlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
					m_pTransform->Set_Pos(vPos.x, vPlayerPos.y - 0.5f, vPos.z);
				}
				else
				{
					m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
					Set_Animation();
				}
			}
		}
	}
}



void CMonster_Rat::Attack(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_ATTACK)
	{
		if (m_pAnimationCom->Get_IsAnimationEnd())
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
			// 플레이어 hp 깎기
			//dynamic_cast<CPlayer*>(m_pTarget)->Minus_PlayerHp(m_iMonsterAttackDamage);
		}
		if (m_pAnimationCom->Get_CurrFrame() == 3
			&& m_bIsBulletFired == false)
		{
			CSoundManager::GetInstance()->SoundPlay(L"rat-attack.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
			m_pWeaponCom->Fire();
			m_bIsBulletFired = true;
		}
		if (m_pAnimationCom->Get_CurrFrame() == 4)
		{
			m_bIsBulletFired = false;
		}
	}
}

void CMonster_Rat::Jump(const _float& fTimeDelta)
{
	if (m_eMonsterStatus == MONSTER_STATUS::MONSTER_STATUS_JUMP)
	{
		// 플레이어 뚫고 가거나 너무 가까워지지않게 조건 걸기
		if (D3DXVec3Length(&m_vFrom_Monster_To_Player) > 0.7f)
		{
			D3DXVec3Normalize(&m_vFrom_Monster_To_Player, &m_vFrom_Monster_To_Player); // 정규화
			m_pTransform->Move_Pos(&m_vFrom_Monster_To_Player, m_fSpeed * 3.f, fTimeDelta);
		}
		// 너무 가까우면 아이들로 바꿔주기
		else
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
		}
		
		// 애니메이션 끝나면 아이들로 바꿔주기
		if (m_pAnimationCom->Get_IsAnimationEnd())
		{
			m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_IDLE;
		}
	}
}

void CMonster_Rat::Set_Animation()
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
		m_pAnimationCom->SetRow(6);      // 맨 위 행
		if (m_bIsBuffed)
		{
			m_pAnimationCom->AniMation_Init(4, 0.08f);	// 몇 열이고 출력 속도 몇인지
			m_fSpeed = 2.3f;
		}
		else
		{
			m_pAnimationCom->AniMation_Init(4, 0.12f);
			m_fSpeed = 2.f;
		}
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_JUMP):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(8, 0.15f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_MOVE_IDLE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(1, 0.25f);	// 몇 열이고 출력 속도 몇인지
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_ATTACK):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(2);      // 맨 위 행
		if (m_bIsBuffed)
		{
			m_pAnimationCom->AniMation_Init(5, 0.12f);	// 몇 열이고 출력 속도 몇인지
		}
		else
		{
			m_pAnimationCom->AniMation_Init(5, 0.17f);
		}
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_HIT):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(3);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(2, 0.15f);	// 몇 열이고 출력 속도 몇인지
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
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_DYING):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(5);      // 맨 위 행
		m_pAnimationCom->AniMation_Init(6, 0.18f);	// 몇 열이고 출력 속도 몇인지.
		m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::OnceHold);
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		// 다잉일때는 바로 데드로 바꿔주기
		m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_DEAD;
		break;
	}
	default:
		break;
	}

	m_ePrevMonsterStatus = m_eMonsterStatus;
}

HRESULT CMonster_Rat::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWeaponCom->Set_MyOwner(this);
	m_pWeaponCom->Weapon_Inintialize();
	m_pWeaponCom->Change_Weapon(WEAPON_RAT);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon",pComponent });


	return S_OK;
}

CMonster_Rat* CMonster_Rat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Rat* pMonster = new CMonster_Rat(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Goblin Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Rat::Free()
{
	auto pool = CObjectPoolingManager::GetInstance();
	//pool->Return(m_pBloodParticleSystem);
	//pool->Return(m_pIceParticleSystem);

	Safe_Release(m_pBuff);

	Engine::CMonster::Free();
}

void CMonster_Rat::Play_BloodParticleSystem()
{
	m_pBloodParticleSystem = dynamic_cast<CParticleSystem_Blood*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BLOOD));
	m_pBloodParticleSystem->Set_Target(this);
	m_pBloodParticleSystem->Activate_ParticleSystem();
	m_pBloodParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CMonster_Rat::Play_IceParticleSystem()
{
	m_pIceParticleSystem = dynamic_cast<CParticleSystem_Ice*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_ICE));
	m_pIceParticleSystem->Set_Target(this);
	m_pIceParticleSystem->Activate_ParticleSystem();
	m_pIceParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}


