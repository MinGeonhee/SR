#include "CObstacle.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CTexture.h"


CObstacle::CObstacle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapObject(pGraphicDev) , m_pBillboardCom(nullptr)

{

    m_tHandle.m_eType = OBJ_MAPOBSTACLE;
    m_eCategory = MAPOBJECT_CATEGORY::OBSTACLE;
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CObstacle::CObstacle(const CObstacle& rhs)
	: CMapObject(rhs) , m_pBillboardCom(nullptr)
{
    m_tHandle.m_eType = OBJ_MAPOBSTACLE;
    m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CObstacle::~CObstacle()
{

}


_int CObstacle::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);

    _vec3 vPos = *this->Get_Transform()->GetInfoPos();
    CGameObject::Compute_ViewZ(&vPos);

    return 0;
}


void  CObstacle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
    else
        if( m_pBillboardCom != nullptr)
            m_pBillboardCom->LateUpdate_Component(fTimeDelta);
}


void    CObstacle::Render_GameObject()
{
    
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    CResourceManager::GetInstance()->Get<CTexture>(L"Obstacle")->Set_Texture();
    m_pBuffer->Render_Buffer();
}

HRESULT CObstacle::Ready_GameObject()
{
    if (FAILED(CObstacle::Add_Compoenet()))
        return E_FAIL;


    return S_OK;
}


HRESULT CObstacle::Add_Compoenet()
{
    if (FAILED(CMapObject::Add_Component()))
        return E_FAIL;

    // ============ SPEAR ÄÄÆ÷³ÍÆ® ===============

    CComponent* newCom = nullptr;

    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    if (newCom == nullptr)
        return E_FAIL;

    m_pColliderCom->Set_MyOwner(this);
    m_pColliderCom->Set_ColliderScale(_vec3{ 0.6f, 0.6f, 0.6f });
    m_pColliderCom->Set_Offset(_vec3{ 0.f, -0.3f, 0.f });
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", newCom });

    // ============ ºôº¸µå ÄÄÆ÷³ÍÆ® ===============

    newCom = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);
    if (newCom == nullptr)
        return E_FAIL;


    m_pBillboardCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",newCom });
}



CObstacle* CObstacle::Create(LPDIRECT3DDEVICE9 Device)
{
    CObstacle* pObj = new CObstacle(Device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        MSG_BOX("CGroundDecor Create Is Failed");
        Safe_Release(pObj);
        return nullptr;
    }

    return pObj;
}


void CObstacle::Free()
{
    CMapObject::Free();
}