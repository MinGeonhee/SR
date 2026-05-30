#include "pch.h"
#include "CGameObject.h"
#include "CMonster_Skeleton_Sword.h"
#include "CLayer.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "CObjectPoolingManager.h"
#include "CSoundManager.h"
#include "CObjectManager.h"

CMonster_Skeleton_Sword::CMonster_Skeleton_Sword(const CMonster_Skeleton_Sword& rhs)
	:CMonster(rhs)
{
}

CMonster_Skeleton_Sword::CMonster_Skeleton_Sword(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CMonster_Skeleton_Sword::~CMonster_Skeleton_Sword()
{
}

HRESULT CMonster_Skeleton_Sword::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_MONSTER_SKELETON_SWORD;
	Set_MonsterType(MONSTER_SKELETON_SWORD);
	Set_MonsterRank(MONSTERRANK::RANK_COMMON);
	

	m_fSpeed = 2.f;
	m_fPlayerRecog_Distance = 10.f;
	m_fAttack_Distance = 1.1f;

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 0.5f, 0.7f, 0.5f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f,0.5f,0.f });

	m_eMonsterStatus = MONSTER_STATUS_SPAWN;

	Set_Animation();

	float randSpeed = 1.6f + (float)rand() / (float)RAND_MAX * (2.4f - 1.6f);

	STATUS st = { 60.f,60.f, 10.f, 3.f, randSpeed };
	m_pStatusCom->Set_Status(st);

	m_fSpeed = randSpeed;

	// 버프 아이콘 생성
	m_pBuff = CBuff::Create(m_pGraphicDev);
	m_pBuff->Set_Target(this);

	return S_OK;
}

_int CMonster_Skeleton_Sword::Update_GameObject(const _float& fTimeDelta)
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

	// 버프
	if (m_bIsBuffed)
	{
		m_pBuff->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CMonster_Skeleton_Sword::LateUpdate_GameObject(const _float& fTimeDelta)
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
		if (m_pBoneParticleSystem == nullptr)
		{
			Play_BoneParticleSystem();
		}
	}

	// 혹시라도 외부에서 상태 바꿧을수도 있으니 애니메이션 적용
	Set_Animation();

	// 플레이어 따라다니게
	Chase_Player(fTimeDelta);

	// 공격 상태 감지하고 공격하게
	Attack();

	// 버프
	if (m_bIsBuffed)
	{
		m_pBuff->LateUpdate_GameObject(fTimeDelta);
	}
}

void CMonster_Skeleton_Sword::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Skeleton_Animation")->Set_Texture();
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

void CMonster_Skeleton_Sword::Chase_Player(const _float& fTimeDelta)
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
			return;
		}
		// 플레이어가 감지 거리 안에 들어오면
		else if (fLength <= m_fPlayerRecog_Distance)
		{
			// 공격 사거리 안에 들어오면 이동하지 않고 공격
			if (fLength < m_fAttack_Distance)
			{
				m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_ATTACK;
				return;
			}
			// 플레이어 방향으로 이동
			else
			{
				m_eMonsterStatus = MONSTER_STATUS::MONSTER_STATUS_MOVE;
				D3DXVec3Normalize(&vDir, &vDir); // 정규화
				m_pTransform->Move_Pos(&vDir, m_fSpeed, fTimeDelta);
				_vec3 vPos = *(m_pTransform->GetInfoPos());
				_vec3 vPlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
				m_pTransform->Set_Pos(vPos.x, vPlayerPos.y - 0.5f, vPos.z);
			}
		}
	}
}

void CMonster_Skeleton_Sword::Set_Animation()
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
		m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		m_pAnimationCom->PlayAnimation();
		break;
	}
	case(MONSTER_STATUS::MONSTER_STATUS_MOVE):
	{
		// 애니메이션의 행 열 지정.
		m_pAnimationCom->SetGrid(8, 8); // 4행 8열
		m_pAnimationCom->SetRow(1);      // 맨 위 행

		const float baseSpeed = m_pStatusCom->GetSpeed();
		if (m_bIsBuffed)
		{
			m_pAnimationCom->AniMation_Init(4, 0.2f);	// 몇 열이고 출력 속도 몇인지
			//m_fSpeed = 2.3f;
			m_fSpeed = baseSpeed * 1.15f;
		}
		else
		{
			m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
			//m_fSpeed = 2.f;
			m_fSpeed = baseSpeed;
		}
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
			m_pAnimationCom->AniMation_Init(6, 0.08f);	// 몇 열이고 출력 속도 몇인지
		}
		else
		{
			m_pAnimationCom->AniMation_Init(6, 0.13f);	// 몇 열이고 출력 속도 몇인지
		}
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
		m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지.
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

void CMonster_Skeleton_Sword::Attack()
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
			CSoundManager::GetInstance()->SoundPlay(L"weapon-cleric-mace-swing.wav", MONSTER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 1.5);
			m_pWeaponCom->Fire();
			m_bIsBulletFired = true;
		}
		if (m_pAnimationCom->Get_CurrFrame() == 4)
		{
			m_bIsBulletFired = false;
		}
	}
}

HRESULT CMonster_Skeleton_Sword::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pWeaponCom = CLONE_PROTOTYPE(PROTO_WEAPON, CWeaponController);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pWeaponCom->Set_MyOwner(this);
	m_pWeaponCom->Weapon_Inintialize();
	m_pWeaponCom->Change_Weapon(WEAPON_SKELETON_SWORD);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Weapon",pComponent });


	return S_OK;
}

CMonster_Skeleton_Sword* CMonster_Skeleton_Sword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster_Skeleton_Sword* pMonster = new CMonster_Skeleton_Sword(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMonster);
		MSG_BOX("Goblin Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster_Skeleton_Sword::Free()
{
	auto pool = CObjectPoolingManager::GetInstance();
	//pool->Return(m_pBoneParticleSystem);
	//pool->Return(m_pIceParticleSystem);

	Safe_Release(m_pBuff);

	Engine::CMonster::Free();
}

void CMonster_Skeleton_Sword::Play_BoneParticleSystem()
{
	m_pBoneParticleSystem = dynamic_cast<CParticleSystem_Bone*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_BONE));
	m_pBoneParticleSystem->Set_Target(this);
	m_pBoneParticleSystem->Activate_ParticleSystem();
	m_pBoneParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CMonster_Skeleton_Sword::Play_IceParticleSystem()
{
	m_pIceParticleSystem = dynamic_cast<CParticleSystem_Ice*>(CObjectPoolingManager::GetInstance()->
		Get(OBJ_PARTICLE_SYSTEM_ICE));
	m_pIceParticleSystem->Set_Target(this);
	m_pIceParticleSystem->Activate_ParticleSystem();
	m_pIceParticleSystem->Make_And_Apply_Values_To_Particle();

	m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}
