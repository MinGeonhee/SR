#include "CBossEffectAnimation.h"
#include "CVIBuffer.h"

using namespace std;

CBossEffectAnimation::CBossEffectAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
}

CBossEffectAnimation::CBossEffectAnimation(const CBossEffectAnimation& rhs)
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
    , m_bBendMesh(rhs.m_bBendMesh)
    , m_bMakeZBeam(rhs.m_bMakeZBeam)
    , m_fAmplitude(rhs.m_fAmplitude)
    , m_fMaxRad(rhs.m_fMaxRad)
    , m_fZBase(rhs.m_fZBase)
    , m_fZLen(rhs.m_fZLen)
    , m_iCols(rhs.m_iCols)
    , m_iRows(rhs.m_iRows)
    , m_iRow(rhs.m_iRow)
    , m_iStartCol(rhs.m_iStartCol)
    , m_iEndCol(rhs.m_iEndCol)
{
}

CBossEffectAnimation::~CBossEffectAnimation()
{
}

HRESULT CBossEffectAnimation::Ready_Animation()
{
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 28;
    m_dwTricnt = 14;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX* pVertex = nullptr;
    int iRectNum = 0;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    // ===== 7개의 사각면(4정점 x 7) 세로로 쌓기 =====
    // 1
    pVertex[0].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[0].vTexUV = { 0.f, 0.f };
    pVertex[1].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[1].vTexUV = { 1.f, 0.f };
    pVertex[2].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[2].vTexUV = { 1.f, 1.f };
    pVertex[3].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[3].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 2
    pVertex[4].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[4].vTexUV = { 0.f, 0.f };
    pVertex[5].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[5].vTexUV = { 1.f, 0.f };
    pVertex[6].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[6].vTexUV = { 1.f, 1.f };
    pVertex[7].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[7].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 3
    pVertex[8].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[8].vTexUV = { 0.f, 0.f };
    pVertex[9].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[9].vTexUV = { 1.f, 0.f };
    pVertex[10].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[10].vTexUV = { 1.f, 1.f };
    pVertex[11].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[11].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 4
    pVertex[12].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[12].vTexUV = { 0.f, 0.f };
    pVertex[13].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[13].vTexUV = { 1.f, 0.f };
    pVertex[14].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[14].vTexUV = { 1.f, 1.f };
    pVertex[15].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[15].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 5
    pVertex[16].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[16].vTexUV = { 0.f, 0.f };
    pVertex[17].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[17].vTexUV = { 1.f, 0.f };
    pVertex[18].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[18].vTexUV = { 1.f, 1.f };
    pVertex[19].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[19].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 6
    pVertex[20].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[20].vTexUV = { 0.f, 0.f };
    pVertex[21].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[21].vTexUV = { 1.f, 0.f };
    pVertex[22].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[22].vTexUV = { 1.f, 1.f };
    pVertex[23].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[23].vTexUV = { 0.f, 1.f };
    iRectNum += 1;

    // 7
    pVertex[24].vPosition = { -0.5f,  0.5f + iRectNum, 0.f }; pVertex[24].vTexUV = { 0.f, 0.f };
    pVertex[25].vPosition = { 0.5f,  0.5f + iRectNum, 0.f }; pVertex[25].vTexUV = { 1.f, 0.f };
    pVertex[26].vPosition = { 0.5f, -0.5f + iRectNum, 0.f }; pVertex[26].vTexUV = { 1.f, 1.f };
    pVertex[27].vPosition = { -0.5f, -0.5f + iRectNum, 0.f }; pVertex[27].vTexUV = { 0.f, 1.f };

    // ===== 모양 변경(선택) — 기본은 그대로 두어 ‘일직선’ =====
    if (m_bMakeZBeam || m_bBendMesh)
    {
        const int   VERT_COUNT = 28;

        // 세로로 쌓인 정점들의 y 범위를 0~1 진행도로 변환
        const int   startRect = 0;
        const int   numRects = 7;
        const float yMin = -1.f + float(startRect);
        const float yMax = 1.f + float(startRect + numRects - 1);
        const float invDen = 1.f / (yMax - yMin);

        for (int i = 0; i < VERT_COUNT; ++i)
        {
            const float y_old = pVertex[i].vPosition.y;
            float t = (y_old - yMin) * invDen;   // 0~1
            if (t < 0.f) t = 0.f;
            else if (t > 1.f) t = 1.f;

            if (m_bMakeZBeam)
            {
                // Z 방향 직선 빔 (Y 고정)
                pVertex[i].vPosition.y = 0.f;
                pVertex[i].vPosition.z = m_fZBase + m_fZLen * t;
            }
            else // m_bBendMesh == true
            {
                const float theta = m_fMaxRad * t;
                const float y_new = m_fAmplitude * sinf(theta);
                const float z_new = m_fZBase + m_fZLen * t;

                pVertex[i].vPosition.y = y_new;
                pVertex[i].vPosition.z = z_new;
            }
        }
    }

    m_pVB->Unlock();

    // ===== 인덱스 버퍼 =====
    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    pIndex[0] = { 0, 1, 2 };  pIndex[1] = { 0, 2, 3 };
    pIndex[2] = { 4, 5, 6 };  pIndex[3] = { 4, 6, 7 };
    pIndex[4] = { 8, 9,10 };  pIndex[5] = { 8,10,11 };
    pIndex[6] = { 12,13,14 };  pIndex[7] = { 12,14,15 };
    pIndex[8] = { 16,17,18 };  pIndex[9] = { 16,18,19 };
    pIndex[10] = { 20,21,22 };  pIndex[11] = { 20,22,23 };
    pIndex[12] = { 24,25,26 };  pIndex[13] = { 24,26,27 };

    m_pIB->Unlock();

    // 초기 상태
    m_bTimePause = true;
    m_iCurrentFrame = 0;
    m_fElapsedTime = 0.f;
    ApplyUVToVB();

    return S_OK;
}

void CBossEffectAnimation::RebuildShape()
{
    if (!m_pVB) return;

    VTXTEX* v = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&v, 0))) return;

    // 정점은 7개의 사각면(각 4정점) = 28개
    const int VERT_COUNT = 28;
    const int numRects = 7;
    const float yMin = -1.f + 0;                     // startRect = 0
    const float yMax = 1.f + float(numRects - 1);
    const float invDen = 1.f / (yMax - yMin);

    for (int i = 0; i < VERT_COUNT; ++i)
    {
        const int k = i / 4;         // 몇 번째 사각면(0~6)
        const int c = i % 4;         // 사각 내 코너(0:LT,1:RT,2:RB,3:LB)

        // 원본 기준 X, Y 복원
        float x = (c == 0 || c == 3) ? -0.5f : 0.5f;
        float y = (c == 0 || c == 1) ? 0.5f : -0.5f;
        y += float(k);

        // 진행도 t(0~1)
        float t = (y - yMin) * invDen;
        if (t < 0.f) t = 0.f; else if (t > 1.f) t = 1.f;

        float outY = y;
        float outZ = 0.f;

        if (m_bMakeZBeam)
        {
            // Z로 곧게(물줄기), Y는 평면 고정
            outY = 0.f;
            outZ = m_fZBase + m_fZLen * t;
        }
        else if (m_bBendMesh)
        {
            const float theta = m_fMaxRad * t;
            outY = m_fAmplitude * sinf(theta);
            outZ = m_fZBase + m_fZLen * t;
        }

        v[i].vPosition.x = x;
        v[i].vPosition.y = outY;
        v[i].vPosition.z = outZ;
        // UV는 기존 ApplyUVToVB에서 관리
    }

    m_pVB->Unlock();
}

HRESULT CBossEffectAnimation::AniMation_Init(int spriteCount, float delayTime)
{
    m_iSpriteCount = spriteCount;
    m_fDelayTime = delayTime;
    m_iCurrentFrame = 0;
    m_fElapsedTime = 0.f;
    m_bTimePause = false;
    return S_OK;
}

_int CBossEffectAnimation::Update_Component(const _float& fTimeDelta)
{
    if (m_bTimePause)
        return 0;

    int cols = (m_iCols > 0) ? m_iCols : 1;
    int rows = (m_iRows > 0) ? m_iRows : 1;

    int startCol, endCol;
    if (m_bUseColSetting)
    {
        startCol = (m_iStartCol >= 0) ? m_iStartCol : 0;
        if (startCol >= cols) startCol = cols - 1;

        if (m_iEndCol >= 0) endCol = m_iEndCol;
        else                endCol = cols - 1;

        if (endCol >= cols) endCol = cols - 1;
        if (endCol < startCol) endCol = startCol;
    }
    else
    {
        startCol = 0;
        endCol = (m_iSpriteCount > 0) ? (m_iSpriteCount - 1) : (cols - 1);
        if (endCol < 0) endCol = 0;
        if (endCol >= cols) endCol = cols - 1;
    }

    int activeCount = endCol - startCol + 1;
    if (activeCount < 1) activeCount = 1;

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

    int localIdx = m_iCurrentFrame;
    if (localIdx < 0) localIdx = 0;
    if (localIdx >= activeCount) localIdx = activeCount - 1;

    int colIndex = startCol + localIdx;

    int safeRow = m_iRow;
    if (safeRow < 0) safeRow = 0;
    if (safeRow >= rows) safeRow = rows - 1;

    float halfU = 1.f / (2.f * cols);
    float halfV = 1.f / (2.f * rows);
    float centerU = (colIndex + 0.5f) / cols;
    float centerV = (safeRow + 0.5f) / rows;

    m_vHalfUV = { halfU, halfV };
    m_vCenterUV = { centerU, centerV };

    ApplyUVToVB();
    return 0;
}

void CBossEffectAnimation::LateUpdate_Component(const _float& fTimeDelta) {}

void CBossEffectAnimation::PlayAnimation()
{
    m_bTimePause = false;
    ApplyUVToVB();
    m_bIsAnimationEnd = false;
}
void CBossEffectAnimation::PauseAnimation() { m_bTimePause = true; }
void CBossEffectAnimation::ResumeAnimation() { m_bTimePause = false; }
void CBossEffectAnimation::PauseOnEnd() { m_bHoldLastFrame = true; }

void CBossEffectAnimation::OnReachEnd()
{
    switch (m_eMode)
    {
    case PlayMode::Loop:
        if (m_bHoldLastFrame) {
            m_bTimePause = true;
        }
        else {
            m_iCurrentFrame = 0;
        }
        break;

    case PlayMode::Once:
        m_bTimePause = true;
        m_bIsAnimationEnd = true;
        break;

    case PlayMode::OnceHold:
        m_bTimePause = true;
        m_bIsAnimationEnd = true;
        break;
    }
}

void CBossEffectAnimation::ApplyUVToVB()
{
    VTXTEX* v = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&v, 0))) return;

    const float cu = m_vCenterUV.x;
    const float cv = m_vCenterUV.y;
    const float hu = m_vHalfUV.x;
    const float hv = m_vHalfUV.y;

    for (int i = 0; i < 7; i++)
    {
        v[(4 * i) + 0].vTexUV = { cu - hu, cv - hv };
        v[(4 * i) + 1].vTexUV = { cu + hu, cv - hv };
        v[(4 * i) + 2].vTexUV = { cu + hu, cv + hv };
        v[(4 * i) + 3].vTexUV = { cu - hu, cv + hv };
    }

    m_pVB->Unlock();
}

CComponent* CBossEffectAnimation::Clone()
{
    return CBossEffectAnimation::Create(m_pGraphicDev);
}

CBossEffectAnimation* CBossEffectAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossEffectAnimation* p = new CBossEffectAnimation(pGraphicDev);
    if (FAILED(p->Ready_Animation()))
    {
        Safe_Release(p);
        return nullptr;
    }
    return p;
}

void CBossEffectAnimation::Free()
{
    CVIBuffer::Free();
}
