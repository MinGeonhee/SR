#include "pch.h"
#include "CParticle.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CParticleMgr.h"

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

    Make_Random_Position(_vec3{ 1.f, 1.f, 1.f });
    m_pTransformCom->Set_Scale(_vec3{ 0.2f, 0.2f, 0.2f });

    return S_OK;
}

_int CParticle::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 스파클 상태일때
    if (m_bIsSparkling)
    {
        Update_Sparkle(fTimeDelta);
    }
    // 올라가는 상태일때
    if (m_bIsGoingUp)
    {
        Update_Go_Up(fTimeDelta);
    }
    // 드랍된 상태일때
    if (m_bIsDropped)
    {
        Update_Drop(fTimeDelta);
    }


    return 0;
}

void CParticle::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CParticle::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    // 텍스처 필터링을 Point로 설정(픽셀 아트 스타일 유지)
    m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

    CResourceManager::GetInstance()->Get<CTexture>(L"ParticleTex")->Set_Texture();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


    if (m_bIsRenderOn)
    {
        m_pBufferCom->Render_Buffer();
    }

    // 다시 켜줌.
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CParticle::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_PARTICLERECTEX, CParticleRecTex);

    if (nullptr == pComponent)
        return E_FAIL;

    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

    // transform 컴포넌트.

    pComponent = m_pTransformCom = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

    if (pComponent == NULL)
        return E_FAIL;

    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });



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

void CParticle::Make_Random_Position(_vec3 _vRange)
{
    // 시드 생성 (시간 기반으로 고유하게 생성)
    unsigned seed = static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 rng(seed);  // 난수 생성기

    // 각 축(X, Y, Z)에 대해 범위 지정
    uniform_real_distribution<float> distX(-(_vRange.x * 0.5f), (_vRange.x * 0.5f));
    uniform_real_distribution<float> distY(-(_vRange.y * 0.5f), (_vRange.y * 0.5f));
    uniform_real_distribution<float> distZ(-(_vRange.z * 0.5f), (_vRange.z * 0.5f));

    // 랜덤 좌표 생성
    float fRandX = distX(rng);
    float fRandY = distY(rng);
    float fRandZ = distZ(rng);

    m_pTransformCom->Set_Pos(fRandX, fRandY, fRandZ);
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

    m_pTransformCom->Set_Scale(_vec3{ fRandSize, fRandSize, fRandSize });
}

void CParticle::Set_Particle_Size_Constant(float _fSize)
{
    m_pTransformCom->Set_Scale(_vec3{ _fSize,_fSize,_fSize });
}

void CParticle::Set_Particle_Texture_Index(int _iXIndex, int _iYIndex)
{
    m_pBufferCom->Change_CenterUV(_vec2{ (float)_iXIndex, (float)_iYIndex });
}

void CParticle::Reset_All_Particle_Type_Values()
{
    m_bIsRenderOn = false;
    m_bIsSparkling = false;
    m_bIsGoingUp = false;
    m_bIsDropped = false;
    m_fActivationTimer = 0.f;
    m_vDir_Drop = { 0.f, 0.f, 0.f };
}

void CParticle::Update_Sparkle(const _float& fTimeDelta)
{
    m_bIsRenderOn = true;
    m_fActivationTimer += fTimeDelta; // 활성화 타이머 증가

    if (m_fActivationTimer > m_fLifeTime) // 생명주기 동안 활성화 상태 유지
    {
        m_bIsRenderOn = false;
        m_bIsSparkling = false; // 활성화 상태도 끔.
        m_fActivationTimer = 0.f; // 타이머 초기화
    }
}

void CParticle::Update_Go_Up(const _float& fTimeDelta)
{
    m_bIsRenderOn = true;
    m_fActivationTimer += fTimeDelta; // 활성화 타이머 증가

    _vec3 vUpDir = { 0.f, 1.f, 0.f };
    m_pTransformCom->Move_Pos(&vUpDir, m_fSpeed_GoUp, fTimeDelta); // 위로 이동

    if (m_fActivationTimer > m_fLifeTime) // 생명주기 동안 활성화 상태 유지
    {
        m_bIsRenderOn = false;
        m_bIsGoingUp = false; // 활성화 상태도 끔.
        m_fActivationTimer = 0.f; // 타이머 초기화
    }
}

void CParticle::Update_Drop(const _float& fTimeDelta)
{
    // 최초로 들어왓을때
    if (!m_bIsRenderOn)
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

    m_bIsRenderOn = true;

    // 활성화 타이머 증가
    m_fActivationTimer += fTimeDelta;

    // 시간
    float t = m_fActivationTimer;

    _vec3 pos;

    // 포물선 공식
    pos.x = m_fSpeed_XZ_Drop * m_vDir_Drop.x * t;
    pos.y = m_fSpeed_Y_Drop * t - (1.f * m_fGravity_Drop * t * t);
    pos.z = m_fSpeed_XZ_Drop * m_vDir_Drop.z * t;

    m_pTransformCom->Move_Pos(&pos, 1.f, fTimeDelta); // 위로 이동

    if (m_fActivationTimer > m_fLifeTime) // 생명주기 동안 활성화 상태 유지
    {
        m_bIsRenderOn = false;
        m_bIsDropped = false; // 활성화 상태도 끔.
        m_fActivationTimer = 0.f; // 타이머 초기화
    }
}

void CParticle::Free()
{
    Engine::CGameObject::Free();
}
