#include "CParticleSystem_Sparkle.h"
#include "CRendererManager.h"
#include "CParticle.h"

CParticleSystem_Sparkle::CParticleSystem_Sparkle(const CParticleSystem_Sparkle& rhs)
    :CParticleSystem(rhs)
{

}

CParticleSystem_Sparkle::CParticleSystem_Sparkle(LPDIRECT3DDEVICE9 pGraphicDev)
    :CParticleSystem(pGraphicDev)
{
}

CParticleSystem_Sparkle::~CParticleSystem_Sparkle()
{
}

HRESULT CParticleSystem_Sparkle::Ready_GameObject()
{
    CParticleSystem::Ready_GameObject();

    m_pTransform->Set_Pos(5.f, 3.f, 5.f);

    m_vBoundingBoxScale = { 0.3f,0.3f,0.3f };
    m_iMaxParticleNum = 10;
    m_iParticleNum = 10;
    m_eParticleType = PARTICLE_TYPE_SPARKLE;

    m_iParticleTextureIndex_X = 0;
    m_iParticleTextureIndex_Y = 0;

    m_fParticleSize = 0.1f;

    m_bIsParticleRandomSize = false;


    return S_OK;
}

_int CParticleSystem_Sparkle::Update_GameObject(const _float& fTimeDelta)
{
    if (m_eParticleSystemStatus == PARTICLE_SYSTEM_STATUS_ACTIVE)
    {
        CParticleSystem::Update_GameObject(fTimeDelta);

        Particle_Update_Sparkle(fTimeDelta);
        if (m_bParticleSystemOff)
        {
            m_eParticleSystemStatus = PARTICLE_SYSTEM_STATUS_INACTIVE;

            for (CGameObject* p : m_vecParticles)
            {
                static_cast<CParticle*>(p)->Reset_And_Return();
            }
            CObjectPoolingManager::GetInstance()->Return(this);
        }
    }

    return 0;
}

void CParticleSystem_Sparkle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_eParticleSystemStatus == PARTICLE_SYSTEM_STATUS_ACTIVE)
    {
        // 뒤로 들어가야함
        CParticleSystem::LateUpdate_GameObject(fTimeDelta);
    }
}

void CParticleSystem_Sparkle::Render_GameObject()
{
    if (m_eParticleSystemStatus == PARTICLE_SYSTEM_STATUS_ACTIVE)
    {
        // 뒤로 들어가야함
        CParticleSystem::Render_GameObject();
    }
}

HRESULT CParticleSystem_Sparkle::Add_Component()
{
    CParticleSystem::Add_Component();

    return S_OK;
}

CParticleSystem_Sparkle* CParticleSystem_Sparkle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleSystem_Sparkle* pPS = new CParticleSystem_Sparkle(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("Particle System Create Failed");
        return nullptr;
    }


    return pPS;
}

void CParticleSystem_Sparkle::Apply_Values_To_Particles()
{
    m_vecParticles = CObjectPoolingManager::GetInstance()->GetMany(OBJ_PARTICLE, m_iMaxParticleNum);

    _vec3 vParentPos = *(m_pTarget->Get_Transform()->GetInfoPos());
    m_pTransform->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);


    for (int i = 0; i < m_iParticleNum; i++)
    {
        // 크기 설정
        // 1. 일정한 크기로 설정할 때
        if (!m_bIsParticleRandomSize)
        {
            static_cast<CParticle*>(m_vecParticles[i])->Set_Particle_Size_Constant(m_fParticleSize);
        }
        // 2. 랜덤한 크기로 설정할 때
        else
        {
            _vec2 vRandomScaleRange = { 0.1f, 0.2f };
            static_cast<CParticle*>(m_vecParticles[i])->Make_Random_Size(vRandomScaleRange);
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

        static_cast<CParticle*>(m_vecParticles[i])->Make_Random_Position_XYZ(m_vBoundingBoxScale, vPos + _vec3{ 0.f,0.15f,0.f });

        // 값 리셋
        static_cast<CParticle*>(m_vecParticles[i])->Reset_All_Particle_Type_Values();

        // 타입 지정
        static_cast<CParticle*>(m_vecParticles[i])->Set_ParticleType(m_eParticleType);

        // 텍스쳐 지정
        static_cast<CParticle*>(m_vecParticles[i])->Set_Particle_Texture_Index(m_iParticleTextureIndex_X, m_iParticleTextureIndex_Y);

        static_cast<CParticle*>(m_vecParticles[i])->Set_Collider_Type(COLLIDER_NONE);
    }

    // 타이머 초기화
    m_fParticleTimer = 0.f;

}

void CParticleSystem_Sparkle::Particle_Update_Sparkle(const _float& fTimeDelta)
{
    m_fParticleTimer += fTimeDelta * 1.f;

    for (int i = 0; i < m_iParticleNum; i++)
    {
        if ((int)m_fParticleTimer % (m_iParticleNum) == i)
        {
            if (static_cast<CParticle*>(m_vecParticles[i])->Get_ParticleStatus() != PARTICLE_STATUS_ACTIVE)
            {
                static_cast<CParticle*>(m_vecParticles[i])->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);
                static_cast<CParticle*>(m_vecParticles[i])->Set_Animation();
            }
            else
            {
                static_cast<CParticle*>(m_vecParticles[i])->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);
            }
        }
    }
}




void CParticleSystem_Sparkle::Free()
{
    Engine::CParticleSystem::Free();
}
