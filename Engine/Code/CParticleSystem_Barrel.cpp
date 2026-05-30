#include "CParticleSystem_Barrel.h"
#include "CRendererManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CParticle.h"

CParticleSystem_Barrel::CParticleSystem_Barrel(const CParticleSystem_Barrel& rhs)
    :CParticleSystem(rhs)
{

}

CParticleSystem_Barrel::CParticleSystem_Barrel(LPDIRECT3DDEVICE9 pGraphicDev)
    :CParticleSystem(pGraphicDev)
{
}

CParticleSystem_Barrel::~CParticleSystem_Barrel()
{
}

HRESULT CParticleSystem_Barrel::Ready_GameObject()
{
    CParticleSystem::Ready_GameObject();


    m_pTransform->Set_Pos(5.f, 3.f, 5.f);

    m_vBoundingBoxScale = { 0.5f,0.5f,0.5f };
    m_iMaxParticleNum = 4;
    m_iParticleNum = 4;
    m_eParticleType = PARTICLE_TYPE_DROP;

    m_iParticleTextureIndex_X = 2;
    m_iParticleTextureIndex_Y = 2;

    m_fParticleSize = 1.f;

    m_bIsParticleRandomSize = true;

    m_fSpeed_XZ_Drop = 0.7f;
    m_fSpeed_Y_Drop = 3.6f;
    m_fGravity_Drop = 6.f;

    //Apply_Values_To_Particles();

    return S_OK;
}

_int CParticleSystem_Barrel::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos = *(m_pTarget->Get_Transform()->GetInfoPos());
    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

    CParticleSystem::Update_GameObject(fTimeDelta);


    m_fActivationTimer += fTimeDelta;

    if (m_fActivationTimer > m_fLifeTime_Drop) // 생명주기 동안 활성화 상태 유지
    {
        m_fActivationTimer = 0.f;
        m_eParticleSystemStatus = PARTICLE_SYSTEM_STATUS_INACTIVE;
        CObjectPoolingManager::GetInstance()->Return(this);
    }

   
    return 0;
}

void CParticleSystem_Barrel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    // 뒤로 들어가야함
    CParticleSystem::LateUpdate_GameObject(fTimeDelta);
}

void CParticleSystem_Barrel::Render_GameObject()
{
    // 뒤로 들어가야함
    CParticleSystem::Render_GameObject();
}

HRESULT CParticleSystem_Barrel::Add_Component()
{
    CParticleSystem::Add_Component();

    return S_OK;
}

CParticleSystem_Barrel* CParticleSystem_Barrel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleSystem_Barrel* pPS = new CParticleSystem_Barrel(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("Particle System Create Failed");
        return nullptr;
    }


    return pPS;
}

void CParticleSystem_Barrel::Make_And_Apply_Values_To_Particle()
{
    for (int i = 0; i < m_iParticleNum; i++)
    {
        CGameObject* p = CObjectPoolingManager::GetInstance()->Get(OBJ_PARTICLE);
        // 크기 설정
        // 1. 일정한 크기로 설정할 때
        if (!m_bIsParticleRandomSize)
        {
            static_cast<CParticle*>(p)->Set_Particle_Size_Constant(m_fParticleSize);
        }
        // 2. 랜덤한 크기로 설정할 때
        else
        {
            _vec2 vRandomScaleRange = { 0.23f, 0.33f };
            static_cast<CParticle*>(p)->Make_Random_Size(vRandomScaleRange);
        }

        _vec3 vPos;
        if (m_pTarget == nullptr)
        {
            vPos = { 0,0,0 };
        }
        else
        {
            vPos = *(m_pTarget->Get_Transform()->GetInfoPos());
        }

        static_cast<CParticle*>(p)->Make_Random_Position_XYZ(m_vBoundingBoxScale, vPos);

        // 값 리셋
        static_cast<CParticle*>(p)->Reset_All_Particle_Type_Values();

        // 타입 지정
        static_cast<CParticle*>(p)->Set_ParticleType(m_eParticleType);

        // 텍스쳐 지정
        static_cast<CParticle*>(p)->Set_Particle_Texture_Index(m_iParticleTextureIndex_X, m_iParticleTextureIndex_Y);

        static_cast<CParticle*>(p)->Set_ColliderOffset(-0.05f);
        static_cast<CParticle*>(p)->Set_Collider_Type(COLLIDER_NONE);

        static_cast<CParticle*>(p)->m_fGravity_Drop = m_fGravity_Drop;
        static_cast<CParticle*>(p)->m_fSpeed_XZ_Drop = m_fSpeed_XZ_Drop;
        static_cast<CParticle*>(p)->m_fSpeed_Y_Drop = m_fSpeed_Y_Drop;
        static_cast<CParticle*>(p)->m_fSpeed = 3.f;
    
        static_cast<CParticle*>(p)->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);
    }

    // 타이머 초기화
    m_fParticleTimer = 0.f;

}

void CParticleSystem_Barrel::Free()
{
    Engine::CParticleSystem::Free();
}
