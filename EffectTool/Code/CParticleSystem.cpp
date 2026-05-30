#include "pch.h"
#include "CParticleSystem.h"
#include "CParticleMgr.h"
#include "CRendererManager.h"

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

    m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);

    // 바운딩 박스 생성
    m_pBoundingBox = CBoundingBox::Create(m_pGraphicDev);
    if (m_pBoundingBox == NULL)
        return E_FAIL;

    // 파티클 입자 생성 (50개 미리 만들어놓기)
    for (int i = 0; i < m_iMaxParticleNum; i++)
    {
        CParticle* pParticle = CParticle::Create(m_pGraphicDev);
        if (pParticle == nullptr)
            return E_FAIL;
        m_vecParticles.push_back(pParticle);
    }

    return S_OK;
}

_int CParticleSystem::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    // 바운딩 박스 업데이트
    m_pBoundingBox->Update_GameObject(fTimeDelta);

    // 파티클 갯수 세팅
    m_iParticleNum = CParticleMgr::GetInstance()->Get_ParticleNum();

    // 어플라이 버튼 눌렸을때 새 값 세팅
    Apply_New_Particle_Values();

    // 만약 텍스쳐 인덱스가 변경되었다면 텍스쳐 변경
    Apply_New_Texture();

    // 파티클 타입이 변경되었는지 확인
    if (CParticleMgr::GetInstance()->Get_IsParticleTypeChanged())
    {
        m_iParticleType = CParticleMgr::GetInstance()->Get_ParticleType();
        CParticleMgr::GetInstance()->Set_IsParticleTypeChanged(false);
        // 파티클 타입이 변경되었을 때, 모든 파티클의 활성화 상태를 초기화
        for (int i = 0; i < m_iMaxParticleNum; i++)
        {
            m_vecParticles[i]->Set_isRenderOn(false);
            m_vecParticles[i]->Reset_All_Particle_Type_Values();

            _vec3 vBoxScale = CParticleMgr::GetInstance()->Get_BoundingBoxScale();
            m_vecParticles[i]->Make_Random_Position(vBoxScale);
        }
        // 타이머 초기화
        m_fParticleTimer = 0.f;
    }

    switch (m_iParticleType)
    {
    case CParticleMgr::PARTICLE_TYPE_TEST:
        Particle_Update_Test(fTimeDelta);
        break;
    case CParticleMgr::PARTICLE_TYPE_SPARKLE:
        Particle_Update_Sparkle(fTimeDelta);
        break;
    case CParticleMgr::PARTICLE_TYPE_GO_UP:
        Particle_Update_GoUp(fTimeDelta);
        break;
    case CParticleMgr::PARTICLE_TYPE_FLIES:
        Particle_Update_Flies(fTimeDelta);
        break;
    case CParticleMgr::PARTICLE_TYPE_DROP:
        Particle_Update_Drop(fTimeDelta);
        break;
    default:
        Particle_Update_Test(fTimeDelta);
        break;
    }

    return 0;
}

void CParticleSystem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_pBoundingBox->LateUpdate_GameObject(fTimeDelta);

    m_iParticleNum = CParticleMgr::GetInstance()->Get_ParticleNum();
    for (int i = 0; i < m_iParticleNum; i++)
    {
        m_vecParticles[i]->LateUpdate_GameObject(fTimeDelta);
    }

}

void CParticleSystem::Render_GameObject()
{
    m_pBoundingBox->Render_GameObject();

    m_iParticleNum = CParticleMgr::GetInstance()->Get_ParticleNum();
    for (int i = 0; i < m_iParticleNum; i++)
    {
        m_vecParticles[i]->Render_GameObject();
    }
}

HRESULT CParticleSystem::Add_Component()
{
    CComponent* pComponent = NULL;

    // transform 컴포넌트.
    pComponent = m_pTransformCom = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

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

void CParticleSystem::Apply_New_Particle_Values()
{
    // 어플라이 버튼 눌렸을때 새 값 세팅
    if (CParticleMgr::GetInstance()->Get_ApplySettings())
    {
        for (int i = 0; i < m_iMaxParticleNum; i++)
        {
            // 크기 설정
            // 1. 일정한 크기로 설정할 때
            if (!CParticleMgr::GetInstance()->Get_IsParticleRandomSize())
            {
                float fSize = CParticleMgr::GetInstance()->Get_ParticleSize();
                m_vecParticles[i]->Set_Particle_Size_Constant(fSize);
            }
            // 2. 랜덤한 크기로 설정할 때
            else
            {
                _vec2 vRandomScaleRange = { 0.3f, 0.5f };
                m_vecParticles[i]->Make_Random_Size(vRandomScaleRange);
            }

            // 랜덤 위치 설정
            _vec3 vBoxScale = CParticleMgr::GetInstance()->Get_BoundingBoxScale();
            m_vecParticles[i]->Make_Random_Position(vBoxScale);

            // 값 리셋
            m_vecParticles[i]->Set_isRenderOn(false);
            m_vecParticles[i]->Reset_All_Particle_Type_Values();

        }
        CParticleMgr::GetInstance()->Set_ApplySettings(false);

        // 타이머 초기화
        m_fParticleTimer = 0.f;
    }
}

void CParticleSystem::Apply_New_Texture()
{
    if (CParticleMgr::GetInstance()->Get_IsTextureIndexChanged())
    {
        int iXIndex = CParticleMgr::GetInstance()->Get_ParticleTextureIndex_X();
        int iYIndex = CParticleMgr::GetInstance()->Get_ParticleTextureIndex_Y();
        for (int i = 0; i < m_iMaxParticleNum; i++)
        {
            m_vecParticles[i]->Set_Particle_Texture_Index(iXIndex, iYIndex);
        }
        CParticleMgr::GetInstance()->Set_IsTextureIndexChanged(false);
    }
}

void CParticleSystem::Particle_Update_Test(const _float& fTimeDelta)
{
    for (int i = 0; i < m_iParticleNum; i++)
    {
        m_vecParticles[i]->Set_isRenderOn(true);
        m_vecParticles[i]->Update_GameObject(fTimeDelta);
    }
}

void CParticleSystem::Particle_Update_Sparkle(const _float& fTimeDelta)
{
    m_fParticleTimer += fTimeDelta * 0.95f;

    for (int i = 0; i < m_iParticleNum; i++)
    {
        if ((int)m_fParticleTimer % (m_iParticleNum + 1) == i)
        {
            m_vecParticles[i]->m_bIsSparkling = true;
        }
        m_vecParticles[i]->Update_GameObject(fTimeDelta);
    }
}

void CParticleSystem::Particle_Update_GoUp(const _float& fTimeDelta)
{
    for (int i = 0; i < m_iParticleNum; i++)
    {
        m_vecParticles[i]->m_bIsGoingUp = true;
        // 올라가는 속도 설정
        m_vecParticles[i]->m_fSpeed_GoUp = CParticleMgr::GetInstance()->Get_Speed_GoUp();
        m_vecParticles[i]->Update_GameObject(fTimeDelta);
    }
}

void CParticleSystem::Particle_Update_Drop(const _float& fTimeDelta)
{
    for (int i = 0; i < m_iParticleNum; i++)
    {
        m_vecParticles[i]->m_bIsDropped = true;
        // 드랍 속도 설정
        m_vecParticles[i]->m_fSpeed_XZ_Drop = CParticleMgr::GetInstance()->Get_DropSpeed_XZ();
        m_vecParticles[i]->m_fSpeed_Y_Drop = CParticleMgr::GetInstance()->Get_DropSpeed_Y();
        m_vecParticles[i]->m_fGravity_Drop = CParticleMgr::GetInstance()->Get_DropGravity();
        m_vecParticles[i]->Update_GameObject(fTimeDelta);
    }
}

void CParticleSystem::Particle_Update_Flies(const _float& fTimeDelta)
{
}





void CParticleSystem::Free()
{
    Engine::CGameObject::Free();
}
