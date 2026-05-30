#include "CParticleOnFloorRecTex.h"

CParticleOnFloorRecTex::CParticleOnFloorRecTex()
    : CVIBuffer(nullptr)
{
}

CParticleOnFloorRecTex::CParticleOnFloorRecTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
    m_vUVCenterIndex = { 0.f, 0.f };
    m_vUVCenter = { 0.125f ,0.125f };
}

CParticleOnFloorRecTex::CParticleOnFloorRecTex(const CParticleOnFloorRecTex& rhs)
    : CVIBuffer(rhs), m_vUVCenter(rhs.m_vUVCenter)
{
}

CParticleOnFloorRecTex::~CParticleOnFloorRecTex()
{
}

HRESULT CParticleOnFloorRecTex::Ready_Buffer()
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

    pVertex[0].vPosition = { left, 0.f, top };
    pVertex[0].vTexUV = { m_vUVCenter.x - m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset }; // 가로 세로

    pVertex[1].vPosition = { right, 0.f, top };
    pVertex[1].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y - m_fUVOffset };

    pVertex[2].vPosition = { right, 0.f, bottom };
    pVertex[2].vTexUV = { m_vUVCenter.x + m_fUVOffset  ,  m_vUVCenter.y + m_fUVOffset };

    pVertex[3].vPosition = { left, 0.f, bottom };
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

void CParticleOnFloorRecTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

HRESULT CParticleOnFloorRecTex::Change_CenterUV(const _vec2 vCenterIndex)
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

CComponent* CParticleOnFloorRecTex::Clone()
{
    return new CParticleOnFloorRecTex(*this);
}

CParticleOnFloorRecTex* CParticleOnFloorRecTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CParticleOnFloorRecTex* pUIRcTex = new CParticleOnFloorRecTex(pGraphicDev);

    if (FAILED(pUIRcTex->Ready_Buffer()))
    {
        Safe_Release(pUIRcTex);
        MSG_BOX("pUIRcTex Create Failed");
        return nullptr;
    }

    return pUIRcTex;
}

void CParticleOnFloorRecTex::Free()
{
    CVIBuffer::Free();
}
