#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeNetTex :
    public CVIBuffer
{
private:
    explicit CCubeNetTex();
    explicit CCubeNetTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCubeNetTex(const CCubeNetTex& rhs);
    virtual ~CCubeNetTex();

public:

    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

public:
    virtual CComponent* Clone() override;
    static CCubeNetTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void        Free();
};

END

