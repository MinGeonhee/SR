#include "CBullet.h"
#include "CRendererManager.h"
#include "CInputManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CDebugLineHelper.h"
#include "CTransform.h"
#include "CAnimation.h"
#include "CObjectPoolingManager.h"
#include "CParticle.h"
#include "CSceneManager.h"


CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CBullet::CBullet(const CBullet& rhs)
	:CGameObject(rhs)
{

}

CBullet::~CBullet()
{

}

HRESULT	CBullet::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_BULLET;

	return S_OK;
}

_int CBullet::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eBulletStatus != BULLET_STATUS_DEAD && IsBulletTypeExempt())
	{
		if (m_eBulletState == BULLET_BOSS_ICEPILLAR ||
			m_eBulletState == BULLET_BOSS_THUNDER_DROP ||
			m_eBulletState == BULLET_ORGE_AXE ||
			m_eBulletState == BULLET_GOBLIN_HAMMER ||
			m_eBulletState == BULLET_RAT ||
			m_eBulletState == BULLET_SKELETON_SWORD)
		{
		}
		else if (m_eBulletState == BULLET_BOSS_WIND)
		{
			Move(fTimeDelta);
		}
		else
		{
			Move(fTimeDelta);
			if (int(m_fElapsedTime * 100000) % 30 == 0)
			{
				Make_Smoke_Trail_Particle();
			}
		}
		
	}

	Set_Animation();

	CGameObject::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);


	return 0;
}

void CBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	CGameObject::Compute_ViewZ(&vPos);

	if (m_eBulletStatus == BULLET_STATUS_DEAD && m_pAnimationCom->Get_IsAnimationEnd())	// 죽음 상태이고 애니메이션 출력 끝났으면 GG
	{
		// 초기 상태로 초기화 하고 Return.
		m_eBulletState = BULLETSTATE::BULLET_NONE;
		m_ePrevBulletState = BULLETSTATE::BULLET_NONE;
		
		m_pTransform->Set_Scale({1.f, 1.f, 1.f});
		m_pStatusCom->Set_Status({ 1.f,1.f,1.f,1.f,1.f });
		m_fElapsedTime = 0.f;
		m_pTextureCom = nullptr;
		CObjectPoolingManager::GetInstance()->Return(this);
	}

	Tick_Lifetime(fTimeDelta);
}

void CBullet::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

	if (m_pTextureCom == nullptr)
		Bullet_Texture_Setting();

	if (m_pTextureCom)
		m_pTextureCom->Set_Texture();

	if (IsBulletTypeExempt())           // 근접(메이스/해머/도끼) 제외하고 그리기
		m_pAnimationCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void CBullet::Tick_Lifetime(const float& fTimeDelta)
{
	m_fElapsedTime += fTimeDelta;

	if (m_fElapsedTime >= m_fLifeTime)
	{
		if (m_eBulletState == BULLET_BOSS_ICEPILLAR ||
			m_eBulletState == BULLET_BOSS_WIND)
		{
			m_eBulletStatus = BULLET_STATUS_DEAD;
		}
		else
		{
			m_eBulletState = BULLETSTATE::BULLET_NONE;
			m_ePrevBulletState = BULLETSTATE::BULLET_NONE;

			m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });
			m_pStatusCom->Set_Status({ 1.f,1.f,1.f,1.f,1.f });
			m_fElapsedTime = 0.f;
			m_pTextureCom = nullptr;
			CObjectPoolingManager::GetInstance()->Return(this);
		}
	}
}

HRESULT CBullet::Add_Component()
{

	CComponent* pComponent = NULL;

	//================ 버퍼 컴포넌트 ===============

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pAnimationCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

	//================ 트랜스폼 컴포넌트 ===============

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_pTransform->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//================ 콜라이더 충돌 컴포넌트 ===============

	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });

	// =============== 스테이터스 컴포넌트 ===============

	pComponent = m_pStatusCom = CLONE_PROTOTYPE(PROTO_STATUS, CStatus);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pStatusCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Status",pComponent });

	// =============== 빌보드 컴포넌트 ==================
	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });

	return S_OK;
}

void CBullet::Move(const _float& fTimeDelta)
{
	if (m_eBulletState == BULLET_NONE) return;

	m_pTransform->Move_Pos(&m_vBulletDir, m_pStatusCom->GetSpeed(), fTimeDelta);
}

CBullet* CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet* pBullet = new CBullet(pGraphicDev);

	if (FAILED(pBullet->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pBullet);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pBullet;
}

void CBullet::Change_BulletState(BULLETSTATE state)
{
	if (m_eBulletState == state) return;

	if (state == BULLETSTATE::BULLET_NONE) return;

	switch (state)
	{
		// ================ 플레이어 Bullet ===============

		case BULLETSTATE::BULLET_PLAYER_CHARGEEMACE:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 30.f, 0.f, 0.f }); // 공격력 60, 스피드 30
			m_eBulletState = BULLET_PLAYER_CHARGEEMACE;
			m_fLifeTime = 0.000001f;
			break;
		}


		case BULLETSTATE::BULLET_PLAYER_GUN:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 60.f, 0.f, 30.f }); // 공격력 60, 스피드 30
			m_eBulletState = BULLET_PLAYER_GUN;
			m_pColliderCom->Set_ColliderScale({ 0.3f, 0.3f, 0.3f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			break;
		}

		case BULLETSTATE::BULLET_PLAYER_GUNSKILL:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 60.f, 0.f, 5.f }); // 공격력 60, 스피드 1
			m_eBulletState = BULLET_PLAYER_GUNSKILL;
			m_pColliderCom->Set_ColliderScale({ 0.3f, 0.3f, 0.3f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			break;
		}

		case BULLETSTATE::BULLET_PLAYER_ICE:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 20.f }); // 공격력 60, 스피드 10
			m_eBulletState = BULLET_PLAYER_ICE;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.25f, 0.f });
			break;
		}

		case BULLETSTATE::BULLET_PLAYER_FIREBALL:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 15.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_PLAYER_FIREBALL;
			m_pColliderCom->Set_ColliderScale({ 0.3f, 0.3f, 0.3f });
			m_pColliderCom->Set_Offset({ 0.f, 0.35f, 0.f });
			break;
		}

		case BULLETSTATE::BULLET_PLAYER_EMBER:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 20.f }); // 공격력 10, 스피드 2
			m_eBulletState = BULLET_PLAYER_EMBER;
			m_pColliderCom->Set_ColliderScale({ 0.3f, 0.3f, 0.3f });
			m_pColliderCom->Set_Offset({ 0.f, 0.35f, 0.f });
			break;
		}

		// ================ 몬스터 Bullet ===============
		case BULLETSTATE::BULLET_GOBLIN_FIREBALL:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_GOBLIN_FIREBALL;
			m_pColliderCom->Set_ColliderScale({0.05f, 0.05f, 0.05f});
			m_pColliderCom->Set_Offset({ 0.f, 0.25f, 0.f });
			
			break;
		}

		case BULLETSTATE::BULLET_ORGE_AXE:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_ORGE_AXE;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

			break;
		}

		case BULLETSTATE::BULLET_GOBLIN_HAMMER:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_GOBLIN_HAMMER;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

			break;
		}

		case BULLETSTATE::BULLET_RAT:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_GOBLIN_HAMMER;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

			break;
		}

		case BULLETSTATE::BULLET_SKELETON_SWORD:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_SKELETON_SWORD;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

			break;
		}

		case BULLETSTATE::BULLET_WIZARD:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_WIZARD;
			m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });

			break;
		}

		case BULLETSTATE::BULLET_SKELLETON_SILVER_ARROW:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 0.5f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_SKELLETON_SILVER_ARROW;
			break;
		}

		case BULLETSTATE::BULLET_SKELLETON_GOLD_ARROW:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 0.5f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_SKELLETON_GOLD_ARROW;
			break;
		}

		// ================ 트랩 Bullet ===============
		case BULLETSTATE::BULLET_TRAP_FIREBALL:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 0.5f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_TRAP_FIREBALL;
			break;
		}

		// ================ 보스 ================
		case BULLETSTATE::BULLET_BOSS_ICEBALL:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 30.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_BOSS_ICEBALL;
			m_pColliderCom->Set_ColliderScale({ 0.5f, 0.5f, 0.f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			m_fLifeTime = 3.f;
			break;
		}

		case BULLETSTATE::BULLET_BOSS_ICEPILLAR:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 30.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_BOSS_ICEPILLAR;
			m_pColliderCom->Set_ColliderScale({ 0.8f, 0.8f, 0.8f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			m_fLifeTime = 3.f;
			Spawn_Random_Pos();
			break;
		}

		case BULLETSTATE::BULLET_BOSS_THUNDER_DROP:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_BOSS_THUNDER_DROP;
			m_pColliderCom->Set_ColliderScale({ 0.3f, 0.3f, 0.3f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			m_fLifeTime = 0.8f;
			Spawn_Random_Pos();
			break;
		}

		case BULLETSTATE::BULLET_BOSS_WIND:
		{
			m_pStatusCom->Set_Status(STATUS{ 1.f, 1.f, 10.f, 0.f, 5.f }); // 공격력 60, 스피드 3
			m_eBulletState = BULLET_BOSS_WIND;
			m_pColliderCom->Set_ColliderScale({ 0.8f, 0.8f, 0.8f });
			m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
			m_fLifeTime = 3.f;
			Spawn_Random_Pos();
			break;
		}
	}
}

void CBullet::Bullet_Texture_Setting()
{
	switch (m_eBulletState)
	{
		case BULLET_NONE:
			return;
			break;

		case BULLET_PLAYER_CHARGEEMACE:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Player_Mace");
			break;

		case BULLET_PLAYER_GUN:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles");
			break;
			
		case BULLET_PLAYER_GUNSKILL:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles");
			break;

		case BULLET_PLAYER_ICE:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

		case BULLET_PLAYER_FIREBALL:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles");
			break;

		

		case BULLET_PLAYER_EMBER:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

			// ============= 몬스터 Bullet ==============

		case BULLET_GOBLIN_FIREBALL:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Mon_Projectiles");
			break;

		case BULLET_WIZARD:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Mon_Projectiles");
			break;

		case BULLET_SKELLETON_SILVER_ARROW:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

		case BULLET_SKELLETON_GOLD_ARROW:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

			// ============= 함정 Bullet ==============

		case BULLET_TRAP_FIREBALL:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

			// ============== 보스 ===============
		case BULLET_BOSS_ICEBALL:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"projectiles2");
			break;

		case BULLET_BOSS_ICEPILLAR:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Boss_IcePillar_Wind");
			break;

		case BULLET_BOSS_THUNDER_DROP:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Boss_Thunder");
			break;

		case BULLET_BOSS_WIND:
			m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Boss_IcePillar_Wind");
			break;
	}

}

void CBullet::Set_Animation()
{
	// 상태 안바뀌었으면 걍 리턴
	if (m_eBulletStatus == m_ePrevBulletStatus
		&& m_eBulletState == m_ePrevBulletState)
	{
		return;
	}

	m_pAnimationCom->Rewind();	// 프레임 초기화
	
	switch (m_eBulletStatus)
	{
		case(BULLET_STATUS::BULLET_STATUS_IDLE):
		{
			// 애니메이션의 행 열 지정.
			switch (m_eBulletState)
			{

			case(BULLETSTATE::BULLET_PLAYER_CHARGEEMACE):		// 플레이어 메이스 차징 공격
			{
				m_pAnimationCom->SetGrid(0, 0); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
				m_pAnimationCom->PlayAnimation();
				break;
			}

				case(BULLETSTATE::BULLET_PLAYER_GUN):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(6);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.25f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_GUNSKILL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.25f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_ICE):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(1);      // 맨 위 행
					m_pAnimationCom->SetCol(0,7);
					m_pAnimationCom->AniMation_Init(8, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.25f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol( 4 ,7 );	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->AniMation_Init(2, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.45f, 0.45f, 0.45f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.35f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_EMBER):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.35f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_GOBLIN_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0,1);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.15f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_WIZARD):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(6);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(4, 0.15f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
					break;
				}
		

				case(BULLETSTATE::BULLET_SKELLETON_SILVER_ARROW):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case(BULLETSTATE::BULLET_SKELLETON_GOLD_ARROW):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case(BULLETSTATE::BULLET_TRAP_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_ICEBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(1);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(8, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.5f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_ICEPILLAR):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(3);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.5f, 1.5f, 1.5f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_THUNDER_DROP):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(1);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 8.f, 8.f, 8.f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_WIND):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(5);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(5, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.5f, 1.5f, 1.5f });
					break;
				}
			}
			break;
		}
		
		case(BULLET_STATUS::BULLET_STATUS_DEAD):
		{
			// 애니메이션의 행 열 지정.
			switch (m_eBulletState)
			{

				case(BULLETSTATE::BULLET_PLAYER_CHARGEEMACE):		// 플레이어 메이스 차징 공격
				{
					m_pAnimationCom->SetGrid(0, 0);		// 4행 8열
					m_pAnimationCom->SetRow(0);			// 맨 위 행
					m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					break;
				}


				case(BULLETSTATE::BULLET_PLAYER_GUN):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(4);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 4);
					m_pAnimationCom->AniMation_Init(2, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_GUNSKILL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(4);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 4);
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_ICE):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(2);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 3);	
					m_pAnimationCom->AniMation_Init(4, 0.15f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.05f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					break;
				}

				case(BULLETSTATE::BULLET_PLAYER_EMBER):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(4, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					m_pColliderCom->Set_ColliderScale({ 0.05f, 0.05f, 0.05f });
					m_pColliderCom->Set_Offset({ 0.f, 0.35f, 0.f });
					break;
				}

				case(BULLETSTATE::BULLET_GOBLIN_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					break;
				}

				case(BULLETSTATE::BULLET_WIZARD):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(6);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(1, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.25f, 0.25f, 0.25f });
					break;
				}


				case(BULLETSTATE::BULLET_SKELLETON_SILVER_ARROW):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case(BULLETSTATE::BULLET_SKELLETON_GOLD_ARROW):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case(BULLETSTATE::BULLET_TRAP_FIREBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}


				case(BULLETSTATE::BULLET_BOSS_ICEBALL):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(2);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(4, 0.15f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_ICEPILLAR):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(4);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.5f, 1.5f, 1.5f });
					break;
				}

				case(BULLETSTATE::BULLET_BOSS_WIND):
				{
					m_pAnimationCom->SetGrid(8, 8); // 4행 8열
					m_pAnimationCom->SetRow(6);      // 맨 위 행
					m_pAnimationCom->AniMation_Init(6, 0.1f);	// 몇 열이고 출력 속도 몇인지
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.5f, 1.5f, 1.5f });
					break;
				}
			}

			break;
		}
	}

	m_ePrevBulletState = m_eBulletState;
	m_ePrevBulletStatus = m_eBulletStatus;
}


bool CBullet::IsBulletTypeExempt()
{
	switch (m_eBulletState)
	{
		case BULLETSTATE::BULLET_PLAYER_CHARGEEMACE:
			return false;
			
		case BULLETSTATE::BULLET_GOBLIN_HAMMER:
			return false;

		case BULLETSTATE::BULLET_ORGE_AXE:
			return false;

		case BULLETSTATE::BULLET_RAT:
			return false;

		case BULLETSTATE::BULLET_SKELETON_SWORD:
			return false;


		default:
			return true;
	}
}

void CBullet::Make_Smoke_Trail_Particle()
{
	CParticle* pParticle = static_cast<CParticle*>(CObjectPoolingManager::GetInstance()->Get(OBJ_PARTICLE));

	if (pParticle == NULL) return;

	_vec3 vPos = *(this->Get_Transform()->GetInfoPos());
	_vec3 vOffset = _vec3{ 0.f,0.2f,0.f };
	pParticle->Make_Random_Position_XYZ({ 0.05f,0.05f,0.05f }, vPos + vOffset);
	pParticle->Reset_All_Particle_Type_Values();
	pParticle->Set_ParticleType(PARTICLE_TYPE_SMOKE);
	pParticle->Set_Particle_Texture_Index(0, 6);
	pParticle->Set_IsCollisionOn(false);
	pParticle->Set_Collider_Type(COLLIDER_NONE);

	switch (m_eBulletState)	
	{
	case Engine::BULLET_NONE:
		break;

	case Engine::BULLET_PLAYER_GUN:
		pParticle->Make_Random_Size({ 0.05f, 0.12f });
		pParticle->Set_AlphaBufferColor(0.85f, 0.65f, 0.78f);
		break;

	case Engine::BULLET_PLAYER_ICE:
		pParticle->Make_Random_Size({ 0.03f, 0.05f });
		pParticle->Set_AlphaBufferColor(0.6f, 0.9f, 1.f);
		break;

	case Engine::BULLET_PLAYER_FIREBALL:
		pParticle->Make_Random_Size({ 0.05f, 0.08f });
		pParticle->Set_AlphaBufferColor(1.f, 0.9f, 0.6f);
		break;

	case Engine::BULLET_PLAYER_EMBER:
		pParticle->Make_Random_Size({ 0.03f, 0.05f });
		pParticle->Set_AlphaBufferColor(1.f, 0.9f, 0.6f);
		break;

	case Engine::BULLET_GOBLIN_FIREBALL:
		pParticle->Make_Random_Size({ 0.05f, 0.08f });
		pParticle->Set_AlphaBufferColor(1.f, 0.9f, 0.6f);
		break;

	case Engine::BULLET_WIZARD:
		pParticle->Make_Random_Size({ 0.05f, 0.08f });
		pParticle->Set_AlphaBufferColor(0.85f, 0.65f, 0.78f);
		break;

	case Engine::BULLET_BOSS_ICEBALL:
		pParticle->Make_Random_Size({ 0.03f, 0.05f });
		pParticle->Set_AlphaBufferColor(0.6f, 0.9f, 1.f);
		break;

	default:
		break;
	}
	pParticle->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);

}


void CBullet::Free()
{
	Engine::CGameObject::Free();
}

void CBullet::Spawn_Random_Pos()
{
	// 시드 생성 (시간 기반으로 고유하게 생성)
	unsigned seed = static_cast<unsigned>(
		chrono::high_resolution_clock::now().time_since_epoch().count());
	mt19937 rng(seed);  // 난수 생성기

	float xpos = 0.f; float ypos = 0.f; float zpos = 0.f;

	// 랜덤 사이즈 범위 지정
	if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS)
	{
		xpos = 26.5f; ypos = 2.f; zpos = 42.5f;
	}

	else if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS2)
	{
		xpos = 48.5; ypos = 3.f; zpos = 49.5;
	}


	uniform_real_distribution<float> rd_X(xpos - m_vRandomPosRange_XZ.x / 2, xpos + m_vRandomPosRange_XZ.x / 2);
	uniform_real_distribution<float> rd_Z(zpos - m_vRandomPosRange_XZ.y / 2, zpos + m_vRandomPosRange_XZ.y / 2);

	// 랜덤 사이즈 생성
	float fXPos = rd_X(rng);
	float fZPos = rd_Z(rng);

	m_pTransform->Set_Pos(fXPos, ypos, fZPos);
}
