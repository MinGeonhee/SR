#include "CUIRecTex.h"

CUIRecTex::CUIRecTex()
    : CVIBuffer(nullptr)
{
}

CUIRecTex::CUIRecTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
}

CUIRecTex::CUIRecTex(const CUIRecTex& rhs)
    : CVIBuffer(rhs)
    , m_fWidth(rhs.m_fWidth)
    , m_fHeight(rhs.m_fHeight)
{
}

CUIRecTex::~CUIRecTex()
{
}

HRESULT CUIRecTex::Ready_Buffer()
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

    float left = -m_fWidth * 0.5f;
    float right = m_fWidth * 0.5f;
    float top = m_fHeight * 0.5f;
    float bottom = -m_fHeight * 0.5f;

    pVertex[0].vPosition = { left, top, 0.f };
    pVertex[0].vTexUV = { 0.f, 0.f };

    pVertex[1].vPosition = { right, top, 0.f };
    pVertex[1].vTexUV = { 1.f, 0.f };

    pVertex[2].vPosition = { right, bottom, 0.f };
    pVertex[2].vTexUV = { 1.f, 1.f };

    pVertex[3].vPosition = { left, bottom, 0.f };
    pVertex[3].vTexUV = { 0.f, 1.f };

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

void CUIRecTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CComponent* CUIRecTex::Clone()
{
    return new CUIRecTex(*this);
}

CUIRecTex* CUIRecTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUIRecTex* pUIRcTex = new CUIRecTex(pGraphicDev);

    if (FAILED(pUIRcTex->Ready_Buffer()))
    {
        Safe_Release(pUIRcTex);
        MSG_BOX("pUIRcTex Create Failed");
        return nullptr;
    }

    return pUIRcTex;
}

void CUIRecTex::Free()
{
    CVIBuffer::Free();
}

