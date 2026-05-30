#include "CParticleSystem.h"
#include "CRendererManager.h"
#include "CObjectManager.h"
#include "CParticle.h"

CParticleSystem::CParticleSystem(const CParticleSystem& rhs)
    :CGameObject(rhs)
{

}

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CParticleSystem::~CParticleSystem()
{
}

HRESULT CParticleSystem::Ready_GameObject()
{
    // 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_tHandle.m_eType = OBJ_PARTICLE_SYSTEM;

    return S_OK;
}

_int CParticleSystem::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

    return 0;
}

void CParticleSystem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CParticleSystem::Render_GameObject()
{
    CGameObject::Render_GameObject();
}

HRESULT CParticleSystem::Add_Component()
{
    CComponent* pComponent = NULL;

    // transform 컴포넌트.
    pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

    if (pComponent == NULL)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

CParticleSystem* CParticleSystem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleSystem* pPS = new CParticleSystem(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("Particle System Create Failed");
        return nullptr;
    }


    return pPS;
}

void CParticleSystem::Activate_ParticleSystem()
{
    if (m_eParticleSystemStatus == PARTICLE_SYSTEM_STATUS::PARTICLE_SYSTEM_STATUS_INACTIVE)
    {
        m_eParticleSystemStatus = PARTICLE_SYSTEM_STATUS::PARTICLE_SYSTEM_STATUS_ACTIVE;

    }
}


void CParticleSystem::Free()
{
    Engine::CGameObject::Free();
}
