#include "CMeshAnimation.h"
#include "CVIBuffer.h"

CMeshAnimation::CMeshAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{

}

CMeshAnimation::CMeshAnimation(const CMeshAnimation& rhs)
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

CMeshAnimation::~CMeshAnimation() 
{

}

HRESULT CMeshAnimation::Ready_Animation()
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


    //첫번째 면
    //LT
    pVertex[0].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[0].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[1].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[1].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[2].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[2].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[3].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[3].vTexUV = { 0.f, 1.f };

    iRectNum += 1;

    // 두번째 면
    //LT
    pVertex[4].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[4].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[5].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[5].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[6].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[6].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[7].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[7].vTexUV = { 0.f, 1.f };

    iRectNum += 1;

    // 세번째 면
    //LT
    pVertex[8].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[8].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[9].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[9].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[10].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[10].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[11].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[11].vTexUV = { 0.f, 1.f };

    iRectNum += 1;

    // 네번째 면
    //LT
    pVertex[12].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[12].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[13].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[13].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[14].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[14].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[15].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[15].vTexUV = { 0.f, 1.f };

    iRectNum += 1;


    // 다섯번째 면
    //LT
    pVertex[16].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[16].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[17].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[17].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[18].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[18].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[19].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[19].vTexUV = { 0.f, 1.f };

    iRectNum += 1;

    // 여섯번째 면
    //LT
    pVertex[20].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[20].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[21].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[21].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[22].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[22].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[23].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[23].vTexUV = { 0.f, 1.f };

    iRectNum += 1;


    // 일곱번째 면
    //LT
    pVertex[24].vPosition = { -0.5f, 0.5f + iRectNum, 0.f };
    pVertex[24].vTexUV = { 0.f, 0.f }; // 가로 세로

    //RT
    pVertex[25].vPosition = { 0.5f, 0.5f + iRectNum, 0.f };
    pVertex[25].vTexUV = { 1.f, 0.f };

    //RB
    pVertex[26].vPosition = { 0.5f, -0.5f + iRectNum, 0.f };
    pVertex[26].vTexUV = { 1.f, 1.f };

    //LB
    pVertex[27].vPosition = { -0.5f, -0.5f + iRectNum, 0.f };
    pVertex[27].vTexUV = { 0.f, 1.f };

    // YZ 평면을 기준으로 사인 그래프 모양 그리게
    // ---- 고정 파라미터 ----
    const float A = 3.f;                     // Y 진폭 (끝점에서 Y=0, 중간에서 A)
    const float PI = 3.1415926535f;
    const float MAX_RAD = PI * 4.9f / 6.f;                 // 0~180° (원하면 150°=PI*5/6)
    const float zBase = 0.f;                  // 시작 Z
    const float zLen = 6.f;                  // Z로 진행할 총 길이
    const int   VERT_COUNT = 28;

    // 원래 메시가 yMin~yMax로 세로로 쌓였다고 가정하고,
    // 그 y를 "진행도 t(0~1)"로만 쓰고, 실제 좌표는 (y: 사인, z: 선형)로 재배치합니다.
    const int   startRect = 0;
    const int   numRects = 7;
    const float yMin = -1.f + float(startRect);
    const float yMax = 1.f + float(startRect + numRects - 1);
    const float invDen = 1.f / (yMax - yMin);

    // t = (y - yMin)/(yMax - yMin) -> 0~1
    for (int i = 0; i < VERT_COUNT; ++i)
    {
        const float y_old = pVertex[i].vPosition.y;
        float t = (y_old - yMin) * invDen;          // 0~1
        if (t < 0.f) t = 0.f; if (t > 1.f) t = 1.f;

        const float theta = MAX_RAD * t;            // 0 → 180°(또는 150°)
        const float y_new = A * sinf(theta);
        const float z_new = zBase + zLen * t;       // Z는 선형 진행

        pVertex[i].vPosition.y = y_new;             // > Y가 사인: 시작/끝에서 0
        pVertex[i].vPosition.z = z_new;             // > Z는 0→zLen로 증가

    }




    m_pVB->Unlock();




    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 1
    // 오른쪽 위
    pIndex[0]._0 = 0;
    pIndex[0]._1 = 1;
    pIndex[0]._2 = 2;

    // 왼쪽 아래
    pIndex[1]._0 = 0;
    pIndex[1]._1 = 2;
    pIndex[1]._2 = 3;

    // 2
    // 오른쪽 위
    pIndex[2]._0 = 4;
    pIndex[2]._1 = 5;
    pIndex[2]._2 = 6;

    // 왼쪽 아래
    pIndex[3]._0 = 4;
    pIndex[3]._1 = 6;
    pIndex[3]._2 = 7;

    // 3
    // 오른쪽 위
    pIndex[4]._0 = 8;
    pIndex[4]._1 = 9;
    pIndex[4]._2 = 10;

    // 왼쪽 아래
    pIndex[5]._0 = 8;
    pIndex[5]._1 = 10;
    pIndex[5]._2 = 11;

    // 4
    // 오른쪽 위
    pIndex[6]._0 = 12;
    pIndex[6]._1 = 13;
    pIndex[6]._2 = 14;

    // 왼쪽 아래
    pIndex[7]._0 = 12;
    pIndex[7]._1 = 14;
    pIndex[7]._2 = 15;

    // 5
    // 오른쪽 위
    pIndex[8]._0 = 16;
    pIndex[8]._1 = 17;
    pIndex[8]._2 = 18;

    // 왼쪽 아래
    pIndex[9]._0 = 16;
    pIndex[9]._1 = 18;
    pIndex[9]._2 = 19;

    // 6
    // 오른쪽 위
    pIndex[10]._0 = 20;
    pIndex[10]._1 = 21;
    pIndex[10]._2 = 22;

    // 왼쪽 아래
    pIndex[11]._0 = 20;
    pIndex[11]._1 = 22;
    pIndex[11]._2 = 23;


    // 7
    // 오른쪽 위
    pIndex[12]._0 = 24;
    pIndex[12]._1 = 25;
    pIndex[12]._2 = 26;

    // 왼쪽 아래
    pIndex[13]._0 = 24;
    pIndex[13]._1 = 26;
    pIndex[13]._2 = 27;


    m_pIB->Unlock();

    // 초기 상태에는 pause를 걸어서 애니메이션 일단 출력 못하게 막자.
    m_bTimePause = true;
    m_iCurrentFrame = 0;
    m_fElapsedTime = 0.f;
    ApplyUVToVB();

    return S_OK;
}

HRESULT CMeshAnimation::AniMation_Init(int spriteCount, float delayTime)
{
    m_iSpriteCount = spriteCount;         // 스프라이트 개수
    m_fDelayTime = delayTime;         // 애니메이션 재생 속도
    m_iCurrentFrame = 0;        // 현재 프레임
    m_fElapsedTime = 0.f;       // 누적 시간 변수
    m_bTimePause = false;
    
    return S_OK;
}

_int CMeshAnimation::Update_Component(const _float& fTimeDelta)
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

void CMeshAnimation::LateUpdate_Component(const _float& fTimeDelta) {}

void CMeshAnimation::PlayAnimation()
{
    // 외부에서 m_vCenterUV / m_vHalfUV를 맞춰놨다고 가정하고 즉시 반영
    m_bTimePause = false;
    ApplyUVToVB();

    m_bIsAnimationEnd = false;
}

void CMeshAnimation::PauseAnimation() { m_bTimePause = true; }
void CMeshAnimation::ResumeAnimation() { m_bTimePause = false; }
void CMeshAnimation::PauseOnEnd() { m_bHoldLastFrame = true; }

void CMeshAnimation::OnReachEnd()
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

void CMeshAnimation::ApplyUVToVB()
{
    // 진짜 말도 안된다. 우리 애니메이션 사진이 uv 좌표가 다를 수가 있대 아오

    VTXTEX* v = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&v, 0))) return;

    const float cu = m_vCenterUV.x;
    const float cv = m_vCenterUV.y;
    const float hu = m_vHalfUV.x;
    const float hv = m_vHalfUV.y;

    for (int i = 0; i < 7; i++)
    {
        v[(4*i)+0].vTexUV = { cu - hu, cv - hv };
        v[(4*i)+1].vTexUV = { cu + hu, cv - hv };
        v[(4*i)+2].vTexUV = { cu + hu, cv + hv };
        v[(4*i)+3].vTexUV = { cu - hu, cv + hv };
    }

    

    m_pVB->Unlock();
}

CComponent* CMeshAnimation::Clone()
{
    return CMeshAnimation::Create(m_pGraphicDev);
}

CMeshAnimation* CMeshAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMeshAnimation* p = new CMeshAnimation(pGraphicDev);
    if (FAILED(p->Ready_Animation()))
    {
        Safe_Release(p);
        return nullptr;
    }
    return p;
}

void CMeshAnimation::Free()
{
    CVIBuffer::Free();
}
