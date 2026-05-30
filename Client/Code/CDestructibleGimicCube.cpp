#include "pch.h"
#include "CDestructibleGimicCube.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CProtoTypeManager.h"
#include "CObjectManager.h"

CDestructibleGimicCube::CDestructibleGimicCube(LPDIRECT3DDEVICE9 Device)
    : CEventCube(Device)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_EVENTCUBE;
    m_eCubeEventType = CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC;
}

CDestructibleGimicCube::CDestructibleGimicCube(const CDestructibleGimicCube& cpy)
    : CEventCube(cpy)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_EVENTCUBE;
    m_eCubeEventType = CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC;
}

CDestructibleGimicCube::~CDestructibleGimicCube()
{
}


_int       CDestructibleGimicCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);


    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX, this);

    return 0;
}

void		CDestructibleGimicCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void        CDestructibleGimicCube::Render_GameObject()
{
    //렌더 
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

void CDestructibleGimicCube::PlayEvent()
{
    m_bActivate = false;

    NULLPTR_RETURN(m_pColliderCom);

    m_pColliderCom->Set_ColliderType(COLLIDER_NONE);
}


// CDoor CUbe가 필요한 컴포넌트 Transofrm , 콜라이더  , Tex 
HRESULT		CDestructibleGimicCube::Add_Component()
{

    if (FAILED(CEventCube::Add_Component()))
        return E_FAIL;

    CComponent* newCom = nullptr;

    newCom = m_pBuffer = CLONE_PROTOTYPE(PROTO_CUBETEX, CCubeTex);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer" , newCom });

    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Collider" , newCom });
    m_pColliderCom->Set_MyOwner(this);

    return S_OK;
}

HRESULT	    CDestructibleGimicCube::Ready_GameObject()
{
    if (FAILED(CDestructibleGimicCube::Add_Component()))
        return E_FAIL;

    return S_OK;
}


CDestructibleGimicCube* CDestructibleGimicCube::Create(LPDIRECT3DDEVICE9 Device)
{
    CDestructibleGimicCube* pDestructibleGimicCube = new CDestructibleGimicCube(Device);

    if (FAILED(pDestructibleGimicCube->Ready_GameObject()))
    {
        Safe_Release(pDestructibleGimicCube);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pDestructibleGimicCube;
}



void	CDestructibleGimicCube::Free()
{
    CGameObject::Free();
}
