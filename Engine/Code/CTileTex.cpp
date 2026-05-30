#include "CTileTex.h"

CTileTex::CTileTex()
    : CVIBuffer()
{
}

CTileTex::CTileTex(LPDIRECT3DDEVICE9 device)
    :CVIBuffer(device)
{
    ZeroMemory(&m_vCenterUV, sizeof(_vec2));
}

CTileTex::CTileTex(const CTileTex& cpy)
    : CVIBuffer(cpy)
{
}

CTileTex::~CTileTex()
{
}

HRESULT CTileTex::Ready_Buffer(const _vec2& vCenterUV)
{
    m_vCenterUV = vCenterUV;
    _float Offset = 0.015625;

    m_dwVtxSize = sizeof(VTXNORMALTEX);
    m_dwVtxCnt = 4;
    m_dwTricnt = 2;
    m_dwFVF = FVF_NORMALTEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXNORMALTEX* pVertex = NULL;

    // 3인자 : 버텍스 버퍼에 저장된 정점들 중 첫 번째 정점의 주소를 얻어옴
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    // 오른쪽 위
    pVertex[0].vPosition = { - 0.5f, 0.f, 0.5f };
    pVertex[0].vNormal = { 0.f, 1.f, 0.f };
    pVertex[0].vTexUV = { m_vCenterUV.x - Offset  ,  m_vCenterUV.y - Offset }; // 가로 세로

    pVertex[1].vPosition = { 0.5f, 0.f, +0.5f };
    pVertex[1].vNormal = { 0.f, 1.f, 0.f };
    pVertex[1].vTexUV = { m_vCenterUV.x + Offset  ,  m_vCenterUV.y - Offset };

    pVertex[2].vPosition = { 0.5f, 0.f, -0.5f };
    pVertex[2].vNormal = { 0.f, 1.f, 0.f };
    pVertex[2].vTexUV = { m_vCenterUV.x + Offset  ,  m_vCenterUV.y + Offset };

    pVertex[3].vPosition = { -0.5f, 0.f, -0.5f };
    pVertex[3].vNormal = { 0.f, 1.f, 0.f };
    pVertex[3].vTexUV = { m_vCenterUV.x - Offset  ,  m_vCenterUV.y + Offset };

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

void CTileTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}
HRESULT CTileTex::Change_CenterUV(const _vec2 vCenterUV)
{
    VTXNORMALTEX* pVertex = nullptr;          // ← 여기!
    m_vCenterUV = vCenterUV;
    _float Offset = 0.015625f;

    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0))) return E_FAIL;

    pVertex[0].vTexUV = { vCenterUV.x - Offset, vCenterUV.y - Offset };
    pVertex[1].vTexUV = { vCenterUV.x + Offset, vCenterUV.y - Offset };
    pVertex[2].vTexUV = { vCenterUV.x + Offset, vCenterUV.y + Offset };
    pVertex[3].vTexUV = { vCenterUV.x - Offset, vCenterUV.y + Offset };

    m_pVB->Unlock();
    return S_OK;
}

HRESULT CTileTex::Change_IndexCCW()
{
    INDEX32* pIndex = NULL;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 오른쪽 위
    pIndex[0]._0 = 0;
    pIndex[0]._1 = 2;
    pIndex[0]._2 = 1;

    // 왼쪽 아래
    pIndex[1]._0 = 0;
    pIndex[1]._1 = 3;
    pIndex[1]._2 = 2;

    m_pIB->Unlock();


    return S_OK;
}


CTileTex* CTileTex::Create(LPDIRECT3DDEVICE9 device, const _vec2& vCenterUV)
{
    CTileTex* newTileTex = new CTileTex(device);
    
    if (FAILED(newTileTex->Ready_Buffer(vCenterUV)))
    {
        MSG_BOX("CTileTex Create Is Failed");
        Safe_Release(newTileTex);
        return nullptr;
    }

    return newTileTex;
}

CComponent* CTileTex::Clone()
{
    CTileTex* newTile = CTileTex::Create(m_pGraphicDev, _vec2({DEFUALT_TILE_UV_CENTERX, DEFUALT_TILE_UV_CENTERY}));
    
    if (newTile == nullptr)
    {
        MSG_BOX("NewTile Clone Is FAILED");
        Safe_Release(newTile);
        return nullptr;
    }

    return newTile;
}

void CTileTex::Free()
{
    CVIBuffer::Free();
}
