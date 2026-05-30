#include "CCubeTex.h"
#include "Geometry.h"
#include "GeometryHelper.h"

CCubeTex::CCubeTex()
    : CVIBuffer()
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
    {
        m_vCenterUV[i] = { 0,0 };
        m_isFilpHorizontal[i] = false;
        m_isFilpVertically[i] = false;
    }

}

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
    :CVIBuffer(pGraphicDev)
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
    {
        m_vCenterUV[i] = { 0,0 };
        m_isFilpHorizontal[i] = false;
        m_isFilpVertically[i] = false;
    }
}

CCubeTex::CCubeTex(const CCubeTex& rhs)
    :CVIBuffer(rhs)
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
    {
        m_vCenterUV[i] = { 0,0 };
        m_isFilpHorizontal[i] = false;
        m_isFilpVertically[i] = false;
    }
}

CCubeTex::~CCubeTex()
{
    CVIBuffer::Free();
}

HRESULT CCubeTex::Ready_Buffer()
{

    Default_Set_CenterUV();

    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 20;
    m_dwTricnt = 10;
    m_dwFVF = FVF_TEX;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    CVIBuffer::Ready_Buffer();

    VTXTEX* pVertex = nullptr;

    m_pVB->Lock(0,0,(void**)&pVertex , 0);
#pragma region 큐브 정점

    //전면 z - 방향
    //LT
    pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vPosition = { - 0.5f,  + 1.f - MARGIN  , - 0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vPosition = { + 0.5f,  + 1.f- MARGIN  , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vPosition = { + 0.5f,  0.f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vPosition = { -0.5f,  - 0.f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };
    
    // z + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_Z_LT].vPosition = { + 0.5f,  + 1.f - MARGIN , + 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_Z_RT].vPosition = { - 0.5f,  + 1.f - MARGIN, +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_Z_RB].vPosition = { - 0.5f,   0.f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_Z_LB].vPosition = { + 0.5f,  0.f  , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };

    //전면 x - 방향d
    //LT
    pVertex[CUBE_INDEX_NEGATIVE_X_LT].vPosition = { - 0.5f,  +1.f - MARGIN , + 0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_NEGATIVE_X_RT].vPosition = { - 0.5f,  +1.f - MARGIN, - 0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_NEGATIVE_X_RB].vPosition = { - 0.5f,  0.f , - 0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_NEGATIVE_X_LB].vPosition = { - 0.5f,  0.f , + 0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };

    //전면 x + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_X_LT].vPosition = { + 0.5f,  + 1.f - MARGIN , - 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_X_RT].vPosition = { + 0.5f,  + 1.f - MARGIN, + 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_X_RB].vPosition = { + 0.5f,   0.f , + 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_X_LB].vPosition = { + 0.5f,   0.f , - 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };

    ////전면 y - 방향
    ////LT
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LT].vPosition = { + 0.5f,  0.f ,  + 0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y - TILE_UV_CENTER };
    ////RT
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RT].vPosition = { - 0.5f, 0.f ,  + 0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y - TILE_UV_CENTER };
    ////RB
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RB].vPosition = { - 0.5f,  0.f , - 0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y + TILE_UV_CENTER };
    ////LB
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LB].vPosition = { + 0.5f,  0.f , - 0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y + TILE_UV_CENTER };

    //전면 y + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_Y_LT].vPosition = { - 0.5f,  +1.f - MARGIN , + 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_Y_RT].vPosition = { + 0.5f, +1.f - MARGIN ,  + 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_Y_RB].vPosition = { + 0.5f,  +1.f - MARGIN , - 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_Y_LB].vPosition = { - 0.5f,  +1.f - MARGIN , - 0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };

#pragma endregion
    m_pVB->Unlock();


    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0,0,(void**)&pIndex, 0);
#pragma region 큐브 인덱스

    //, CUBE_INDEX_NEGATIVE_Z_RT, CUBE_INDEX_NEGATIVE_Z_RB };
    //z - 
    pIndex[0] = { CUBE_INDEX_NEGATIVE_Z_LT , CUBE_INDEX_NEGATIVE_Z_RT , CUBE_INDEX_NEGATIVE_Z_RB };
    pIndex[1] = { CUBE_INDEX_NEGATIVE_Z_LT , CUBE_INDEX_NEGATIVE_Z_RB , CUBE_INDEX_NEGATIVE_Z_LB };
    //// z+
    pIndex[2] = { CUBE_INDEX_POSITIVE_Z_LT ,CUBE_INDEX_POSITIVE_Z_RT , CUBE_INDEX_POSITIVE_Z_RB };
    pIndex[3] = { CUBE_INDEX_POSITIVE_Z_LT ,CUBE_INDEX_POSITIVE_Z_RB , CUBE_INDEX_POSITIVE_Z_LB };

    //// x-
    pIndex[4] = { CUBE_INDEX_NEGATIVE_X_LT , CUBE_INDEX_NEGATIVE_X_RT , CUBE_INDEX_NEGATIVE_X_RB };
    pIndex[5] = { CUBE_INDEX_NEGATIVE_X_LT , CUBE_INDEX_NEGATIVE_X_RB , CUBE_INDEX_NEGATIVE_X_LB };
    //// x+
    pIndex[6] = { CUBE_INDEX_POSITIVE_X_LT ,CUBE_INDEX_POSITIVE_X_RT , CUBE_INDEX_POSITIVE_X_RB };
    pIndex[7] = { CUBE_INDEX_POSITIVE_X_LT ,CUBE_INDEX_POSITIVE_X_RB , CUBE_INDEX_POSITIVE_X_LB };

    ////// -y
    //pIndex[8] = { CUBE_INDEX_NEGATIVE_Y_LT , CUBE_INDEX_NEGATIVE_Y_RT , CUBE_INDEX_NEGATIVE_Y_RB };
    //pIndex[9] = { CUBE_INDEX_NEGATIVE_Y_LT , CUBE_INDEX_NEGATIVE_Y_RB , CUBE_INDEX_NEGATIVE_Y_LB };

    // +y
    pIndex[8] = { CUBE_INDEX_POSITIVE_Y_LT ,CUBE_INDEX_POSITIVE_Y_RT , CUBE_INDEX_POSITIVE_Y_RB };
    pIndex[9] = { CUBE_INDEX_POSITIVE_Y_LT ,CUBE_INDEX_POSITIVE_Y_RB , CUBE_INDEX_POSITIVE_Y_LB };
#pragma endregion
    m_pIB->Unlock();


    return S_OK;
}

HRESULT CCubeTex::Ready_Buffer(_vec2 vCenterUV[CUBE_FACE_END])
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
    {
        m_vCenterUV[i] = vCenterUV[i];
    }

    m_dwVtxSize = sizeof(VTXTEX);
    m_dwVtxCnt = 24;
    m_dwTricnt = 12;
    m_dwFVF = FVF_TEX;
    m_IdxFmt = D3DFMT_INDEX32;
    m_dwIdxSize = sizeof(INDEX32);

    CVIBuffer::Ready_Buffer();

    VTXTEX* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);
#pragma region 큐브 정점

    //전면 z - 방향
    //LT
    pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vPosition = { -0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vPosition = { +0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vPosition = { +0.5f,  -0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vPosition = { -0.5f,  -0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };

    // z + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_Z_LT].vPosition = { +0.5f,  +0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_Z_RT].vPosition = { -0.5f,  +0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_Z_RB].vPosition = { -0.5f,  -0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_Z_LB].vPosition = { +0.5f,  -0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };

    //전면 x - 방향d
    //LT
    pVertex[CUBE_INDEX_NEGATIVE_X_LT].vPosition = { -0.5f,  +0.5f , +0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_NEGATIVE_X_RT].vPosition = { -0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_NEGATIVE_X_RB].vPosition = { -0.5f,  -0.5f , -0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_NEGATIVE_X_LB].vPosition = { -0.5f,  -0.5f , +0.5f };
    pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };

    //전면 x + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_X_LT].vPosition = { +0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_X_RT].vPosition = { +0.5f,  +0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_X_RB].vPosition = { +0.5f,  -0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_X_LB].vPosition = { +0.5f,  -0.5f , -0.5f };
    pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };

    //전면 y - 방향
    //LT
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LT].vPosition = { +0.5f,  -0.5f ,  +0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y - TILE_UV_CENTER };
    ////RT
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RT].vPosition = { -0.5f, -0.5f ,  +0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y - TILE_UV_CENTER };
    ////RB
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RB].vPosition = { -0.5f,  -0.5f , -0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y + TILE_UV_CENTER };
    ////LB
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LB].vPosition = { +0.5f,  -0.5f , -0.5f };
    //pVertex[CUBE_INDEX_NEGATIVE_Y_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Y].y + TILE_UV_CENTER };

    //전면 y + 방향
    //LT
    pVertex[CUBE_INDEX_POSITIVE_Y_LT].vPosition = { -0.5f,  +0.5f , +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    //RT
    pVertex[CUBE_INDEX_POSITIVE_Y_RT].vPosition = { +0.5f, +0.5f ,  +0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    //RB
    pVertex[CUBE_INDEX_POSITIVE_Y_RB].vPosition = { +0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };
    //LB
    pVertex[CUBE_INDEX_POSITIVE_Y_LB].vPosition = { -0.5f,  +0.5f , -0.5f };
    pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };

#pragma endregion
    m_pVB->Unlock();

    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);
#pragma region 큐브 인덱스

    //, CUBE_INDEX_NEGATIVE_Z_RT, CUBE_INDEX_NEGATIVE_Z_RB };
    //z - 
    pIndex[0] = { CUBE_INDEX_NEGATIVE_Z_LT , CUBE_INDEX_NEGATIVE_Z_RT , CUBE_INDEX_NEGATIVE_Z_RB };
    pIndex[1] = { CUBE_INDEX_NEGATIVE_Z_LT , CUBE_INDEX_NEGATIVE_Z_RB , CUBE_INDEX_NEGATIVE_Z_LB };
    //// z+
    pIndex[2] = { CUBE_INDEX_POSITIVE_Z_LT ,CUBE_INDEX_POSITIVE_Z_RT , CUBE_INDEX_POSITIVE_Z_RB };
    pIndex[3] = { CUBE_INDEX_POSITIVE_Z_LT ,CUBE_INDEX_POSITIVE_Z_RB , CUBE_INDEX_POSITIVE_Z_LB };

    //// x-
    pIndex[4] = { CUBE_INDEX_NEGATIVE_X_LT , CUBE_INDEX_NEGATIVE_X_RT , CUBE_INDEX_NEGATIVE_X_RB };
    pIndex[5] = { CUBE_INDEX_NEGATIVE_X_LT , CUBE_INDEX_NEGATIVE_X_RB , CUBE_INDEX_NEGATIVE_X_LB };
    //// x+
    pIndex[6] = { CUBE_INDEX_POSITIVE_X_LT ,CUBE_INDEX_POSITIVE_X_RT , CUBE_INDEX_POSITIVE_X_RB };
    pIndex[7] = { CUBE_INDEX_POSITIVE_X_LT ,CUBE_INDEX_POSITIVE_X_RB , CUBE_INDEX_POSITIVE_X_LB };

    ////// -y
    //pIndex[8] = { CUBE_INDEX_NEGATIVE_Y_LT , CUBE_INDEX_NEGATIVE_Y_RT , CUBE_INDEX_NEGATIVE_Y_RB };
    //pIndex[9] = { CUBE_INDEX_NEGATIVE_Y_LT , CUBE_INDEX_NEGATIVE_Y_RB , CUBE_INDEX_NEGATIVE_Y_LB };

    // +y
    pIndex[8] = { CUBE_INDEX_POSITIVE_Y_LT ,CUBE_INDEX_POSITIVE_Y_RT , CUBE_INDEX_POSITIVE_Y_RB };
    pIndex[9] = { CUBE_INDEX_POSITIVE_Y_LT ,CUBE_INDEX_POSITIVE_Y_RB , CUBE_INDEX_POSITIVE_Y_LB };
#pragma endregion
    m_pIB->Unlock();
#pragma endregion
    m_pIB->Unlock();
    return S_OK;
}

void CCubeTex::Render_Buffer()
{
    CVIBuffer::Render_Buffer();
}

CComponent* CCubeTex::Clone()
{
    CCubeTex* pCubeTex = new CCubeTex(m_pGraphicDev);

    if (FAILED(pCubeTex->Ready_Buffer()))
    {
        Safe_Release(pCubeTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pCubeTex;
}

CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCubeTex* pCubeTex = new CCubeTex(pGraphicDev);

    if (FAILED(pCubeTex->Ready_Buffer()))
    {
        Safe_Release(pCubeTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pCubeTex;
}

CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec2 vCenterUV[CUBE_FACE_END])
{
    CCubeTex* pCubeTex = new CCubeTex(pGraphicDev);

    if (FAILED(pCubeTex->Ready_Buffer(vCenterUV)))
    {
        Safe_Release(pCubeTex);
        MSG_BOX("pRcTex Create Failed");
        return nullptr;
    }

    return pCubeTex;
}

void CCubeTex::Default_Set_CenterUV()
{
    for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
    {
            m_vCenterUV[i] = { DEFUALT_NAGATIVEY_CENTER_BOSS_X , DEFUALT_NAGATIVEY_CENTER_BOSS_Y };
    }
}

void CCubeTex::ChangeAllUI(const _vec2& vCenterUV)
{
    for (int i = 0; i < CUBE_FACE_END; ++i)
    {
        m_vCenterUV[i] = vCenterUV;
    }

    VTXTEX* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_Z].y + TILE_UV_CENTER };

    pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Z].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Z].y + TILE_UV_CENTER };


    pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };
    pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_NEGATIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_NEGATIVE_X].y + TILE_UV_CENTER };

    pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_X].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_X].y + TILE_UV_CENTER };

    pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y - TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x + TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };
    pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { m_vCenterUV[CUBE_FACE_POSITIVE_Y].x - TILE_UV_CENTER , m_vCenterUV[CUBE_FACE_POSITIVE_Y].y + TILE_UV_CENTER };

    m_pVB->Unlock();
}

void CCubeTex::ChangeTextureCubeFaceUV(const CUBE_FACE& etype, const _vec2& vCenterUV)
{

    m_vCenterUV[etype]  = vCenterUV;

    float centerX = m_vCenterUV[etype].x;
    float centerY = m_vCenterUV[etype].y;

    float left = centerX - TILE_UV_CENTER;
    float right = centerX + TILE_UV_CENTER;
    float top = centerY - TILE_UV_CENTER;
    float bottom = centerY + TILE_UV_CENTER;

    if (m_isFilpHorizontal[etype] == true)
    {
        //false면 좌우반전 되기전 원본이기깨문에 좌우반전 레츠고
        std::swap(left, right);
    }

    if (m_isFilpVertically[etype] == true)
    {
        //false면 좌우반전 되기전 원본이기깨문에 좌우반전 레츠고
        std::swap(top, bottom);
    }



    VTXTEX* pVertex = nullptr;

    m_pVB->Lock(0,0,(void**)&pVertex, 0);

    switch (etype)
    {
    case Engine::CUBE_FACE_POSITIVE_Z:
        pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { left, bottom };
        break;

    case Engine::CUBE_FACE_NEGATIVE_Z:
        pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { left, bottom };
        break;

    case Engine::CUBE_FACE_POSITIVE_X:
        pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { left, bottom };
        break;

    case Engine::CUBE_FACE_NEGATIVE_X:
        pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { left, bottom };
        break;

    case Engine::CUBE_FACE_POSITIVE_Y:
        pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { left, bottom };
        break;

        // NEGATIVE_Y는 쓰지 않아서 주석 처리한 거 유지함
    default:
        break;
    }

    m_pVB->Unlock();
}

void CCubeTex::Filp_Horizontal(const CUBE_FACE& etype)
{


    float centerX = m_vCenterUV[etype].x;
    float centerY = m_vCenterUV[etype].y;

    float left = centerX - TILE_UV_CENTER;
    float right = centerX + TILE_UV_CENTER;
    float top = centerY - TILE_UV_CENTER;
    float bottom = centerY + TILE_UV_CENTER;

    if (m_isFilpHorizontal[etype] == false)
    {
        //false면 좌우반전 되기전 원본이기깨문에 좌우반전 레츠고
        std::swap(left,right);
        m_isFilpHorizontal[etype] = true;
    }
    else
    {
        //True 면 좌우반전되었다가 원래 값으로 바뀌면 된다.
        m_isFilpHorizontal[etype] = false;
    }
   
    VTXTEX* pVertex = nullptr;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    switch (etype)
    {
    case Engine::CUBE_FACE_NEGATIVE_Z:

        pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { left, bottom };

        break;
    case Engine::CUBE_FACE_POSITIVE_Z:

        pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { left, bottom };

        break;
    case Engine::CUBE_FACE_NEGATIVE_X:

        pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { left, bottom };

        break;
    case Engine::CUBE_FACE_POSITIVE_X:

        pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { left, bottom };


        break;
    case Engine::CUBE_FACE_POSITIVE_Y:

        pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { left, bottom };


        break;
    case Engine::CUBE_FACE_END:
        break;
    default:
        break;
    }


    m_pVB->Unlock();



}

void CCubeTex::Filp_Vertically(const CUBE_FACE& etype)
{
    float centerX = m_vCenterUV[etype].x;
    float centerY = m_vCenterUV[etype].y;

    float left = centerX - TILE_UV_CENTER;
    float right = centerX + TILE_UV_CENTER;
    float top = centerY - TILE_UV_CENTER;
    float bottom = centerY + TILE_UV_CENTER;

    if (m_isFilpVertically[etype] == false)
    {
        std::swap(top, bottom);  // 상하 반전
        m_isFilpVertically[etype] = true;
    }
    else
    {
        m_isFilpVertically[etype] = false;  // 원래대로
    }

    VTXTEX* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    switch (etype)
    {
    case Engine::CUBE_FACE_NEGATIVE_Z:
        pVertex[CUBE_INDEX_NEGATIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_Z_LB].vTexUV = { left, bottom };
        break;
    case Engine::CUBE_FACE_POSITIVE_Z:
        pVertex[CUBE_INDEX_POSITIVE_Z_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Z_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Z_LB].vTexUV = { left, bottom };
        break;
    case Engine::CUBE_FACE_NEGATIVE_X:
        pVertex[CUBE_INDEX_NEGATIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_NEGATIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_NEGATIVE_X_LB].vTexUV = { left, bottom };
        break;
    case Engine::CUBE_FACE_POSITIVE_X:
        pVertex[CUBE_INDEX_POSITIVE_X_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_X_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_X_LB].vTexUV = { left, bottom };
        break;
    case Engine::CUBE_FACE_POSITIVE_Y:
        pVertex[CUBE_INDEX_POSITIVE_Y_LT].vTexUV = { left, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RT].vTexUV = { right, top };
        pVertex[CUBE_INDEX_POSITIVE_Y_RB].vTexUV = { right, bottom };
        pVertex[CUBE_INDEX_POSITIVE_Y_LB].vTexUV = { left, bottom };
        break;
    case Engine::CUBE_FACE_END:
        break;
    default:
        break;
    }

    m_pVB->Unlock();
}




void CCubeTex::Free()
{
    CVIBuffer::Free();
}
