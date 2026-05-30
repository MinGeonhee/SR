#include "CFogSystem.h"
#include "CRendererManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CFog.h"

CFogSystem::CFogSystem(const CFogSystem& rhs)
    :CGameObject(rhs)
{

}

CFogSystem::CFogSystem(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CFogSystem::~CFogSystem()
{
}

HRESULT CFogSystem::Ready_GameObject()
{
    // 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_tHandle.m_eType = OBJ_FOG_SYSTEM;

    m_iMaxFogNum = 50;
    m_vBoundingBoxScale = { 30.f,0.5f,30.f };
    m_pTransform->Set_Pos(26.5f, 2.f, 42.5f);

    for (int i = 0; i < m_iMaxFogNum; i++)
    {
        CFog* pFog = CFog::Create(m_pGraphicDev);
        _vec3 pos = *m_pTransform->GetInfoPos();
        pFog->Make_Random_Position(m_vBoundingBoxScale, pos);
        m_vecFogs.push_back(pFog);
    }
    return S_OK;
}

_int CFogSystem::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    for (int i = 0; i < m_iMaxFogNum; i++)
    {
        m_vecFogs[i]->Update_GameObject(fTimeDelta);
    }

    return 0;
}

void CFogSystem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    for (int i = 0; i < m_iMaxFogNum; i++)
    {
        m_vecFogs[i]->LateUpdate_GameObject(fTimeDelta);
    }

}

void CFogSystem::Render_GameObject()
{
    CGameObject::Render_GameObject();

    for (int i = 0; i < m_iMaxFogNum; i++)
    {
        m_vecFogs[i]->Render_GameObject();
    }
}

HRESULT CFogSystem::Add_Component()
{
    CComponent* pComponent = NULL;

    // transform 컴포넌트.
    pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

    if (pComponent == NULL)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    //================ 콜라이더 AAABBB 충돌 컴포넌트 ===============
    pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere);

    if (nullptr == pComponent)
        return E_FAIL;

    m_pColliderCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });
    m_pColliderCom->Set_ColliderType(COLLIDER_NONE);



    return S_OK;
}

CFogSystem* CFogSystem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CFogSystem* pPS = new CFogSystem(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("CFogSystem Create Failed");
        return nullptr;
    }


    return pPS;
}



void CFogSystem::Free()
{
    for (auto& fog : m_vecFogs)
        Safe_Release(fog);

    Engine::CGameObject::Free();
}
