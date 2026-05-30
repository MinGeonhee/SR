#include "CGameObject.h"
#include "CTransform.h"
#include "CColliderBase.h"
#include "CCameraManager.h"
#include "CTexture.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev), m_pTarget(nullptr), m_fViewZ(0) , m_pColliderCom(nullptr), m_pTransform(nullptr) , m_pStatusCom(nullptr) , m_tHandle()
{
    m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
    : m_pGraphicDev(rhs.m_pGraphicDev), m_pTarget(nullptr), m_fViewZ(rhs.m_fViewZ)
{
    m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{

}

CComponent* CGameObject::Get_Component(COMPONENTID eID, const wstring& pComponentTag)
{
    CComponent* pComponent = Find_Component(eID, pComponentTag);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent;
}

void CGameObject::Set_SubType(void* SubType)
{
    int a = 0;
}

HRESULT CGameObject::Ready_GameObject()
{


    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        if (pComponent.second)
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
        if(pComponent.second)
        pComponent.second->LateUpdate_Component(fTimeDelta);
}

void CGameObject::Render_GameObject()
{

}

void CGameObject::Setting_GameObject(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale)
{
    m_tHandle = tHandle;
    
    if (m_pTransform)
    {   // 스 자 이 설정
        m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
        m_pTransform->Set_Angle(vRot.x, vRot.y, vRot.z);
        m_pTransform->Set_Scale(vScale);
    }
}

void CGameObject::Compute_ViewZ(const _vec3* pPos)
{
    _vec3 vCamPos;
    auto Cam = CCameraManager::GetInstance()->Get_Camera(L"Main_Camera");

    // ==========  예외 처리 =======
    if (Cam)
        vCamPos = *Cam->Get_Transform()->GetInfoPos();

    else 
        return;

    // ==========  예외 처리 =======

    _vec3       vDir = vCamPos - *pPos;

    m_fViewZ = D3DXVec3Length(&vDir);
}

void CGameObject::SemiUpdate()
{
    if (m_pTransform != nullptr)
        m_pTransform->Update_Component(0.f);

    if (m_pColliderCom != nullptr)
        m_pColliderCom->LateUpdate_Component(0.f);
}

CComponent* CGameObject::Find_Component(COMPONENTID eID, const wstring& pComponentTag)
{
    auto        iter = find_if(m_mapComponent[eID].begin(),
        m_mapComponent[eID].end(),
        [&pComponentTag](const auto& pair) {return pComponentTag == pair.first; });

    if (iter == m_mapComponent[eID].end())
        return nullptr;

    return iter->second;
}


void CGameObject::Free()
{
    for (_uint i = 0; i < ID_END; ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
        m_mapComponent[i].clear();
    }

     Safe_Release(m_pGraphicDev);
}
