#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRecTex :
    public CVIBuffer
{
private:
    explicit CRecTex();
    explicit CRecTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRecTex(const CRecTex& rhs);
    virtual ~CRecTex();

public:

    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

public:
    virtual CComponent* Clone() override;
    static CRecTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void        Free();
};

END