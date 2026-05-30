#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CUIRecTex :
    public CVIBuffer
{
private:
    explicit CUIRecTex();
    explicit CUIRecTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUIRecTex(const CUIRecTex& rhs);
    virtual ~CUIRecTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();
public:
    virtual CComponent* Clone() override;
    static CUIRecTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;

private:
    float m_fWidth = 700.f;
    float m_fHeight = 450.f;
};

END
