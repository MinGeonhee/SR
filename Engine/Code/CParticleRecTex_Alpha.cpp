#include "CParticleRecTex_Alpha.h"

CParticleRecTex_Alpha::CParticleRecTex_Alpha()
    : CVIBuffer(nullptr)
{
}

CParticleRecTex_Alpha::CParticleRecTex_Alpha(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
    m_vUVCenterIndex = { 0.f, 0.f };
    m_vUVCenter = { 0.03125f ,0.03125f };
}

CParticleRecTex_Alpha::CParticleRecTex_Alpha(const CParticleRecTex_Alpha& rhs)
    : CVIBuffer(rhs), m_vUVCenter(rhs.m_vUVCenter)
{
}

CParticleRecTex_Alpha::~CParticleRecTex_Alpha()
{
}

HRESULT CParticleRecTex_Alpha::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 4;
    m_dwTricnt = 2;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX* pVertex = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    float left = -0.5f;
    float right = 0.5f;
    float top = 0.5f;
    float bottom = -0.5f;

    pVertex[0].vPosition = { left, top, 0.f };
    pVertex[0].vTexUV = { m_vUVCenter.x - m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset }; // 가로 세로

    pVertex[1].vPosition = { right, top, 0.f };
    pVertex[1].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset };

    pVertex[2].vPosition = { right, bottom, 0.f };
    pVertex[2].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y + m_fUVOffset };

    pVertex[3].vPosition = { left, bottom, 0.f };
    pVertex[3].vTexUV = { m_vUVCenter.x - m_fUVOffset  ,  m_vUVCenter.y + m_fUVOffset };

    m_pVB->Unlock();

    INDEX32* pIndex = NULL;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 오른쪽 위
    pIndex[0]._0 = 0;
    pIndex[0]._1 = 1;
    pIndex[0]._2 = 2;

    // 왼쪽 아래
    pIndex[1]._0 = 0;
    pIndex[1]._1 = 2;
    pIndex[1]._2 = 3;

    m_pIB->Unlock();


    return S_OK;
}

void CParticleRecTex_Alpha::Render_Buffer()
{

    // 2) 색상 = 텍스처 RGB × TFACTOR RGB  (색 틴트 적용)
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    // 3) 알파 = 텍스처 A × TFACTOR A  (페이드/투명도 제어)
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    // 4) TFACTOR의 A에 현재 m_fAlpha 반영 (RGB는 1,1,1로 유지: 색상 틴트 없음)
    D3DCOLOR tf = D3DCOLOR_COLORVALUE(m_fRed, m_fGreen, m_fBlue, m_fAlpha);
    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tf);

    // 사실상 이 한 줄로 페이드가 적용됩니다.
    CVIBuffer::Render_Buffer();

    // 필요시 상태 원복(프로젝트의 렌더 파이프라인 규칙에 맞춰 처리)
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

HRESULT CParticleRecTex_Alpha::Change_CenterUV(const _vec2 vCenterIndex)
{
    m_vUVCenterIndex = vCenterIndex;
    m_vUVCenter = m_vUVCenterIndex * (m_fUVOffset * 2) + _vec2{ m_fUVOffset,m_fUVOffset };

    VTXTEX* pVertex = nullptr;

    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;

    pVertex[0].vTexUV = { m_vUVCenter.x - m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset }; // 가로 세로
    pVertex[1].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset };
    pVertex[2].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y + m_fUVOffset };
    pVertex[3].vTexUV = { m_vUVCenter.x - m_fUVOffset  ,  m_vUVCenter.y + m_fUVOffset };


    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;

    return S_OK;
}

CComponent* CParticleRecTex_Alpha::Clone()
{
    return new CParticleRecTex_Alpha(*this);
}

CParticleRecTex_Alpha* CParticleRecTex_Alpha::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleRecTex_Alpha* pUIRcTex = new CParticleRecTex_Alpha(pGraphicDev);

    if (FAILED(pUIRcTex->Ready_Buffer()))
    {
        Safe_Release(pUIRcTex);
        MSG_BOX("pUIRcTex Create Failed");
        return nullptr;
    }

    return pUIRcTex;
}

void CParticleRecTex_Alpha::Free()
{
    CVIBuffer::Free();
}
