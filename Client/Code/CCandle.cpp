#include "pch.h"
#include "CCandle.h"
#include "CProtoTypeManager.h"
#include "CTransform.h"
#include "CResourceManager.h"
#include "CRendererManager.h"


CCandle::CCandle(LPDIRECT3DDEVICE9 Device)
	: CLightObject(Device)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::CANDLE;
	m_ePlacementType = MAPOBJECT_PLACEMENT::GROUND;
}

CCandle::CCandle(const CCandle& cpy)
	: CLightObject(cpy)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::CANDLE;
	m_ePlacementType = MAPOBJECT_PLACEMENT::GROUND;
}

CCandle::~CCandle()
{
}

HRESULT CCandle::Ready_GameObject()
{
	if (FAILED(CCandle::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"CANDLE");


	m_pAnimationCom->SetGrid(4 , 2); // 4열 2행
	m_pAnimationCom->SetRow(0);      // 맨 위 행
	m_pAnimationCom->SetCol(0,3);
	m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
	m_pAnimationCom->PlayAnimation();

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pAnimationCom->PlayAnimation();

	return S_OK;
}

_int CCandle::Update_GameObject(const _float& fTimeDelta)
{
	//if (m_isEvent == true)
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);


	return 0;
}

void CCandle::LateUpdate_GameObject(const _float& fTimeDelta)
{
	m_pBillboardCom->LateUpdate_Component(fTimeDelta);
}

void CCandle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pTextureCom->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

CCandle* CCandle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCandle* pObj = new CCandle(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("CCandle Create Failed");
		return nullptr;
	}


	return pObj;
}

HRESULT CCandle::Add_Component()
{
	//부모에는 트랜스폼 밖에없나 애니매이션 컴포넌트도 있다.
	if (FAILED(CLightObject::Add_Component()))
		return E_FAIL;

	CComponent* pComponent = NULL;

	//================ 콜라이더 AAABBB 충돌 컴포넌트 ===============
	pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });

	// =============== 빌보드 컴포넌트 ==================

	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });

	return S_OK;

}

void CCandle::Free()
{
	CGameObject::Free();
}
