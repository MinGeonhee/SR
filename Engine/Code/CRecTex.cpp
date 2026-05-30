#include "CRecTex.h"

CRecTex::CRecTex()
{

}

CRecTex::CRecTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{

}

CRecTex::CRecTex(const CRecTex& rhs)
	:CVIBuffer(rhs)
{

}

CRecTex::~CRecTex()
{

}

HRESULT CRecTex::Ready_Buffer()
{
    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 4;
    m_dwTricnt = 2;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX* pVertex = NULL;

    // 3인자 : 버텍스 버퍼에 저장된 정점들 중 첫 번째 정점의 주소를 얻어옴
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    // 오른쪽 위
    pVertex[0].vPosition = { -1.f, 1.f, 0.f };
    pVertex[0].vTexUV = { 0.f, 0.f }; // 가로 세로

    pVertex[1].vPosition = { 1.f, 1.f, 0.f };
    pVertex[1].vTexUV = { 1.f, 0.f };

    pVertex[2].vPosition = { 1.f, -1.f, 0.f };
    pVertex[2].vTexUV = { 1.f, 1.f };

    pVertex[3].vPosition = { -1.f, -1.f, 0.f };
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

void CRecTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CComponent* CRecTex::Clone()
{
    return CRecTex::Create(m_pGraphicDev);
}

CRecTex* CRecTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRecTex* pRcTex = new CRecTex(pGraphicDev);

    if (FAILED(pRcTex->Ready_Buffer()))
    {
        Safe_Release(pRcTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pRcTex;
}

void CRecTex::Free()
{
    CVIBuffer::Free();
}
