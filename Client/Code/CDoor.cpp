#include "pch.h"
#include "CDoor.h"
#include "CRendererManager.h"
#include "CInputManager.h"
#include "CResourceManager.h"

CDoor::CDoor(const CDoor& rhs)
	:CGameObject(rhs), m_pTarget(nullptr), m_pDoorCom(nullptr)
{
}

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_pTarget(nullptr), m_pDoorCom(nullptr)
{
}

CDoor::~CDoor()
{

}

HRESULT	CDoor::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_DOOR;
	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_ColliderScale(_vec3{ 1.f, 1.f, 1.f });
	m_pColliderCom->Set_Offset(_vec3{ 0.f, 0.5f, 0.f });

	return S_OK;
}

_int CDoor::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);


	_vec3 vPos = *m_pTransform->GetInfoPos();
	CGameObject::Compute_ViewZ(&vPos);

	return 0;
}

void CDoor::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	Set_Animation();
	Set_Texture();

	if (m_pAnimationCom->Get_IsAnimationEnd() && m_pDoorCom->Get_DoorState() == DOORSTATE::DOOR_OPEN)
		m_pColliderCom->Set_ColliderType(COLLIDER_NONE);

}

void CDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	if (m_pTexture != nullptr) m_pTexture->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

void CDoor::Set_SubType(void* SubType)
{

	DOORTYPE eDoorType = *static_cast<DOORTYPE*>(SubType);

	m_eDoorType = eDoorType;

	NULLPTR_RETURN(m_pDoorCom);

	m_pDoorCom->Set_DoorType(eDoorType);

}

HRESULT CDoor::Add_Component()
{
	CComponent* pComponent = NULL;
	// transform 컴포넌트.

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_pTransform->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// ============ 문 컴포넌트 생성.===========
	pComponent = m_pDoorCom = CLONE_PROTOTYPE(PROTO_DOOR, CDoor_Component);

	if (pComponent == NULL)
		return E_FAIL;

	m_pDoorCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Door", pComponent });

	//=========== 애니메이션 컴포넌트 생성 ===========

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pAnimationCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

	// ========= 콜라이더 ==================

	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });


	return S_OK;
}

void CDoor::Set_Texture()
{
 	if (m_pTexture != nullptr || m_eDoorType == DOORTYPE::NONE_DOOR) return;

	switch (m_eDoorType)
	{
		case DOORTYPE::BOSS_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"bossDoor"); break;
		case DOORTYPE::DEFAULT_WOOD_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"defaultWoodDoor"); break;
		case DOORTYPE::DEFAULT_IRON_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"defaultIronDoor"); break;
		case DOORTYPE::SWITCH_WOOD_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"SwitchWoodDoor"); break;
		case DOORTYPE::SWITCH_IRON_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"SwitchIronDoor"); break;
		case DOORTYPE::BRONZE_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"BronzeDoor"); break;
		case DOORTYPE::SILVER_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"SilverDoor"); break;
		case DOORTYPE::GOLD_DOOR: m_pTexture = CResourceManager::GetInstance()->Get<CTexture>(L"GoldDoor"); break;
	}
}

bool CDoor::Set_Animation()
{
	// 상태 안바뀌었으면 걍 리턴
	if (m_bDoorOpen) return false;
	m_eCurState = m_pDoorCom->Get_DoorState();

	if (m_eCurState == m_ePrevState) return false;
	// 애니메이션의 행 열 지정.

	switch (m_eCurState)
	{
		case DOORSTATE::DOOR_CLOSE:
		{
			switch (m_eDoorType)
			{

				case DOORTYPE::DEFAULT_WOOD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0,0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::DEFAULT_IRON_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::SWITCH_WOOD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::SWITCH_IRON_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::BRONZE_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::SILVER_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::GOLD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::BOSS_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 0);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}
			}

			break;
		}

		case DOORSTATE::DOOR_OPEN:
		{
			switch (m_eDoorType)
			{
				case DOORTYPE::DEFAULT_WOOD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::DEFAULT_IRON_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::SWITCH_WOOD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}

				case DOORTYPE::SWITCH_IRON_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

					break;
				}


				case DOORTYPE::BRONZE_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::SILVER_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::GOLD_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(0);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 7);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.15f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}

				case DOORTYPE::BOSS_DOOR:
				{
					m_pAnimationCom->SetGrid(10, 1); // 4행 8열
					m_pAnimationCom->SetRow(1);      // 맨 위 행
					m_pAnimationCom->SetCol(0, 9);      // 맨 위 열
					m_pAnimationCom->AniMation_Init(7, 0.25f);	// 몇 열이고 출력 속도 몇인지.
					m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
					m_pAnimationCom->PlayAnimation();
					m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
					break;
				}
			}
			break;
		}
	}

	m_ePrevState = m_eCurState;
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor* pDoor = new CDoor(pGraphicDev);

	if (FAILED(pDoor->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pDoor);
		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pDoor;
}

void CDoor::Free()
{
	Engine::CGameObject::Free();
}