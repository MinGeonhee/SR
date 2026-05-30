#include "CTriCol.h"

CTriCol::CTriCol() 
{
	m_pMyOwner = nullptr;
}

CTriCol::CTriCol(LPDIRECT3DDEVICE9 mGraphicDev, TRIANGLETYPE _type)
	:CVIBuffer(mGraphicDev), m_eType(_type)
{
	m_pMyOwner = nullptr;
}

// 자식을 복사하면 부모도 같이 복사가 됨.
// 이때 부모 객체에게 자식 객체를 넘겨주면, 내 안에 있는 부모 부분(CVIBuffer)을 네가 복사해줘 라는 뜻이 된다.
// VIBuffer(1) <- TriCol(1) 형태라고 한다면
// VIBuffer(2) <- TriCol(2) 이런식으로 되는 것.

CTriCol::CTriCol(const CTriCol& rhs)
	:CVIBuffer(rhs)
{
	m_pMyOwner = nullptr;
}

CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Ready_Buffer()
{
	// 버텍스 사이즈 초기 설정 
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwTricnt = 1;
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

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	// 오른쪽 위
	Make_Index(pIndex);

	m_pIB->Unlock();

	return S_OK;
}

void CTriCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTriCol::Clone()
{
	return new CTriCol(*this);
}

CTriCol* CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, TRIANGLETYPE _type)
{
	CTriCol* pTriCol = new CTriCol(pGraphicDev, _type);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("pTriCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

void CTriCol::Free()
{
	CVIBuffer::Free();
}

void CTriCol::Make_Vertex(VTXCOL* pVerTex)
{
	switch (m_eType)
	{
	case TRIANGLE_X:

		pVerTex[0].vPosition = { 1.f, 0.f, 0.f };
		pVerTex[1].vPosition = { -1.f, 0.f, -1.f };
		pVerTex[2].vPosition = { -1.f, 0.f, 1.f };

		pVerTex[0].dwColor = D3DXCOLOR{ 1.f, 0.f, 1.f, 1.f }; // ARGB 색상
		pVerTex[1].dwColor = D3DXCOLOR{ 1.f, 1.f, 0.f, 1.f };
		pVerTex[2].dwColor = D3DXCOLOR{ 1.f, 1.f, 1.f, 0.f };
		break;

	case TRIANGLE_Y:

		pVerTex[0].vPosition = { 0.f, 1.f, 0.f };
		pVerTex[1].vPosition = { 1.f, -1.f, 0.f };
		pVerTex[2].vPosition = { -1.f, -1.f, 0.f };

		pVerTex[0].dwColor = D3DXCOLOR{ 1.f, 0.f, 1.f, 1.f }; // ARGB 색상
		pVerTex[1].dwColor = D3DXCOLOR{ 1.f, 1.f, 0.f, 1.f };
		pVerTex[2].dwColor = D3DXCOLOR{ 1.f, 1.f, 1.f, 0.f };
		break;

	case TRIANGLE_Z:

		pVerTex[0].vPosition = { 0.f, 0.f, 1.f };
		pVerTex[1].vPosition = { 1.f, 0.f, -1.f };
		pVerTex[2].vPosition = { -1.f, 0.f, -1.f };

		pVerTex[0].dwColor = D3DXCOLOR{ 1.f, 0.f, 1.f, 1.f }; // ARGB 색상
		pVerTex[1].dwColor = D3DXCOLOR{ 1.f, 1.f, 0.f, 1.f };
		pVerTex[2].dwColor = D3DXCOLOR{ 1.f, 1.f, 1.f, 0.f };
		break;
	}
}

void CTriCol::Make_Index(INDEX32* pIndex)
{
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;
}