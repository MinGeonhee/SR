#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CParticleOnFloorRecTex :
    public CVIBuffer
{
private:
    explicit CParticleOnFloorRecTex();
    explicit CParticleOnFloorRecTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CParticleOnFloorRecTex(const CParticleOnFloorRecTex& rhs);
    virtual ~CParticleOnFloorRecTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    HRESULT         Change_CenterUV(const _vec2 vCenterIndex);


public:
    virtual CComponent* Clone() override;
    static CParticleOnFloorRecTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _vec2       m_vUVCenter;
    _vec2       m_vUVCenterIndex;
    // 8 / 256 ÇÑ°Å
    float       m_fUVOffset = 0.125f;


private:
    virtual void Free() override;

};

END


