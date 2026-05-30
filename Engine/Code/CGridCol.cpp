#include "CGridCol.h"
#include "GeometryHelper.h"
#include "Geometry.h"

CGridCol::CGridCol()
	: CVIBuffer()
{
}

CGridCol::CGridCol(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{
}

CGridCol::CGridCol(const CGridCol& cpy)
	: CVIBuffer(cpy)
{
}

CGridCol::~CGridCol()
{
}

HRESULT CGridCol::Ready_Buffer(const int32& CntX, const int32& CntZ, const int32 Floor, int32 Size)
{
	Geometry<VTXCOL>* _Geometry = new Geometry<VTXCOL>;
	GeometryHelper::CreateGridLine(_Geometry, CntX, CntZ, Floor, Size);

	m_dwTricnt = _Geometry->GetIndexCount()/2;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = _Geometry->GetVertexCount();
	m_dwFVF = FVF_COL;


	m_dwIdxSize = sizeof(_uint32) * 2 ;
	m_IdxFmt = D3DFMT_INDEX32;

#pragma region 버퍼생성

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 버텍스 버퍼의 크기
		0,	// 버퍼의 종류(속성) -> D3DUSAGE_DYNAMIC
		m_dwFVF,	// 버텍스 속성
		D3DPOOL_MANAGED,	// 메모리 풀 종류
		&m_pVB,	// 생성된 버퍼를 받을 포인터
		NULL)))	// 공유 핸들
		return E_FAIL;

	// 인덱스 버퍼.
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTricnt, // 인덱스 버퍼의 크기. 
		0,	// 버퍼의 종류(속성) -> D3DUSAGE_DYNAMIC
		m_IdxFmt,	// 인덱스 속성
		D3DPOOL_MANAGED,	// 메모리 풀 종류
		&m_pIB,	// 생성된 버퍼를 받을 포인터                                            
		NULL)))	// 공유 핸들
		return E_FAIL;

#pragma endregion

#pragma region 버퍼넣기


	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, _Geometry->GetVertexData() , m_dwVtxCnt * m_dwVtxSize);
	m_pVB->Unlock();

	_uint32* pIndex = nullptr;
	vector<_uint32> vecIndex = _Geometry->GetIndices();

	m_pIB->Lock( 0 , 0 , (void**)&pIndex, 0);
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		pIndex[i] = vecIndex[i];
	}
	//memcpy(pIndex, _Geometry->GetIndexData(), m_dwTricnt * m_dwIdxSize);
	m_pIB->Unlock();

#pragma endregion

	Safe_Delete(_Geometry); // 쓰고나면 바로 삭제.

	return S_OK;
}

void CGridCol::Render_Buffer()
{

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF); // 버텍스 버퍼에 보관될 버텍스의 유연한 버텍스 포멧
	m_pGraphicDev->SetIndices(m_pIB); // 

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_dwVtxCnt, 0, m_dwTricnt);

}

CGridCol* CGridCol::Create(LPDIRECT3DDEVICE9 device, const int32& CntX, const int32& CntZ, const int32 Floor, int32 Size)
{
	CGridCol* newGrid = new CGridCol(device);
	if (newGrid == nullptr)
		return nullptr;

	if (FAILED(newGrid->Ready_Buffer(CntX, CntZ, Floor, Size)))
	{
		MSG_BOX("Create Grid Is Failed");
		Safe_Release(newGrid);
		return nullptr;
	}

	return newGrid;
}

CComponent* CGridCol::Clone()
{
	return new CGridCol(*this);
}

void CGridCol::Free()
{
	CVIBuffer::Free();
}
