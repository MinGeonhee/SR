#include <random> 
#include <chrono>
#include "CParticle.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CObjectPoolingManager.h"
#include "CSoundManager.h"

CParticle::CParticle(const CParticle& rhs)
    :CGameObject(rhs)
{
}

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CParticle::~CParticle()
{
}


HRESULT CParticle::Ready_GameObject()
{
    // 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_tHandle.m_eType = OBJTYPE::OBJ_PARTICLE;

    Make_Random_Position(_vec3{ 1.f, 1.f, 1.f },_vec3{ 0.f, 0.f, 0.f });

    m_pTransform->Set_Scale(_vec3{ 0.2f, 0.2f, 0.2f });

    m_pColliderCom->Set_ColliderScale(_vec3{0.1f,0.1f,0.1f});

    return S_OK;
}

_int CParticle::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bIsCollisionOn)
    {
        if (m_pColliderCom->Get_ColliderType() != COLLIDER_NONE)
        {
            CGameObject::Update_GameObject(fTimeDelta);
            if (m_bIsOnFloor)
            {
                m_pColliderCom->Set_ColliderType(COLLIDER_NONE);
            }
        }
    }
    else
    {
        CGameObject::Update_GameObject(fTimeDelta);
    }
    
    
    if (m_eParticleType == PARTICLE_TYPE_RAIN ||
        m_eParticleType == PARTICLE_TYPE_SMOKE)
    {
        CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHABLEND, this);
    }
    else
    {
        CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);
    }
    

    if (m_eParticleStatus != PARTICLE_STATUS_ACTIVE)
    {
        return 0;
    }

    if (m_eParticleStatus == PARTICLE_STATUS_ACTIVE)
    {
        switch (m_eParticleType)
        {
            case Engine::PARTICLE_TYPE_TEST:
                break;
            case Engine::PARTICLE_TYPE_SPARKLE:
                Update_Sparkle(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_GO_UP:
                Update_Go_Up(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_DROP:
                Update_Drop(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_DROP_ON_FLOOR:
                Update_Drop_On_Floor(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_SMOKE:
                Update_Smoke(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_INIT_SMOKE:
                Update_Init_Smoke(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_RAIN:
                Update_Rain(fTimeDelta);
                break;
            case Engine::PARTICLE_TYPE_BROKENCUBE:
                break;
            case Engine::PARTICLE_TYPE_END:
                break;
            default:
                break;
        }
    }



    _vec3 vPos =  *this->Get_Transform()->GetInfoPos();
    CGameObject::Compute_ViewZ(&vPos);

    return 0;
}

void CParticle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bIsCollisionOn)
    {
        if (m_pColliderCom->Get_ColliderType() != COLLIDER_NONE)
        {
            CGameObject::LateUpdate_GameObject(fTimeDelta);
        }
    }
    else
    {
        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}

void CParticle::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());


    if (m_eParticleStatus != PARTICLE_STATUS_INACTIVE)
    {
        switch (m_eParticleType)
        {   
            case PARTICLE_TYPE_SPARKLE:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex_Anim")->Set_Texture();
                m_pAnimationCom->Render_Buffer();
                break;
            case PARTICLE_TYPE_DROP_ON_FLOOR:
                if (m_bIsOnFloor)
                {
                    CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex_Ground")->Set_Texture();
                    m_pBufferOnFloorCom->Render_Buffer();
                }
                else
                {
                    CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
                    m_pBufferCom->Render_Buffer();
                }
                break;
            case PARTICLE_TYPE_DROP:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
                m_pBufferCom->Render_Buffer();
                break;
            case PARTICLE_TYPE_SMOKE:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
                m_pAlphaBufferCom->Render_Buffer();
                break;
            case PARTICLE_TYPE_INIT_SMOKE:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex_Anim")->Set_Texture();
                m_pAnimationCom->Render_Buffer();
                break;
            case PARTICLE_TYPE_RAIN:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
                m_pBufferCom->Render_Buffer();
                break;
            default:
                CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
                m_pBufferCom->Render_Buffer();
                break;
        }
        
    }

    // 다시 켜줌.
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

HRESULT CParticle::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_PARTICLERECTEX, CParticleRecTex);

    if (nullptr == pComponent)
        return E_FAIL;

    m_pBufferCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer",pComponent });

    // 바닥용 버퍼
    pComponent = m_pBufferOnFloorCom = CLONE_PROTOTYPE(PROTO_PARTICLEONFLOORTEX, CParticleOnFloorRecTex);
    
    if (nullptr == pComponent)
        return E_FAIL;
    
    m_pBufferOnFloorCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_OnFloorBuffer",pComponent });

    // 알파 연산 버퍼
    pComponent = m_pAlphaBufferCom = CLONE_PROTOTYPE(PROTO_PARTICLERECTEX_ALPHA, CParticleRecTex_Alpha);

    if (nullptr == pComponent)
        return E_FAIL;

    m_pAlphaBufferCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_AlphaBuffer",pComponent });


    // transform 컴포넌트
    pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

    if (pComponent == NULL)
        return E_FAIL;

    m_pTransform->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    // 콜라이더
    pComponent = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);

    if (nullptr == pComponent)
        return E_FAIL;

    m_pColliderCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider",pComponent });


    // 빌보드
    pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

    if (nullptr == pComponent)
        return E_FAIL;

    m_pBillboardCom->Set_MyOwner(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });


    // 애니메이션
    pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

    if (nullptr == pComponent)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });


    return S_OK;
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticle* pParticle = new CParticle(pGraphicDev);

    if (FAILED(pParticle->Ready_GameObject()))
    {
        // 만약에 제대로 생성이 되지 않았다면 바로 삭제.
        Safe_Release(pParticle);
        MSG_BOX("pParticle Create Failed");
        return nullptr;
    }

    return pParticle;
}

void CParticle::Make_Random_Position(_vec3 _vRange, _vec3 _vPos)
{
    // 시드 생성 (시간 기반으로 고유하게 생성)
    unsigned seed = static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 rng(seed);  // 난수 생성기

    // 각 축(X, Y, Z)에 대해 범위 지정
    uniform_real_distribution<float> distX(_vPos.x - _vRange.x / 2, _vPos.x + _vRange.x);
    uniform_real_distribution<float> distY(0.1f, _vRange.y);
    uniform_real_distribution<float> distZ(_vPos.z - _vRange.z / 2, _vPos.z + _vRange.z);

    // 랜덤 좌표 생성
    float fRandX = distX(rng);
    float fRandY = distY(rng);
    float fRandZ = distZ(rng);

    m_pTransform->Set_Pos(fRandX, fRandY, fRandZ);
}

void CParticle::Make_Random_Position_XYZ(_vec3 _vRange, _vec3 _vPos)
{
    // 시드 생성 (시간 기반으로 고유하게 생성)
    unsigned seed = static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 rng(seed);  // 난수 생성기

    // 각 축(X, Y, Z)에 대해 범위 지정
    uniform_real_distribution<float> distX(_vPos.x - _vRange.x / 2, _vPos.x + _vRange.x);
    uniform_real_distribution<float> distY(_vPos.y - _vRange.y / 2, _vPos.y + _vRange.y);
    uniform_real_distribution<float> distZ(_vPos.z - _vRange.z / 2, _vPos.z + _vRange.z);

    // 랜덤 좌표 생성
    float fRandX = distX(rng);
    float fRandY = distY(rng);
    float fRandZ = distZ(rng);

    m_pTransform->Set_Pos(fRandX, fRandY, fRandZ);
}

void CParticle::Make_Random_Size(_vec2 _vRange)
{
    // 시드 생성 (시간 기반으로 고유하게 생성)
    unsigned seed = static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 rng(seed);  // 난수 생성기

    // 랜덤 사이즈 범위 지정
    uniform_real_distribution<float> Size(_vRange.x, _vRange.y);

    // 랜덤 사이즈 생성
    float fRandSize = Size(rng);

    m_pTransform->Set_Scale(_vec3{ fRandSize, fRandSize, fRandSize });
    m_pColliderCom->Set_ColliderScale(_vec3{ fRandSize / 2,fRandSize / 2,fRandSize / 2 }); // 콜라이더 크기도 같이 변경
}

void CParticle::Set_Particle_Size_Constant(float _fSize)
{
    m_pTransform->Set_Scale(_vec3{ _fSize,_fSize,_fSize });
    m_pColliderCom->Set_ColliderScale(_vec3{ _fSize / 2,_fSize / 2 ,_fSize / 2 }); // 콜라이더 크기도 같이 변경
}

void CParticle::Set_Particle_Texture_Index(int _iXIndex, int _iYIndex)
{
    m_iTextureIndex_X = _iXIndex;
    m_iTextureIndex_Y = _iYIndex;

    if (m_eParticleType == PARTICLE_TYPE_SMOKE)
    {
        m_pAlphaBufferCom->Change_CenterUV(_vec2{ (float)m_iTextureIndex_X, (float)m_iTextureIndex_Y });
    }
    else
    {
        m_pBufferCom->Change_CenterUV(_vec2{ (float)m_iTextureIndex_X, (float)m_iTextureIndex_Y });
    }
    
    
}

void CParticle::Set_Collider_Type(COLLIDERTYPE _e)
{
    m_pColliderCom->Set_ColliderType(_e);
}

void CParticle::Reset_All_Particle_Type_Values()
{
	m_eParticleStatus = PARTICLE_STATUS_INACTIVE; 
    m_fActivationTimer = 0.f;
    m_vDir_Drop = { 0.f, 0.f, 0.f };
    m_bIsDropped = false;
    m_bIsOnFloor = false;
    m_pBillboardCom->Set_Active(true);
}

void CParticle::Update_Sparkle(const _float& fTimeDelta)
{
    m_fActivationTimer += fTimeDelta; // 활성화 타이머 증가

    if (m_fActivationTimer > m_fLifeTime_Sparkle) // 생명주기 동안 활성화 상태 유지
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
        m_fActivationTimer = 0.f; // 타이머 초기화
    }
}

void CParticle::Update_Go_Up(const _float& fTimeDelta)
{
    m_fActivationTimer += fTimeDelta; // 활성화 타이머 증가

    _vec3 vUpDir = { 0.f, 1.f, 0.f };
    m_pTransform->Move_Pos(&vUpDir, m_fSpeed_GoUp, fTimeDelta); // 위로 이동

    if (m_fActivationTimer > m_fLifeTime_GoUp) // 생명주기 동안 활성화 상태 유지
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
        m_fActivationTimer = 0.f; // 타이머 초기화
        Reset_And_Return();
    }
}

void CParticle::Update_Drop(const _float& fTimeDelta)
{
    // 최초로 들어왓을때
    if (!m_bIsDropped)
    {
        // 시드 생성 (시간 기반으로 고유하게 생성)
        unsigned seed = static_cast<unsigned>(
            chrono::high_resolution_clock::now().time_since_epoch().count());
        mt19937 rng(seed);  // 난수 생성기

        // 랜덤각 범위 지정
        uniform_real_distribution<float> XAngle(0.f, (2 * 3.14f));
        uniform_real_distribution<float> ZAngle(0.f, (2 * 3.14f));

        // 랜덤각 생성
        float fRandXAngle = XAngle(rng);
        float fRandZAngle = ZAngle(rng);

        float dx = cosf(fRandXAngle);
        float dz = sinf(fRandZAngle);
        m_vDir_Drop = { dx, 0.f, dz };
    }

    m_bIsDropped = true;

    // 활성화 타이머 증가
    m_fActivationTimer += fTimeDelta;

    // 시간
    float t = m_fActivationTimer;

    _vec3 pos;

    // 포물선 공식
    pos.x = m_fSpeed_XZ_Drop * m_vDir_Drop.x * t;
    pos.y = m_fSpeed_Y_Drop * t - (1.f * m_fGravity_Drop * t * t);
    pos.z = m_fSpeed_XZ_Drop * m_vDir_Drop.z * t;

    m_pTransform->Move_Pos(&pos, m_fSpeed, fTimeDelta); // 위로 이동

    if (m_fActivationTimer > m_fLifeTime_Drop) // 생명주기 동안 활성화 상태 유지
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
        m_fActivationTimer = 0.f; // 타이머 초기화
        Reset_And_Return();
    }
}

void CParticle::Update_Drop_On_Floor(const _float& fTimeDelta)
{
    if (m_bIsOnFloor)
    {
        m_bIsDropped = false;
        m_fActivationTimer = 0.f;
        return;
    }
    // 최초로 들어왓을때
    if (!m_bIsDropped)
    {
        // 시드 생성 (시간 기반으로 고유하게 생성)
        unsigned seed = static_cast<unsigned>(
            chrono::high_resolution_clock::now().time_since_epoch().count());
        mt19937 rng(seed);  // 난수 생성기

        // 랜덤각 범위 지정
        uniform_real_distribution<float> XAngle(0.f, (2 * 3.14f));
        uniform_real_distribution<float> ZAngle(0.f, (2 * 3.14f));

        // 랜덤각 생성
        float fRandXAngle = XAngle(rng);
        float fRandZAngle = ZAngle(rng);

        float dx = cosf(fRandXAngle);
        float dz = sinf(fRandZAngle);
        m_vDir_Drop = { dx, 0.f, dz };
    }

    m_bIsDropped = true;

    // 활성화 타이머 증가
    m_fActivationTimer += fTimeDelta;

    // 시간
    float t = m_fActivationTimer;

    _vec3 pos;

    // 포물선 공식
    pos.x = m_fSpeed_XZ_Drop * m_vDir_Drop.x * t;
    pos.y = m_fSpeed_Y_Drop * t - (1.f * m_fGravity_Drop * t * t);
    pos.z = m_fSpeed_XZ_Drop * m_vDir_Drop.z * t;

    m_pTransform->Move_Pos(&pos, m_fSpeed, fTimeDelta); // 위로 이동

}

void CParticle::Update_Smoke(const _float& fTimeDelta)
{
    m_fActivationTimer += fTimeDelta; // 활성화 타이머 증가
    m_pAlphaBufferCom->FadeOut(fTimeDelta);
    if (m_fActivationTimer > m_fLifeTime_Smoke) // 생명주기 동안 활성화 상태 유지
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
        m_fActivationTimer = 0.f; // 타이머 초기화
        Reset_And_Return();
    }
}

void CParticle::Set_AlphaBufferColor(float r, float g, float b)
{
    m_pAlphaBufferCom->SetColor(r, g, b);
}

void CParticle::Update_Init_Smoke(const _float& fTimeDelta)
{
    if(m_pAnimationCom->Get_IsAnimationEnd())
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
        Reset_And_Return();
    }
}

void CParticle::Update_Rain(const _float& fTimeDelta)
{
    if (m_pTransform->GetInfoPos()->y < 1.5)
    {
        m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
    }

    _vec3 vDownDir = { 0.f, -1.f, 0.f };
    m_pTransform->Move_Pos(&vDownDir, m_fSpeed_Rain, fTimeDelta); // 아래로 이동

}

void CParticle::Change_OnFloorType(float _yPos)
{
    if (!m_bIsOnFloor) {
        CSoundManager::GetInstance()->SoundPlay(
            L"blood-splash.wav",
            EVENT_SOUND,
            CSoundManager::GetInstance()->GetVol().sfx
        );
    }

    m_bIsOnFloor = true;
    m_pBillboardCom->Set_Active(false);
	// 위치 변경
	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	m_pTransform->Set_Pos(vPos.x, _yPos + 0.005f, vPos.z);
	m_pTransform->Set_Scale(_vec3{ 0.6f, 0.6f, 0.6f }); // 크기 변경
    
}

void CParticle::Set_Animation()
{
    switch (m_eParticleType)
    {
    case Engine::PARTICLE_TYPE_SPARKLE:
        m_pAnimationCom->SetGrid(8, 8); // 4행 8열
        m_pAnimationCom->SetRow(0);      // 맨 위 행
        m_pAnimationCom->AniMation_Init(8, 0.1f);	// 몇 열이고 출력 속도 몇인지
        m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
        m_pAnimationCom->PlayAnimation();
        break;
    case Engine::PARTICLE_TYPE_INIT_SMOKE:
        m_pAnimationCom->SetGrid(8, 8); // 4행 8열
        m_pAnimationCom->SetRow(1);      // 맨 위 행
        m_pAnimationCom->AniMation_Init(4, 0.1f);	// 몇 열이고 출력 속도 몇인지
        m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Once);
        m_pAnimationCom->PlayAnimation();
        break;
    default:
        break;
    }
    
}

void CParticle::Reset_And_Return()
{
    m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
    m_bIsDropped = false; // 활성화 상태도 끔.
    m_fActivationTimer = 0.f; // 타이머 초기화
    m_pColliderCom->Set_ColliderType(COLLIDER_AAABBB);
    m_bIsOnFloor = false;
    m_bIsCollisionOn = false;
    this->Set_Active(false);
    m_pAlphaBufferCom->SetAlpha(1.f);
    CObjectPoolingManager::GetInstance()->Return(this);
    
}

void CParticle::Free()
{
    Engine::CGameObject::Free();
}
