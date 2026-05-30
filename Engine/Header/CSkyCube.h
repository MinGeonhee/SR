#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSkyCube :
    public CVIBuffer
{
private:
    explicit CSkyCube();
    explicit CSkyCube(LPDIRECT3DDEVICE9 pGrpahicDev);
    explicit CSkyCube(const CSkyCube& rhs);
    virtual ~CSkyCube();

public:

    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

public:
    virtual CComponent* Clone() override;
    static CSkyCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void        Free();
};

END