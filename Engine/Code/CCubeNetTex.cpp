#include "CCubeNetTex.h"
#include "Geometry.h"
#include "GeometryHelper.h"


CCubeNetTex::CCubeNetTex()
{

}

CCubeNetTex::CCubeNetTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
{

}

CCubeNetTex::CCubeNetTex(const CCubeNetTex& rhs)
    :CVIBuffer(rhs)
{

}

CCubeNetTex::~CCubeNetTex()
{

}

HRESULT CCubeNetTex::Ready_Buffer()
{
    // <사용시 유의 ! ! !>
    // 이놈은 전개도 큐브이다．
    // @@@ 6면이 전부 같은 텍스처로 나온다 @@@

    // 지오매트리 클래스를 통해 Vertex 정보를 다 계산하고 가져옴.
    Geometry<VTXTEX>* _Geometry = new Geometry<VTXTEX>;
    GeometryHelper::CreateCubeNet(_Geometry);

    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = _Geometry->GetVertexCount();
    m_dwTricnt = _Geometry->GetIndexCount() / 3; // 인덱스 갯수의 /3 만큼이 삼각형 
    m_dwFVF = FVF_TEX;      // 강사님꺼 그대로. Normal 정보 값 없는 Tex

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;;

    // 버텍스 버퍼 pVertex에 우리가 GeoMetry함수에 저장해둔 Vertex 정보를
    // memcpy를 통해서 전부 복사해서 들고온다. 
    VTXTEX* pVertex = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);
    memcpy(pVertex, _Geometry->GetVertexData(), m_dwVtxCnt * m_dwVtxSize);
    m_pVB->Unlock();

    // 인덱스 버퍼 pIndex에 우리가 GeoMetry함수에 저장해둔 Index 정보를
  // memcpy를 통해서 전부 복사해서 들고온다. 
    INDEX32* pIndex = nullptr;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);
    const vector<_uint32>& indices = _Geometry->GetIndices();
    for (_uint32 i = 0; i < m_dwTricnt; ++i)
    {
        pIndex[i]._0 = indices[i * 3 + 0];
        pIndex[i]._1 = indices[i * 3 + 1];
        pIndex[i]._2 = indices[i * 3 + 2];
    }
    m_pIB->Unlock();

    Safe_Delete(_Geometry); // 쓰고나면 바로 삭제.

    return S_OK;
}

void CCubeNetTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CComponent* CCubeNetTex::Clone()
{
    return new CCubeNetTex(*this);
}

CCubeNetTex* CCubeNetTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCubeNetTex* pCubeTex = new CCubeNetTex(pGraphicDev);

    if (FAILED(pCubeTex->Ready_Buffer()))
    {
        Safe_Release(pCubeTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pCubeTex;
}

void CCubeNetTex::Free()
{
    CVIBuffer::Free();
}
