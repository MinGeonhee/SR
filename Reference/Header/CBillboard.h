#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CBillboard :
    public CComponent
{
private:
    explicit CBillboard();
    explicit CBillboard(const CBillboard& rhs);
    explicit CBillboard(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBillboard();

public:
    HRESULT         Ready_Billboard();
    virtual _int    Update_Component(const _float& fTimeDelta) override;
    virtual void    LateUpdate_Component(const _float& fTimeDelta) override;

public:

    void            Set_Billboard();

public:
    virtual CComponent* Clone();
    static  CBillboard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    bool        m_bActive = true;
public:
    void        Set_Active(bool _bActive) { m_bActive = _bActive; }

private:

    virtual void    Free() override;

};

END
