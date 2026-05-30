#include "pch.h"
#include "CChandelier.h"
#include "CProtoTypeManager.h"
#include "CTransform.h"
#include "CResourceManager.h"
#include "CRendererManager.h"


CChandelier::CChandelier(LPDIRECT3DDEVICE9 Device)
	: CLightObject(Device) ,  m_pBillboardCom(nullptr)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::CHANDELIER;
	m_ePlacementType = MAPOBJECT_PLACEMENT::GROUND;

	m_pColliderCom = nullptr;
}

CChandelier::CChandelier(const CChandelier& cpy)
	: CLightObject(cpy) , m_pBillboardCom(nullptr)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::CANDLETRIPLE;
}

CChandelier::~CChandelier()
{
}

HRESULT CChandelier::Ready_GameObject()
{
	if (FAILED(CChandelier::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"CHANDELIER");


	m_pAnimationCom->SetGrid(1,4); // 1열 3행
	m_pAnimationCom->SetRow(0);      // 
	m_pAnimationCom->SetCol(0,0);
	m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
	m_pAnimationCom->PlayAnimation();

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pAnimationCom->PlayAnimation();

	return S_OK;
}

_int CChandelier::Update_GameObject(const _float& fTimeDelta)
{

	CGameObject::Update_GameObject(fTimeDelta);


	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	return 0;
}

void CChandelier::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CChandelier::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pTextureCom->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

CChandelier* CChandelier::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChandelier* pObj = new CChandelier(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("CChandelier Create Failed");
		return nullptr;
	}


	return pObj;
}

HRESULT CChandelier::Add_Component()
{
	//부모에는 트랜스폼 밖에없나 애니매이션 컴포넌트도 있다.
	if (FAILED(CLightObject::Add_Component()))
		return E_FAIL;

	CComponent* pComponent = NULL;

	// =============== 빌보드 컴포넌트 ==================

	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });

	return S_OK;

}

void CChandelier::Free()
{
	CGameObject::Free();
}
