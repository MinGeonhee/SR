#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CUITileTex :
    public CVIBuffer
{
private:
    explicit CUITileTex();
    explicit CUITileTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUITileTex(const CUITileTex& rhs);
    virtual ~CUITileTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    HRESULT Change_UV(int idxX, int countX, int idxY, int countY);

public:
    virtual CComponent* Clone() override;
    static CUITileTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;

private:
    float m_fWidth = 700.f;
    float m_fHeight = 450.f;
};

END