#include "CPlaneColl.h"

CPlaneColl::CPlaneColl()
{
	m_pMyOwner = nullptr;
}

CPlaneColl::CPlaneColl(LPDIRECT3DDEVICE9 mGraphicDev)
	:CVIBuffer(mGraphicDev)
{
	m_pMyOwner = nullptr;
}

// 자식을 복사하면 부모도 같이 복사가 됨.
// 이때 부모 객체에게 자식 객체를 넘겨주면, 내 안에 있는 부모 부분(CVIBuffer)을 네가 복사해줘 라는 뜻이 된다.
// VIBuffer(1) <- TriCol(1) 형태라고 한다면
// VIBuffer(2) <- TriCol(2) 이런식으로 되는 것.

CPlaneColl::CPlaneColl(const CPlaneColl& rhs)
	:CVIBuffer(rhs)
{
	m_pMyOwner = nullptr;
}

CPlaneColl::~CPlaneColl()
{
	m_pMyOwner = nullptr;
}

HRESULT CPlaneColl::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = (TileSize+1)* (TileSize+1);
	m_dwTricnt = (128*128)*2;
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

void CPlaneColl::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CPlaneColl::Clone()
{
	return new CPlaneColl(*this);
}

CPlaneColl* CPlaneColl::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlaneColl* pPlane = new CPlaneColl(pGraphicDev);

	if (FAILED(pPlane->Ready_Buffer()))
	{
		Safe_Release(pPlane);
		MSG_BOX("CRectCol Create Failed");
		return nullptr;
	}

	return pPlane;
}

void CPlaneColl::Free()
{
	CVIBuffer::Free();
}

void CPlaneColl::Make_Vertex(VTXCOL* pVerTex)
{
	for (_uint i = 0; i < TileSize+1; ++i)
	{
		for (_uint j = 0; j < TileSize+1; ++j)
		{
			// 수정: xy 평면에 그림
			pVerTex[j + (TileSize + 1) * i].vPosition = {-(TileSize / 2.f) + j, -(TileSize / 2.f) + i, 0.f};
			pVerTex[j + (TileSize + 1) * i].dwColor = D3DXCOLOR{ 1.f, 1.f, 1.f, 1.f };
		}
	}
}

void CPlaneColl::Make_Index(INDEX32* pIndex)
{
	_uint count = 0;

	for (_uint i = 0; i < TileSize; ++i)
	{
		for (_uint j = 0; j < TileSize; ++j)
		{
			_uint leftdown = j + ((TileSize+1) * i);
			_uint rightdown = leftdown + 1;
			_uint lefttop = leftdown + (TileSize+1);
			_uint righttop = lefttop + 1;
					 
			pIndex[count]._0 = leftdown;
			pIndex[count]._1 = lefttop;
			pIndex[count]._2 = rightdown;
			count++;

			pIndex[count]._0 = lefttop;
			pIndex[count]._1 = righttop;
			pIndex[count]._2 = rightdown;
			count++;
		}
	}
}
