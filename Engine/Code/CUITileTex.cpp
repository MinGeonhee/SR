#include "CUITileTex.h"

CUITileTex::CUITileTex()
    : CVIBuffer(nullptr)
{
}

CUITileTex::CUITileTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CVIBuffer(pGraphicDev)
{
}

CUITileTex::CUITileTex(const CUITileTex& rhs)
    : CVIBuffer(rhs)
    , m_fWidth(rhs.m_fWidth)
    , m_fHeight(rhs.m_fHeight)
{
}

CUITileTex::~CUITileTex()
{
}

HRESULT CUITileTex::Ready_Buffer()
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

void CUITileTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CComponent* CUITileTex::Clone()
{
    return new CUITileTex(*this);
}

CUITileTex* CUITileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUITileTex* pUITileTex = new CUITileTex(pGraphicDev);

    if (FAILED(pUITileTex->Ready_Buffer()))
    {
        Safe_Release(pUITileTex);
        MSG_BOX("pUITileTex Create Failed");
        return nullptr;
    }

    return pUITileTex;
}

void CUITileTex::Free()
{
    CVIBuffer::Free();
}

HRESULT CUITileTex::Change_UV(int idxX, int countX, int idxY, int countY)
{
    if (countX <= 0 || countY <= 0)
        return E_FAIL;

    if (idxX < 0) 
        idxX = 0;
    if (idxX >= countX) 
        idxX = countX - 1;
    if (idxY < 0) 
        idxY = 0;
    if (idxY >= countY) 
        idxY = countY - 1;

    const float invX = 1.0f / static_cast<float>(countX);
    const float invY = 1.0f / static_cast<float>(countY);

    VTXTEX* pVertex = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;

    pVertex[0].vTexUV = { idxX * invX, idxY * invY };
    pVertex[1].vTexUV = { (idxX + 1) * invX, idxY * invY };
    pVertex[2].vTexUV = { (idxX + 1) * invX, (idxY + 1) * invY };
    pVertex[3].vTexUV = { idxX * invX, (idxY + 1) * invY };

    m_pVB->Unlock();
    return S_OK;
}
