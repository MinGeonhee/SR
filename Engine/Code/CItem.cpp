#include "CItem.h"
#include "CBillboard.h"
#include "CObjectManager.h"
#include "CResourceManager.h"
#include "CRendererManager.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CItem::CItem(const CItem& rhs)
	:CGameObject(rhs)
{

}

CItem::~CItem()
{

}

HRESULT	CItem::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_ITEM;


	mt19937 rng{ random_device{}() };                  // 엔진 + 시드
	uniform_int_distribution<int> d6(1, static_cast<uint8_t>(ITEMTYPE::ITEM_COUNT_END-1));            // 1~6 균등정수
	int x = d6(rng);                                   // 하나 뽑기

	m_eItemtype = static_cast<ITEMTYPE>(x);

	return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);
	
	if (!m_bIsDropItem)
	{
		ADD_HoverComponent();
	}
	else
	{
		ADD_DropComponent();
	}
	
	Item_StatusSetting();
	ITem_Animation_Setting();


	_vec3 vPos = *this->Get_Transform()->GetInfoPos();
	CGameObject::Compute_ViewZ(&vPos);

	return 0;
}

void CItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

	if (m_pTextureCom == nullptr) Item_Texture_Setting();
	else m_pTextureCom->Set_Texture();

	m_pAnimationCom->Render_Buffer();
}

HRESULT CItem::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ 트랜스폼 컴포넌트 ===============
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	// =============== 빌보드 컴포넌트 ==================

	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });

	//================ 애니메이션 컴포넌트 ===============

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });


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
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Status", pComponent });


	return S_OK;
}


void CItem::Free()
{
	Engine::CGameObject::Free();
}




void CItem::Item_StatusSetting()
{
	if (m_eItemtype == ITEMTYPE::ITEM_NONE || m_bStatusSetting) return;

	switch (m_eItemtype)
	{
		// ================ 플레이어 Bullet ===============

		case ITEMTYPE::ITEM_BERRIES:
		{
			m_pStatusCom->Set_Status(STATUS{ 2.f, 2.f, 0.f, 0.f, 0.f }); // 체력 회복 2
			break;
		}

		case ITEMTYPE::ITEM_HEALTH_SHARD:
		{
			m_pStatusCom->Set_Status(STATUS{ 2.f, 2.f, 0.f, 0.f, 0.f }); // 체력 회복 2
			break;
		}

		case ITEMTYPE::ITEM_ARMOR_SHARD:
		{
			m_pStatusCom->Set_Status(STATUS{ 5.f, 5.f, 0.f, 0.f, 0.f }); // 방어구 +5
			break;
		}

		case ITEMTYPE::ITEM_GREATER_POTION:
		{
			m_pStatusCom->Set_Status(STATUS{ 25.f, 25.f, 0.f, 0.f, 0.f }); // 체력 회복 25
			break;
		}
	}

	m_bStatusSetting = true;
}

void CItem::Item_Texture_Setting()
{
	if (m_eItemtype == ITEMTYPE::ITEM_NONE) return;

	switch (m_eItemtype)
	{
	case ITEM_NONE:
		return;
		break;

		// 회복 & 방어구 아이템

	case ITEM_BERRIES:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_HEALTH_SHARD:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_ARMOR_SHARD:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Armor_Shard");
		break;

	case ITEM_GREATER_POTION:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

		// 돈 종류

	case ITEM_BROZE_COINS:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_SILVER_COINS:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_GOLD_COINS:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_COIN_PURSE:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_GOLD_CHALICE:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;


		// ============= 무기류 ITem ==============

	case ITEM_FIREBALL_RING:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_ICEWAND:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Ice_Wand");
		break;

	case ITEM_GUN:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Gun");
		break;

		// ============= 총알류 ITem ==============

	case ITEM_LOW_MANA:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item1");
		break;

	case ITEM_FIRE_SHARD:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item1");
		break;

	case ITEM_ICE_SHARD:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_GUN_BULLET:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"GunBullet");
		break;

		// =========== 열쇠류 Item =============== 
	case ITEM_BRONZE_KEY:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_SILVER_KEY:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_GOLD_KEY:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;

	case ITEM_TREASURE_KEY:
		m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Item2");
		break;
	}
}

void CItem::ITem_Animation_Setting()
{
	// 상태 안바뀌었으면 걍 리턴
	if (m_eItemtype == ITEMTYPE::ITEM_NONE || m_bAnimationSetting) return;

		// 애니메이션의 행 열 지정.
		switch (m_eItemtype)
		{
			// ========== 회복 아이템 종류 ==============
			case(ITEMTYPE::ITEM_BERRIES):		// 플레이어 메이스 차징 공격
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(6, 6);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_HEALTH_SHARD):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(4, 4);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_ARMOR_SHARD):
			{
				m_pAnimationCom->SetGrid(5, 1); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(5, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GREATER_POTION):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(1, 1);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			// ============== 돈 종류 아이템 =================

			case(ITEMTYPE::ITEM_BROZE_COINS):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(1);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(0, 0);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_SILVER_COINS):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(1);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(1, 1);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GOLD_COINS):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(1);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(2, 2);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_COIN_PURSE):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(1);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(6, 6);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GOLD_CHALICE):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(1);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(3, 3);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			// ============== 무기 종류 아이템 =================

			case(ITEMTYPE::ITEM_FIREBALL_RING):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(6);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(1, 1);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_ICEWAND):
			{
				m_pAnimationCom->SetGrid(1, 1); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(1, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GUN):
			{
				m_pAnimationCom->SetGrid(1, 1); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(1, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			// ============== 총알 종류 아이템 ====================

			case(ITEMTYPE::ITEM_LOW_MANA):
			{
				m_pAnimationCom->SetGrid(8, 8); // 4행 8열
				m_pAnimationCom->SetRow(2);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(4, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(4, 7);
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_FIRE_SHARD):
			{
				m_pAnimationCom->SetGrid(8, 8); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(7, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_ICE_SHARD):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(5, 5);
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GUN_BULLET):
			{
				m_pAnimationCom->SetGrid(6, 1); // 1행 6열
				m_pAnimationCom->SetRow(0);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(6, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}


			// ============== 열쇠 종류 아이템 ====================

			case(ITEMTYPE::ITEM_BRONZE_KEY):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(2);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(0, 0);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_SILVER_KEY):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(2);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(1, 1);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_GOLD_KEY):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(2);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(2, 2);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

			case(ITEMTYPE::ITEM_TREASURE_KEY):
			{
				m_pAnimationCom->SetGrid(8, 16); // 4행 8열
				m_pAnimationCom->SetRow(2);      // 맨 위 행
				m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetCol(3, 3);	// 몇 열이고 출력 속도 몇인지
				m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
				m_pAnimationCom->PlayAnimation();
				m_pTransform->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
				break;
			}

		}

		m_bAnimationSetting = true;
}

void CItem::ADD_HoverComponent()
{
	if (m_eItemtype == ITEMTYPE::ITEM_NONE || m_bHoverComponentSetting) return;

	if (IsItemTypeExempt())
	{
		CComponent* pComponent = NULL;
		//================ 아이템 호버 컴포넌트  ===============
		pComponent = m_pHoverCom = CLONE_PROTOTYPE(PROTO_HOVERITEM, CHoverItem);

		if (pComponent == NULL)
		{
			MSG_BOX("Hover Component 생성 실패!");
			return;
		}
		m_pHoverCom->Set_MyOwner(this);
		m_pHoverCom->HoverItem_Initialize();
		m_pHoverCom->Set_HoverOffset(0.5f);	// 둥실둥실하기까지 딜레이 시간.
		m_pHoverCom->Set_HoverSpeed(0.25f); // 2배속
		m_mapComponent[ID_DYNAMIC].insert({ L"Com_Hover", pComponent });
	}

	m_bHoverComponentSetting = true;
}

void CItem::ADD_DropComponent()
{
	if (m_eItemtype == ITEMTYPE::ITEM_NONE || m_bDropComponentSetting) return;
	
	CComponent* pComponent = NULL;
	//================ 드랍 컴포넌트  ===============
	if (ExcludeDropTable())
	{
		pComponent = m_pDropCom = CLONE_PROTOTYPE(PROTO_DROPITEM, CDropItem);

		if (pComponent == NULL)
		{
			MSG_BOX("Drop Component 생성 실패!");
			return;
		}
		m_pDropCom->Set_MyOwner(this);
		m_pDropCom->Set_TargetTransform(m_pTargetTransform);
		m_mapComponent[ID_DYNAMIC].insert({ L"Com_Drop", pComponent });

		m_bDropComponentSetting = true;
	}
}


// 예외 처리하고 싶은 것.
bool CItem::IsItemTypeExempt()
{
	switch (m_eItemtype)
	{
	case ITEMTYPE::ITEM_BROZE_COINS:
	case ITEMTYPE::ITEM_SILVER_COINS:
	case ITEMTYPE::ITEM_GOLD_COINS:
	case ITEMTYPE::ITEM_COIN_PURSE:
	case ITEMTYPE::ITEM_GOLD_CHALICE:
	case ITEMTYPE::ITEM_GREATER_POTION:
		return false;

	default:
		return true;
	}
}

bool CItem::ExcludeDropTable()
{
	switch (m_eItemtype)
	{
	case ITEMTYPE::ITEM_BRONZE_KEY:
	case ITEMTYPE::ITEM_SILVER_KEY:
	case ITEMTYPE::ITEM_GOLD_KEY:
	case ITEMTYPE::ITEM_TREASURE_KEY:
	case ITEMTYPE::ITEM_ICEWAND:
	case ITEMTYPE::ITEM_FIREBALL_RING:
	case ITEMTYPE::ITEM_GUN:
		return false;

	default:
		return true;
	}
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItem* pItem = new CItem(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pItem);
		MSG_BOX("ITem Create Failed");
		return nullptr;
	}

	return pItem;
}


