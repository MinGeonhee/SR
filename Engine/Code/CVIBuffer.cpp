#include "CVIBuffer.h"

CVIBuffer::CVIBuffer()
	:m_dwVtxSize(0), m_dwVtxCnt(0), m_dwTricnt(0), m_dwFVF(0)
	, m_dwIdxSize(0), m_pVB(nullptr), m_pIB(nullptr)
{
}
CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_dwVtxSize(0), m_dwVtxCnt(0), m_dwTricnt(0), m_dwFVF(0)
	, m_dwIdxSize(0), m_pVB(nullptr), m_pIB(nullptr)
{
	//m_pMyOwner = nullptr;
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	:CComponent(rhs),
	m_dwVtxSize(rhs.m_dwVtxSize), m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwIdxSize(rhs.m_dwIdxSize),
	m_dwTricnt(rhs.m_dwTricnt), m_dwFVF(rhs.m_dwFVF), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
{
	//m_pMyOwner = nullptr;
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}


HRESULT CVIBuffer::Ready_Buffer()
{

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



	return S_OK;
}

// 버퍼에 있는 것을 그리는 함수
void CVIBuffer::Render_Buffer()
{
	// 몇 번 스트림으로 설정을 할거냐(몇 번 통로로 설정해서 출력을 하겠느냐.)
	// -> 여기에 저장되어있는 vertex들을 그리기 디바이스에게 넘겨서 출력을 하겠다.넘기는 역할을 하는 setstream.
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF); // 버텍스 버퍼에 보관될 버텍스의 유연한 버텍스 포멧
	m_pGraphicDev->SetIndices(m_pIB); // 

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTricnt);
	/*m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTricnt);*/
	// 인덱스 기준으로 나는 그리겠다.
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
