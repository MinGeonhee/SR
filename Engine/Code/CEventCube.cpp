#include "CEventCube.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CCollisionManager.h"
#include "CResourceManager.h"

CEventCube::CEventCube(const CEventCube& cpy)
    : CGameObject(cpy), m_IsEvent(cpy.m_IsEvent), m_eCubeType(cpy.m_eCubeType), m_pBuffer(nullptr)
    ,m_eCubeEventType(cpy.m_eCubeEventType) , m_isEventTriggered(cpy.m_isEventTriggered)
{
    m_eCubeType = cpy.m_eCubeType;
    m_tHandle.m_eType = cpy.m_tHandle.m_eType;

}

CEventCube::CEventCube(LPDIRECT3DDEVICE9 device)
    : CGameObject(device) , m_IsEvent(false), m_pBuffer(nullptr) , m_eCubeType(CUBE_TYPE::CUBE_EVENT)
    , m_eCubeEventType(CUBE_EVENT_TYPE::_END) , m_isEventTriggered(false)
{
    m_eCubeType = CUBE_EVENT;
    m_tHandle.m_eType = OBJ_EVENTCUBE;
}


CEventCube::~CEventCube()
{
}


HRESULT        CEventCube::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;


    m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Tile");

    return S_OK;
}

_int CEventCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == true)
        CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX, this);

    return 0;
}

void CEventCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_IsEvent == false)
        return;

    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEventCube::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBuffer->Render_Buffer();
}

// 단순 트랜스 폼만 생성해준다..
HRESULT        CEventCube::Add_Component()
{
    CComponent* newCom = nullptr;

    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans" , newCom });
    m_pTransform->Set_MyOwner(this);

    return S_OK;
}

void CEventCube::SetVerFilpArr(_bool* arr)
{
    if (arr == nullptr)
        return;

    m_pBuffer->SetFilpVerticalArr(arr);
}

void CEventCube::SetHorFilpArr(_bool* arr)
{
    if (arr == nullptr)
        return;

    m_pBuffer->SetFilpHorizontalArr(arr);
}

void CEventCube::Change_CubeTextureFaceUV(const CUBE_FACE& eType, const _vec2& vCenterUV)
{
    m_pBuffer->ChangeTextureCubeFaceUV(eType, vCenterUV);
}

void CEventCube::SemiUpdate()
{
    // 두 콜라이더가 모두 nptr 이 아닐때만 업데이트 
    if(m_pTransform != nullptr)
        m_pTransform->Update_Component(0.f);


    if( m_pColliderCom != nullptr)
        m_pColliderCom->LateUpdate_Component(0.f);
}



CEventCube* CEventCube::Create(LPDIRECT3DDEVICE9 device)
{
    return nullptr;
}

void CEventCube::Play_BrokenCubeParticleSystem()
{
    m_pCubeParticleSystem = dynamic_cast<CParticleSystem_BrokenCube*>(CObjectPoolingManager::GetInstance()->
        Get(OBJ_PARTICLE_SYSTEM_BROKENCUBE));
    m_pCubeParticleSystem->Set_Target(this);
    m_pCubeParticleSystem->Activate_ParticleSystem();
    m_pCubeParticleSystem->Make_And_Apply_Values_To_Particle();

    m_pColliderCom->Set_ColliderType(COLLIDERTYPE::COLLIDER_NONE);
}

void CEventCube::Free()
{
    CGameObject::Free();
}
