#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

class CMap_UI :
    public CUI
{
private:

    explicit CMap_UI(const CMap_UI& rhs);
    explicit CMap_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMap_UI();

    virtual void            Free();

public:
    static CMap_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        Set_Toggle(const _float& fTimeDelta);

private:
    bool m_bVanish = false;
};

