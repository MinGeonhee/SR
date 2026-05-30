#include "CParticleSystem_Blood.h"
#include "CRendererManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CParticle.h"

CParticleSystem_Blood::CParticleSystem_Blood(const CParticleSystem_Blood& rhs)
    :CParticleSystem(rhs)
{

}

CParticleSystem_Blood::CParticleSystem_Blood(LPDIRECT3DDEVICE9 pGraphicDev)
    :CParticleSystem(pGraphicDev)
{
}

CParticleSystem_Blood::~CParticleSystem_Blood()
{
}

HRESULT CParticleSystem_Blood::Ready_GameObject()
{
    CParticleSystem::Ready_GameObject();

    
    m_pTransform->Set_Pos(5.f, 3.f, 5.f);

    m_vBoundingBoxScale = { 0.5f,0.3f,0.5f };
    m_iMaxParticleNum = 5;
    m_iParticleNum = 5;
    m_eParticleType = PARTICLE_TYPE_DROP_ON_FLOOR;

    m_iParticleTextureIndex_X = 0;
    m_iParticleTextureIndex_Y = 0;

    m_fParticleSize = 1.f;

    m_bIsParticleRandomSize = true;

    m_fSpeed_XZ_Drop = 1.1f;
    m_fSpeed_Y_Drop = 7.f;
    m_fGravity_Drop = 14.f;
 

    return S_OK;
}

_int CParticleSystem_Blood::Update_GameObject(const _float& fTimeDelta)
{
    
    // 바운딩 박스 업데이트
    //m_pBoundingBox->Update_GameObject(fTimeDelta);

	_vec3 vPos = *(m_pTarget->Get_Transform()->GetInfoPos());
    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

    CParticleSystem::Update_GameObject(fTimeDelta);
    
 

    return 0;
}

void CParticleSystem_Blood::LateUpdate_GameObject(const _float& fTimeDelta)
{
    // 뒤로 들어가야함
    CParticleSystem::LateUpdate_GameObject(fTimeDelta);
}

void CParticleSystem_Blood::Render_GameObject()
{
    // 뒤로 들어가야함
    CParticleSystem::Render_GameObject();
}

HRESULT CParticleSystem_Blood::Add_Component()
{
    CParticleSystem::Add_Component();

    return S_OK;
}

CParticleSystem_Blood* CParticleSystem_Blood::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleSystem_Blood* pPS = new CParticleSystem_Blood(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("Particle System Create Failed");
        return nullptr;
    }


    return pPS;
}

void CParticleSystem_Blood::Make_And_Apply_Values_To_Particle()
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
            _vec2 vRandomScaleRange = { 0.15f, 0.25f };
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

        static_cast<CParticle*>(p)->Make_Random_Position_XYZ(m_vBoundingBoxScale, vPos + _vec3{ 0.f,0.4f,0.f });

        // 값 리셋
        static_cast<CParticle*>(p)->Reset_All_Particle_Type_Values();

        // 타입 지정
        static_cast<CParticle*>(p)->Set_ParticleType(m_eParticleType);

        // 텍스쳐 지정
        static_cast<CParticle*>(p)->Set_Particle_Texture_Index(0, 0);

        static_cast<CParticle*>(p)->Set_ColliderOffset(-0.05f);
        static_cast<CParticle*>(p)->Set_IsCollisionOn(true);

        static_cast<CParticle*>(p)->m_fGravity_Drop = m_fGravity_Drop;
        static_cast<CParticle*>(p)->m_fSpeed_XZ_Drop = m_fSpeed_XZ_Drop;
        static_cast<CParticle*>(p)->m_fSpeed_Y_Drop = m_fSpeed_Y_Drop;
        static_cast<CParticle*>(p)->m_fSpeed = 1.f;


        static_cast<CParticle*>(p)->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);

    }
    // 타이머 초기화
    m_fParticleTimer = 0.f;

}



void CParticleSystem_Blood::Free()
{
    Engine::CParticleSystem::Free();
}
