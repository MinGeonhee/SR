#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTileTex :
    public CVIBuffer
{
private:

    explicit CTileTex();
    explicit CTileTex(LPDIRECT3DDEVICE9 device);
    explicit CTileTex(const CTileTex& cpy);
    virtual ~CTileTex();

private:

    virtual HRESULT Ready_Buffer(const _vec2& vCenterUV);


public:
    static      CTileTex* Create(LPDIRECT3DDEVICE9 device, const _vec2& vCenterUV = {});

    virtual     CComponent* Clone() override;

    virtual void    Render_Buffer();

    HRESULT         Change_CenterUV(const _vec2 vCenterUV);
    HRESULT         Change_IndexCCW();

    const _vec2&     Get_CenterUV() { return m_vCenterUV; }

private:

    _vec2       m_vCenterUV;

private:
    virtual     void    Free();
};

END