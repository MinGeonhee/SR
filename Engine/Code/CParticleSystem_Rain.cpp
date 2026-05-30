#include "CParticleSystem_Rain.h"
#include "CRendererManager.h"
#include "CObjectManager.h"
#include "CParticle.h"
#include "CCameraManager.h"

CParticleSystem_Rain::CParticleSystem_Rain(const CParticleSystem_Rain& rhs)
    :CParticleSystem(rhs)
{

}

CParticleSystem_Rain::CParticleSystem_Rain(LPDIRECT3DDEVICE9 pGraphicDev)
    :CParticleSystem(pGraphicDev)
{
}

CParticleSystem_Rain::~CParticleSystem_Rain()
{
}

HRESULT CParticleSystem_Rain::Ready_GameObject()
{
    CParticleSystem::Ready_GameObject();

    
    m_pTransform->Set_Pos(5.f, 0.f, 5.f);

    m_vBoundingBoxScale = { 2.f,2.f,2.f };
    m_iMaxParticleNum = 25;
    m_iParticleNum = 25;
    m_eParticleType = PARTICLE_TYPE_RAIN;

    m_iParticleTextureIndex_X = 1;
    m_iParticleTextureIndex_Y = 6;

    m_fParticleSize = 0.13f;

    m_bIsParticleRandomSize = false;

    m_eParticleSystemStatus = PARTICLE_SYSTEM_STATUS_ACTIVE;

    m_pTarget = CCameraManager::GetInstance()->Get_Camera(L"Main_Camera");

    m_vecParticles = CObjectPoolingManager::GetInstance()->GetMany(OBJ_PARTICLE, m_iMaxParticleNum);

  

    return S_OK;
}

_int CParticleSystem_Rain::Update_GameObject(const _float& fTimeDelta)
{
    
    // 바운딩 박스 업데이트
    //m_pBoundingBox->Update_GameObject(fTimeDelta);

	_vec3 vPos = *(m_pTarget->Get_Transform()->GetInfoPos());
    m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

    CParticleSystem::Update_GameObject(fTimeDelta);
    
    Particle_Update_Rain(fTimeDelta);


    return 0;
}

void CParticleSystem_Rain::LateUpdate_GameObject(const _float& fTimeDelta)
{
    // 뒤로 들어가야함
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    
}

void CParticleSystem_Rain::Render_GameObject()
{
    // 뒤로 들어가야함
    CGameObject::Render_GameObject();
}

HRESULT CParticleSystem_Rain::Add_Component()
{
    CParticleSystem::Add_Component();

    return S_OK;
}

CParticleSystem_Rain* CParticleSystem_Rain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleSystem_Rain* pPS = new CParticleSystem_Rain(pGraphicDev);

    if (FAILED(pPS->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pPS);
        MSG_BOX("Particle System Create Failed");
        return nullptr;
    }


    return pPS;
}

void CParticleSystem_Rain::Apply_Values_To_Particle(CParticle* _p)
{
     _p->Reset_All_Particle_Type_Values();
    // 크기 설정
    // 1. 일정한 크기로 설정할 때
    if (!m_bIsParticleRandomSize)
    {
        _p->Set_Particle_Size_Constant(m_fParticleSize);
    }
    // 2. 랜덤한 크기로 설정할 때
    else
    {
        _vec2 vRandomScaleRange = { 0.15f, 0.25f };
        _p->Make_Random_Size(vRandomScaleRange);
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

    _p->Make_Random_Position_XYZ(m_vBoundingBoxScale, vPos);

    // 타입 지정
    _p->Set_ParticleType(m_eParticleType);
    _p->Set_Particle_Texture_Index(m_iParticleTextureIndex_X, m_iParticleTextureIndex_Y);


    _p->Set_IsCollisionOn(false);
    _p->Set_Collider_Type(COLLIDER_NONE);

    _p->m_fSpeed_Rain = m_fSpeed_Rain;

    // 타이머 초기화
    m_fParticleTimer = 0.f;


    
}

void CParticleSystem_Rain::Particle_Update_Rain(const _float& fTimeDelta)
{
    m_fElapsedTime += fTimeDelta;

    for (int i = 0; i < m_iParticleNum; i++)
    {
        if (int(m_fElapsedTime * 100000) % 3 == 0)
        {
            if (static_cast<CParticle*>(m_vecParticles[i])->Get_ParticleStatus() != PARTICLE_STATUS_ACTIVE)
            {
                Apply_Values_To_Particle(static_cast<CParticle*>(m_vecParticles[i]));
                static_cast<CParticle*>(m_vecParticles[i])->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);
            }
            else
            {
                continue;;
            }
           
        }
    }
}


void CParticleSystem_Rain::Free()
{
    Engine::CParticleSystem::Free();
}
