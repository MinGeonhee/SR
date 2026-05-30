
#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainCol :
    public CVIBuffer
{
private:

    explicit CTerrainCol();
    explicit CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrainCol(const CTerrainCol& rhs);
    virtual ~CTerrainCol();

public:
    // 누구 기준으로 
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

protected:

    virtual void    Make_Vertex(VTXCOL* pVerTex) override;
    virtual void    Make_Index(INDEX32* pIndex) override;

public:
    virtual CComponent* Clone() override;

    static  CTerrainCol* Create(LPDIRECT3DDEVICE9 mGraphicDev);
    // 이거 객체 생성할 때 왜 컴 객체를 들고 오지.
    // 아 생성할 때 생성자에 넣어주려고.

private:
    virtual void    Free();

private:
    _uint   TileSize = 30;
};

END

