#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex :
    public CVIBuffer
{
private:
    explicit CTerrainTex();
    explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrainTex(const CTerrainTex& rhs);
    virtual ~CTerrainTex();

public:
    HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
    void    Render_Buffer();

public:
    virtual CComponent* Clone() override;
    static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _ulong& dwCntX = VTXCNTX,
        const _ulong& dwCntZ = VTXCNTZ,
        const _ulong& dwVtxItv = VTXITV);



private:
    virtual void        Free();
};

END