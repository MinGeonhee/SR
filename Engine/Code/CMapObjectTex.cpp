#include "CMapObjectTex.h"
#include "CGraphicDev.h"

CMapObjectTex::CMapObjectTex()
    : CVIBuffer()
{
}

CMapObjectTex::CMapObjectTex(LPDIRECT3DDEVICE9 Device)
    : CVIBuffer(Device)
{
}

CMapObjectTex::CMapObjectTex(const CMapObjectTex& cpy)
    : CVIBuffer(cpy)
{
}

CMapObjectTex::~CMapObjectTex()
{
}

HRESULT CMapObjectTex::Change_Buffer(const MAPOBJECT_PLACEMENT& eType, const SpriteRect& tSprite)
{

    _vec2 _LT = LT(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _RT = RT(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _RB = RB(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _LB = LB(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);


    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;


    if (eType == MAPOBJECT_PLACEMENT::GROUND)
    {
        //땅 타입이면 기본 로컬 포지션값을 위로 올려준다.
        pVertex[BUFFER_LT].vPosition.y = 1.f;
        pVertex[BUFFER_RT].vPosition.y = 1.f;
        pVertex[BUFFER_RB].vPosition.y = 0.f;
        pVertex[BUFFER_LB].vPosition.y = 0.f;
    }
    else if (eType == MAPOBJECT_PLACEMENT::WALL)
    {
        pVertex[BUFFER_LT].vPosition.y = +0.5f;
        pVertex[BUFFER_RT].vPosition.y = +0.5f;
        pVertex[BUFFER_RB].vPosition.y = -0.5f;
        pVertex[BUFFER_LB].vPosition.y = -0.5f;
    }

    pVertex[0].vTexUV = { _LT.x , _LT.y }; // 가로 세로
    pVertex[1].vTexUV = { _RT.x , _RT.y };
    pVertex[2].vTexUV = { _RB.x , _RB.y };
    pVertex[3].vTexUV = { _LB.x , _LB.y };

    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;



    return S_OK;
}

HRESULT CMapObjectTex::Change_Buffer(const MAPOBJECT_PLACEMENT& eType, const UVRect& tUVRect)
{

    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;


    if (eType == MAPOBJECT_PLACEMENT::GROUND)
    {
        //땅 타입이면 기본 로컬 포지션값을 위로 올려준다.
        pVertex[BUFFER_LT].vPosition.y = 1.f;
        pVertex[BUFFER_RT].vPosition.y = 1.f;
        pVertex[BUFFER_RB].vPosition.y = 0.f;
        pVertex[BUFFER_LB].vPosition.y = 0.f;
    }
    else if (eType == MAPOBJECT_PLACEMENT::WALL)
    {
        pVertex[BUFFER_LT].vPosition.y = +0.5f;
        pVertex[BUFFER_RT].vPosition.y = +0.5f;
        pVertex[BUFFER_RB].vPosition.y = -0.5f;
        pVertex[BUFFER_LB].vPosition.y = -0.5f;
    }

    pVertex[UV_LT].vTexUV = { tUVRect.vUV[UV_LT].x , tUVRect.vUV[UV_LT].y }; // 가로 세로
    pVertex[UV_RT].vTexUV = { tUVRect.vUV[UV_RT].x , tUVRect.vUV[UV_RT].y };
    pVertex[UV_RB].vTexUV = { tUVRect.vUV[UV_RB].x , tUVRect.vUV[UV_RB].y };
    pVertex[UV_LB].vTexUV = { tUVRect.vUV[UV_LB].x , tUVRect.vUV[UV_LB].y };

    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;



    return S_OK;
}

HRESULT CMapObjectTex::Change_Buffer(const MAPOBJECT_PLACEMENT& eType)
{
    
    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;


    if (eType == MAPOBJECT_PLACEMENT::GROUND)
    {
        //땅 타입이면 기본 로컬 포지션값을 위로 올려준다.
        pVertex[BUFFER_LT].vPosition.y = 1.f;
        pVertex[BUFFER_RT].vPosition.y = 1.f;
        pVertex[BUFFER_RB].vPosition.y = 0.f;
        pVertex[BUFFER_LB].vPosition.y = 0.f;
    }

    else if (eType == MAPOBJECT_PLACEMENT::WALL)
    {
        pVertex[BUFFER_LT].vPosition.y = +0.5f;
        pVertex[BUFFER_RT].vPosition.y = +0.5f;
        pVertex[BUFFER_RB].vPosition.y = -0.5f;
        pVertex[BUFFER_LB].vPosition.y = -0.5f;
    }

    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;



    return S_OK;
}

HRESULT CMapObjectTex::Change_Buffer(const SpriteRect& tSprite)
{
    _vec2 _LT = LT(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _RT = RT(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _RB = RB(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);
    _vec2 _LB = LB(tSprite.x, tSprite.y, tSprite.w, tSprite.h, tSprite.Mw, tSprite.Mh);

    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;

    pVertex[0].vTexUV = { _LT.x , _LT.y }; // 가로 세로
    pVertex[1].vTexUV = { _RT.x , _RT.y };
    pVertex[2].vTexUV = { _RB.x , _RB.y };
    pVertex[3].vTexUV = { _LB.x , _LB.y };

    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;

    return S_OK;

}

HRESULT CMapObjectTex::Change_Buffer(const UVRect& tUVRect)
{
    VTXTEX* pVertex = nullptr;


    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertex, 0)))
        return E_FAIL;

    pVertex[UV_LT].vTexUV = { tUVRect.vUV[UV_LT].x , tUVRect.vUV[UV_LT].y}; // 가로 세로
    pVertex[UV_RT].vTexUV = { tUVRect.vUV[UV_RT].x , tUVRect.vUV[UV_RT].y };
    pVertex[UV_RB].vTexUV = { tUVRect.vUV[UV_RB].x , tUVRect.vUV[UV_RB].y };
    pVertex[UV_LB].vTexUV = { tUVRect.vUV[UV_LB].x , tUVRect.vUV[UV_LB].y };

    if (FAILED(m_pVB->Unlock()))
        return E_FAIL;


    return S_OK;
}



HRESULT CMapObjectTex::Ready_Buffer()
{
    _float Offset = 0.015625;

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
    pVertex[0].vPosition = { -0.5f , 1.f, 0.f };
    pVertex[0].vTexUV = {  0 , 0  }; // 가로 세로

    pVertex[1].vPosition = { +0.5f, 1.f, + 0.f };
    pVertex[1].vTexUV = {  1 , 0  };

    pVertex[2].vPosition = { +0.5f, 0.f, +0.f };
    pVertex[2].vTexUV = {  1 , 1 };

    pVertex[3].vPosition = { -0.5f, 0.f, 0.f };
    pVertex[3].vTexUV = { 0 , 1 };

    m_pVB->Unlock();

    INDEX32* pIndex = NULL;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 오른쪽 위
    pIndex[0]._0 = 0;
    pIndex[0]._1 = 1;
    pIndex[0]._2 = 2;

    // 왼쪽 CMapObjectTex
    pIndex[1]._0 = 0;
    pIndex[1]._1 = 2;
    pIndex[1]._2 = 3;

    m_pIB->Unlock();


    return S_OK;

}



void CMapObjectTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}


CMapObjectTex* CMapObjectTex::Create(LPDIRECT3DDEVICE9 Device)
{
    CMapObjectTex* pGroundTex = new CMapObjectTex(Device);

    if (FAILED(pGroundTex->Ready_Buffer()))
    {
        Safe_Release(pGroundTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pGroundTex;
}

CComponent* CMapObjectTex::Clone()
{
    CComponent* pGroundTex = Create(CGraphicDev::GetInstance()->m_pGraphicDev);

	return pGroundTex;
}


void CMapObjectTex::Free()
{
    CVIBuffer::Free();
}
