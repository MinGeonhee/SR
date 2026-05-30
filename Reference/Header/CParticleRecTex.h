#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CParticleRecTex :
    public CVIBuffer
{
private:
    explicit CParticleRecTex();
    explicit CParticleRecTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CParticleRecTex(const CParticleRecTex& rhs);
    virtual ~CParticleRecTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    HRESULT         Change_CenterUV(const _vec2 vCenterIndex);


public:
    virtual CComponent* Clone() override;
    static CParticleRecTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _vec2       m_vUVCenter;
    _vec2       m_vUVCenterIndex;
    // 8 / 256 ÇÑ°Å
    float       m_fUVOffset = 0.03125f;


private:
    virtual void Free() override;

};

END


