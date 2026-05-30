#include "CCenterLineCol.h"

CCenterLineCol::CCenterLineCol()
	: CVIBuffer()
{
}

CCenterLineCol::CCenterLineCol(LPDIRECT3DDEVICE9 device)
	: CVIBuffer(device)
{
}

CCenterLineCol::CCenterLineCol(const CCenterLineCol& cpy)
	: CVIBuffer(cpy)
{
}

CCenterLineCol::~CCenterLineCol()
{
}

HRESULT CCenterLineCol::Ready_Buffer(const _float& Floor)
{
	m_dwTricnt =  2;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt =  4;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(_uint32) * 2;
	m_IdxFmt = D3DFMT_INDEX32;

	CVIBuffer::Ready_Buffer();
	
	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { +0.3f , Floor , 0.f };
	pVertex[0].dwColor = D3DCOLOR_ARGB(255, 255 , 0, 0);

	pVertex[1].vPosition = { -0.3f , Floor , 0.f };
	pVertex[1].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertex[2].vPosition = { 0.f , Floor , +0.3f };
	pVertex[2].dwColor = D3DCOLOR_ARGB(255,0,0,255);

	pVertex[3].vPosition = { 0.f , Floor , -0.3f };
	pVertex[3].dwColor = D3DCOLOR_ARGB(255, 255, 0, 255);

	m_pVB->Unlock();
	
	__int32* pIndex = nullptr;
	m_pIB->Lock(0,0,(void**)&pIndex, 0);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CCenterLineCol::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF); // 버텍스 버퍼에 보관될 버텍스의 유연한 버텍스 포멧
	m_pGraphicDev->SetIndices(m_pIB); // 

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_dwVtxCnt, 0, m_dwTricnt);

}

CCenterLineCol* CCenterLineCol::Create(LPDIRECT3DDEVICE9 device, const _float& Floor)
{
	CCenterLineCol* newCenterLine = new CCenterLineCol(device);
	if (newCenterLine == nullptr)
		return nullptr;

	if (FAILED(newCenterLine->Ready_Buffer(Floor)))
	{
		MSG_BOX("Create Grid Is Failed");
		Safe_Release(newCenterLine);
		return nullptr;
	}

	return newCenterLine;
}

CComponent* CCenterLineCol::Clone()
{
	return new CCenterLineCol(*this);
}

void CCenterLineCol::Free()
{
	CVIBuffer::Free();
}
