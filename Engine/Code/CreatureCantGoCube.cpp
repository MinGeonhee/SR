#include "CreatureCantGoCube.h"
#include "CTransform.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CCollisionManager.h"
#include "CResourceManager.h"

CreatureCantGoCube::CreatureCantGoCube(const CreatureCantGoCube& cpy)
    : CGameObject(cpy), m_IsEvent(cpy.m_IsEvent), m_eCubeType(cpy.m_eCubeType)
{
    m_eCubeType = cpy.m_eCubeType;
    m_tHandle.m_eType = cpy.m_tHandle.m_eType;

}

CreatureCantGoCube::CreatureCantGoCube(LPDIRECT3DDEVICE9 device)
    : CGameObject(device), m_IsEvent(false), m_eCubeType(CUBE_TYPE::CUBE_EVENT)
{
    m_eCubeType = CUBE_CREATURECANTGO;
    m_tHandle.m_eType = OBJ_CREATURECANTGO;
}


CreatureCantGoCube::~CreatureCantGoCube()
{
}


HRESULT        CreatureCantGoCube::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CreatureCantGoCube::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CreatureCantGoCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    return;
}

void CreatureCantGoCube::Render_GameObject()
{
    //m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
}

// 단순 트랜스 폼만 생성해준다..
HRESULT        CreatureCantGoCube::Add_Component()
{
    CComponent* newCom = nullptr;

    //트랜스폼
    newCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans" , newCom });
    m_pTransform->Set_MyOwner(this);

    //AAABBB콜라이더
    newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", newCom });
    m_pColliderCom->Set_MyOwner(this);


    return S_OK;
}



void CreatureCantGoCube::SemiUpdate()
{
    // 두 콜라이더가 모두 nptr 이 아닐때만 업데이트 
    if (m_pTransform != nullptr)
        m_pTransform->Update_Component(0.f);


    if (m_pColliderCom != nullptr)
        m_pColliderCom->LateUpdate_Component(0.f);
}



CreatureCantGoCube* CreatureCantGoCube::Create(LPDIRECT3DDEVICE9 device)
{
    CreatureCantGoCube* pObj = new CreatureCantGoCube(device);

    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pObj;

}

void CreatureCantGoCube::Free()
{
    CGameObject::Free();
}
