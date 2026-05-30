#include "CAnimation.h"
#include "CVIBuffer.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{

}

CAnimation::CAnimation(const CAnimation& rhs)
    : CVIBuffer(rhs)
    , m_vCenterUV(rhs.m_vCenterUV)
    , m_vHalfUV(rhs.m_vHalfUV)
    , m_iSpriteCount(rhs.m_iSpriteCount)
    , m_iCurrentFrame(rhs.m_iCurrentFrame)
    , m_fDelayTime(rhs.m_fDelayTime)
    , m_fElapsedTime(0.f)
    , m_bTimePause(true)
    , m_bHoldLastFrame(rhs.m_bHoldLastFrame)
    , m_eMode(rhs.m_eMode)
    , m_bUseColSetting(rhs.m_bUseColSetting)
{

}

CAnimation::~CAnimation() 
{

}

HRESULT CAnimation::Ready_Animation()
{
    // 건희 코드 복사. 
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 4;
    m_dwTricnt = 2;
    m_dwFVF = FVF_TEX; 
    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
    {
        MSG_BOX("죄송합니다 애니메이션 Buffer Fail");
        return E_FAIL;
    }

    // 정점 위치 및 초기 UV(풀 사각)
    VTXTEX* v = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&v, 0)))
    {
        MSG_BOX("죄송합니다 애니메이션 Buffer Fail");
        return E_FAIL;
    }

     // 2 * 2

    v[0].vPosition = { -0.5f,  1.f, 0.f };  // 좌상 LT
    v[0].vTexUV = { 0.f, 0.f };

    v[1].vPosition = { 0.5f,  1.f, 0.f };  // 우상 RT
    v[1].vTexUV = { 1.f, 0.f };

    v[2].vPosition = { 0.5f, 0.f, 0.f };  // 우하 RB
    v[2].vTexUV = { 1.f, 1.f };

    v[3].vPosition = { -0.5f, 0.f, 0.f };  // 좌하 LB
    v[3].vTexUV = { 0.f, 1.f };

    m_pVB->Unlock();

    INDEX32* i = nullptr;
    if (FAILED(m_pIB->Lock(0, 0, (void**)&i, 0)))
    {
        MSG_BOX("죄송합니다 애니메이션 Buffer Fail");
        return E_FAIL;
    }

    // 인덱스 설정
    i[0]._0 = 0; i[0]._1 = 1; i[0]._2 = 2;
    i[1]._0 = 0; i[1]._1 = 2; i[1]._2 = 3;
    m_pIB->Unlock();

    // 초기 상태에는 pause를 걸어서 애니메이션 일단 출력 못하게 막자.
    m_bTimePause = true;
    m_iCurrentFrame = 0;
    m_fElapsedTime = 0.f;
    ApplyUVToVB();

    return S_OK;
}

HRESULT CAnimation::AniMation_Init(int spriteCount, float delayTime)
{
    m_iSpriteCount = spriteCount;         // 스프라이트 개수
    m_fDelayTime = delayTime;         // 애니메이션 재생 속도
    m_iCurrentFrame = 0;        // 현재 프레임
    m_fElapsedTime = 0.f;       // 누적 시간 변수
    m_bTimePause = false;
    
    return S_OK;
}

_int CAnimation::Update_Component(const _float& fTimeDelta)
{
    if (m_bTimePause) 
        return 0;

    // 그리드 보호
    int cols = (m_iCols > 0) ? m_iCols : 1;
    int rows = (m_iRows > 0) ? m_iRows : 1;

    // --- 활성 재생 구간 결정 ---
    int startCol, endCol;

    if (m_bUseColSetting) 
    {
        // start
        startCol = (m_iStartCol >= 0) ? m_iStartCol : 0;

        if (startCol >= cols) 
            startCol = cols - 1;

        // end
        if (m_iEndCol >= 0) 
            endCol = m_iEndCol;

        else                
            endCol = cols - 1;

        if (endCol >= cols) 
            endCol = cols - 1;

        if (endCol < startCol) 
            endCol = startCol;

    }
    else {
        startCol = 0;
        if (m_iSpriteCount > 0) endCol = m_iSpriteCount - 1;
        else                    endCol = cols - 1;

        if (endCol < 0) endCol = 0;
        if (endCol >= cols) endCol = cols - 1;
    }

    int activeCount = endCol - startCol + 1;
    if (activeCount < 1) activeCount = 1;

    // --- 타이밍/프레임 진척 ---
    m_fElapsedTime += fTimeDelta;
    if (m_fElapsedTime >= m_fDelayTime) {
        m_fElapsedTime = 0.f;

        if (m_eMode == PlayMode::Loop && !m_bHoldLastFrame) {
            int next = m_iCurrentFrame + 1;
            if (next >= activeCount) next = 0;
            m_iCurrentFrame = next;
        }
        else {
            if (m_iCurrentFrame < activeCount - 1) {
                m_iCurrentFrame += 1;
            }
            else {
                OnReachEnd();
            }
        }
    }

    // --- 현재 프레임 → 실제 열 인덱스 ---
    int localIdx = m_iCurrentFrame;

    if (localIdx < 0) 
        localIdx = 0;

    if (localIdx >= activeCount) 
        localIdx = activeCount - 1;

    int colIndex = startCol + localIdx;

    int safeRow = m_iRow;

    if (safeRow < 0) 
        safeRow = 0;

    if (safeRow >= rows) 
        safeRow = rows - 1;

    // --- UV 계산 ---
    float halfU = 1.f / (2.f * cols);
    float halfV = 1.f / (2.f * rows);
    float centerU = (colIndex + 0.5f) / cols;
    float centerV = (safeRow + 0.5f) / rows;

    m_vHalfUV = { halfU, halfV };
    m_vCenterUV = { centerU, centerV };

    ApplyUVToVB();
    return 0;
}

void CAnimation::LateUpdate_Component(const _float& fTimeDelta) {}

HRESULT CAnimation::Change_Buffer(const MAPOBJECT_PLACEMENT& eType)
{
    if (eType == MAPOBJECT_PLACEMENT::GROUND)
        return S_OK;


    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;

 
    pVertex[BUFFER_LT].vPosition.y = +0.5f;
    pVertex[BUFFER_RT].vPosition.y = +0.5f;
    pVertex[BUFFER_RB].vPosition.y = -0.5f;
    pVertex[BUFFER_LB].vPosition.y = -0.5f;


    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;
    

    return S_OK;
}

void CAnimation::PlayAnimation()
{
    // 외부에서 m_vCenterUV / m_vHalfUV를 맞춰놨다고 가정하고 즉시 반영
    m_bTimePause = false;
    ApplyUVToVB();

    m_bIsAnimationEnd = false;
}

void CAnimation::PauseAnimation() { m_bTimePause = true; }
void CAnimation::ResumeAnimation() { m_bTimePause = false; }
void CAnimation::PauseOnEnd() { m_bHoldLastFrame = true; }

void CAnimation::OnReachEnd()
{
    switch (m_eMode)
    {
    case PlayMode::Loop:
        // m_bHoldLastFrame == true면 Loop보다 우선해서 멈추게 하고 싶다면 아래로
    {
        if (m_bHoldLastFrame)
        {
            m_bTimePause = true;
        }
        else
        {
            m_iCurrentFrame = 0;
        }

        break;
    }

    case PlayMode::Once:
        m_bTimePause = true; // 시간아 잠깐 멈춰라
        m_bIsAnimationEnd = true;
        break;

    case PlayMode::OnceHold:
        m_bTimePause = true; // 시간아 이번에는 계속 멈춰라
        m_bIsAnimationEnd = true;
        break;
    }
}

void CAnimation::ApplyUVToVB()
{
    // 진짜 말도 안된다. 우리 애니메이션 사진이 uv 좌표가 다를 수가 있대 아오

    VTXTEX* v = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&v, 0))) return;

    const float cu = m_vCenterUV.x;
    const float cv = m_vCenterUV.y;
    const float hu = m_vHalfUV.x;
    const float hv = m_vHalfUV.y;

    v[0].vTexUV = { cu - hu, cv - hv };
    v[1].vTexUV = { cu + hu, cv - hv };
    v[2].vTexUV = { cu + hu, cv + hv };
    v[3].vTexUV = { cu - hu, cv + hv };

    m_pVB->Unlock();
}

CComponent* CAnimation::Clone()
{
    return CAnimation::Create(m_pGraphicDev);
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CAnimation* p = new CAnimation(pGraphicDev);
    if (FAILED(p->Ready_Animation()))
    {
        Safe_Release(p);
        return nullptr;
    }
    return p;
}

void CAnimation::Free()
{
    CVIBuffer::Free();
}
