#pragma once
#include "CUI.h"
#include "CProtoTypeManager.h"

class CLogo_UI :
    public CUI
{
private:

    explicit CLogo_UI(const CLogo_UI& rhs);
    explicit CLogo_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLogo_UI();

    virtual void            Free();

public:
    static CLogo_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

};

