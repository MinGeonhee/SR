#include "CCubeCol.h"

CCubeCol::CCubeCol()
{
	m_pMyOwner = nullptr;
}

CCubeCol::CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
	m_pMyOwner = nullptr;
}

CCubeCol::CCubeCol(const CCubeCol& rhs)
	:CVIBuffer(rhs)
{
	m_pMyOwner = nullptr;
}

CCubeCol::~CCubeCol()
{
	m_pMyOwner = nullptr;
}

HRESULT CCubeCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;
	m_dwTricnt = 12;
	m_dwFVF = FVF_COL;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	Make_Vertex(pVertex);
	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, m_IdxFmt);
	Make_Index(pIndex);
	m_pIB->Unlock();

	return S_OK;
}

void CCubeCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CCubeCol::Change_Color(const _vec4& ColorARGB)
{
	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (int i = 0; i < 8; ++i)
	{
		pVertex[i].dwColor = D3DCOLOR_RGBA((int)ColorARGB.x, (int)ColorARGB.y, (int)ColorARGB.z, (int)ColorARGB.w);
	}

	m_pVB->Unlock();
}


void CCubeCol::Make_Vertex(VTXCOL* pVerTex)
{
	pVerTex[0].vPosition = { -0.5f,  0.f,  -0.5f };
	pVerTex[1].vPosition = {  0.5f,  0.f,  -0.5f };
	pVerTex[2].vPosition = {  0.5f,  1.f,  -0.5f };
	pVerTex[3].vPosition = { -0.5f,  1.f,  -0.5f };
	pVerTex[4].vPosition = { -0.5f,  0.f,   0.5f };
	pVerTex[5].vPosition = {  0.5f,  0.f,   0.5f };
	pVerTex[6].vPosition = {  0.5f,	 1.f ,  0.5f };
	pVerTex[7].vPosition = { -0.5f,  1.f,   0.5f };

	for (int i = 0; i < 8; ++i)
	{
		pVerTex[i].dwColor = D3DXCOLOR{ 1.f, 0.f, 0.f, 0.5f }; 
	}
}

void CCubeCol::Make_Index(INDEX32* pIndex)
{

	pIndex[0] = { 4, 5, 6 };
	pIndex[1] = { 4, 6, 7 };

	pIndex[2] = { 0, 2, 1 };
	pIndex[3] = { 0, 3, 2 };

	pIndex[4] = { 0, 7, 3 };
	pIndex[5] = { 0, 4, 7 };

	pIndex[6] = { 1, 2, 6 };
	pIndex[7] = { 1, 6, 5 };

	pIndex[8] = { 3, 7, 6 };
	pIndex[9] = { 3, 6, 2 };

	pIndex[10] = { 0, 1, 5 };
	pIndex[11] = { 0, 5, 4 };

}

CComponent* CCubeCol::Clone()
{
	CCubeCol* pTriCol = new CCubeCol(m_pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("CCubeCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

CCubeCol* CCubeCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeCol* pTriCol = new CCubeCol(pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("CCubeCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

void CCubeCol::Free()
{
	CVIBuffer::Free();
}