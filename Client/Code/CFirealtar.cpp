#include "pch.h"
#include "CFirealtar.h"
#include "CProtoTypeManager.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CResourceManager.h"


CFirealtar::CFirealtar(LPDIRECT3DDEVICE9 Device)
    : CLightObject(Device), m_pBillboardCom(nullptr)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
    m_eLight_Object_Type = LIGHT_OBJECT_TYPE::HANGINGBRAZIER;
    m_ePlacementType = MAPOBJECT_PLACEMENT::GROUND;
}

CFirealtar::CFirealtar(const CFirealtar& cpy)
    : CLightObject(cpy) , m_pBillboardCom(nullptr)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
    m_eLight_Object_Type = LIGHT_OBJECT_TYPE::STATUE;
    m_ePlacementType = MAPOBJECT_PLACEMENT::GROUND;
}

CFirealtar::~CFirealtar()
{
}

HRESULT CFirealtar::Ready_GameObject()
{
    if (FAILED(CFirealtar::Add_Component()))
        return E_FAIL;


    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"FIREALTAR");


    m_pAnimationCom->SetGrid(8,1); // 8열 4행
    m_pAnimationCom->SetRow(0);      // 맨 위 행
    m_pAnimationCom->SetCol(0, 7);      // 맨 위 행
    m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
    m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
    m_pAnimationCom->PlayAnimation();

    m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
    m_pAnimationCom->PlayAnimation();

    return S_OK;
}

_int CFirealtar::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);


    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);


    return 0;
}

void CFirealtar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_pBillboardCom->LateUpdate_Component(fTimeDelta);

}

void CFirealtar::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pTextureCom->Set_Texture();
    m_pAnimationCom->Render_Buffer();
}

CFirealtar* CFirealtar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CFirealtar* pObj = new CFirealtar(pGraphicDev);

    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("CFirealtar Create Failed");
        return nullptr;
    }


    return pObj;
}

HRESULT CFirealtar::Add_Component()
{
    //부모에는 트랜스폼 밖에없나 애니매이션 컴포넌트도 있다.
    if (FAILED(CLightObject::Add_Component()))
        return E_FAIL;

    CComponent* pComponent = NULL;

    //================ 콜라이더 SPHERE 충돌 컴포넌트 ===============
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

void CFirealtar::Free()
{
    CGameObject::Free();
}
