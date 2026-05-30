#include "CRectCol.h"

CRectCol::CRectCol()
{
	m_pMyOwner = nullptr;
}

CRectCol::CRectCol(LPDIRECT3DDEVICE9 mGraphicDev)
	:CVIBuffer(mGraphicDev)
{
	m_pMyOwner = nullptr;
}

// 자식을 복사하면 부모도 같이 복사가 됨.
// 이때 부모 객체에게 자식 객체를 넘겨주면, 내 안에 있는 부모 부분(CVIBuffer)을 네가 복사해줘 라는 뜻이 된다.
// VIBuffer(1) <- TriCol(1) 형태라고 한다면
// VIBuffer(2) <- TriCol(2) 이런식으로 되는 것.

CRectCol::CRectCol(const CRectCol& rhs)
	:CVIBuffer(rhs)
{
	m_pMyOwner = nullptr;
}

CRectCol::~CRectCol()
{
	m_pMyOwner = nullptr;
}

HRESULT CRectCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 6;
	m_dwTricnt = 2;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer())) // 버퍼를 초기 설정해두는 함수.
		return E_FAIL;

	VTXCOL* pVertex = NULL;

	// 3인자 : 버텍스 버퍼에 저장된 정점들 중 첫 번째 정점의 주소를 얻어옴
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	Make_Vertex(pVertex);
	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, m_IdxFmt);
	Make_Index(pIndex);
	m_pIB->Unlock();

	return S_OK;
}

void CRectCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CRectCol::Change_Color(const _vec4& Color)
{
	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0,0,(void**)&pVertex,0);

	for (int i = 0; i < 4; ++i)
	{
		pVertex[i].dwColor = D3DCOLOR_RGBA((int)Color.x, (int)Color.y, (int)Color.z, (int)Color.w);
	}

	m_pVB->Unlock();

}

CComponent* CRectCol::Clone()
{
	CRectCol* pTriCol = new CRectCol(m_pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("CRectCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

CRectCol* CRectCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRectCol* pTriCol = new CRectCol(pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("CRectCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

void CRectCol::Free()
{
	CVIBuffer::Free();
}

void CRectCol::Make_Vertex(VTXCOL* pVerTex)
{

		pVerTex[0].vPosition = { -0.5f, 0.f, 0.5f };
		pVerTex[1].vPosition = { 0.5f, 0.f, +0.5f };
		pVerTex[2].vPosition = { 0.5f, 0.f, -0.5f };
		pVerTex[3].vPosition = { -0.5f, 0.f, -0.5f };

		pVerTex[0].dwColor = D3DXCOLOR{ 1.f, 0.f, 1.f, 0.5f }; // ARGB 색상
		pVerTex[1].dwColor = D3DXCOLOR{ 1.f, 1.f, 0.f, 0.5f };
		pVerTex[2].dwColor = D3DXCOLOR{ 1.f, 1.f, 1.f, 0.5f };

		pVerTex[3].dwColor = D3DXCOLOR{ 1.f, 0.f, 1.f, 1.f }; // ARGB 색상

}

void CRectCol::Make_Index(INDEX32* pIndex)
{
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;
}