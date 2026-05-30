#include "pch.h"
#include "CPortalCube.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CProtoTypeManager.h"
#include "CObjectManager.h"
#include "CUIManager.h"
#include "CPlayer.h"

CPortalCube::CPortalCube(LPDIRECT3DDEVICE9 Device)
	: CEventCube(Device)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_EVENTCUBE;
    m_eCubeEventType = CUBE_EVENT_TYPE::PORTAL;
    m_isEventTriggered = false;
}

CPortalCube::CPortalCube(const CPortalCube& cpy)
	: CEventCube(cpy)
{
    m_isEventTriggered = false;
}

CPortalCube::~CPortalCube()
{
}


_int       CPortalCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);


    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX , this );

    return 0;
}

void		CPortalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void        CPortalCube::Render_GameObject()
{
    //렌더 
    m_pGraphicDev->SetTransform(D3DTS_WORLD , m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

void CPortalCube::PlayEvent()
{
    CGameObject* Player = CObjectManager::GetInstance()->Get(OBJ_PLAYER);
    CGameObject* TargetPosCube = CObjectManager::GetInstance()->GetPositionCube(m_tHandle.m_sEventIndex);

    NULLPTR_RETURN(Player); NULLPTR_RETURN(TargetPosCube);

    if (m_isEventTriggered == false)
    {
        if (100 < m_tHandle.m_sEventIndex && m_tHandle.m_sEventIndex <= 150)
        {
            _vec2* CenterUV = m_pBuffer->GetCenterUV();

            for (int i = 0; i < CUBE_FACE_END; ++i)
            {
                m_pBuffer->ChangeTextureCubeFaceUV(static_cast<CUBE_FACE>(i), _vec2(CenterUV[i].x + TILE_UV_SIZE, CenterUV[i].y));
            }
        }

        m_isEventTriggered = true;
    }

    auto PlayerTransform = Player->Get_Transform();

    auto TargetTransform = TargetPosCube->Get_Transform();

    _vec3 TargetPos = *TargetTransform->GetInfoPos();
    _vec3 TargetRot = TargetTransform->Get_Angle();

    //플레이어가 큐브위 Rot , Pos 값을 이어받는다.
    PlayerTransform->Set_Pos(TargetPos.x, TargetPos.y + TargetTransform->Get_Scale().y * 0.5f, TargetPos.z);
    PlayerTransform->Set_Angle(TargetRot.x, TargetRot.y, TargetRot.z);

    static_cast<CPlayer*>(Player)->SetPlayerDefaultY(TargetPos.y + TargetTransform->Get_Scale().y * 0.5f);

    CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");

}


HRESULT		CPortalCube::Add_Component()
{

    if (FAILED(CEventCube::Add_Component()))
        return E_FAIL;

    CComponent* newCom = nullptr;

    newCom = m_pBuffer = CLONE_PROTOTYPE( PROTO_CUBETEX , CCubeTex);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer" , newCom });

    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    if (newCom == nullptr)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Collider" , newCom });
    m_pColliderCom->Set_MyOwner(this);

    return S_OK;
}

HRESULT	    CPortalCube::Ready_GameObject()
{
    if (FAILED(CPortalCube::Add_Component()))
        return E_FAIL;

    return S_OK;
}


CPortalCube* CPortalCube::Create(LPDIRECT3DDEVICE9 Device)
{
    CPortalCube* pCPortalCube = new CPortalCube(Device);

    if (FAILED(pCPortalCube->Ready_GameObject()))
    {
        Safe_Release(pCPortalCube);
        MSG_BOX("PortalCube Create Failed");
        return nullptr;
    }

    return pCPortalCube;
}



void		CPortalCube::Free()
{
    CGameObject::Free();
}
