#pragma once
#include "CDecor.h"

BEGIN(Engine)

class ENGINE_DLL CWallDecor final :
    public CDecor
{
private:
    explicit CWallDecor(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CWallDecor(const CWallDecor& rhs);
    virtual ~CWallDecor();
public:

    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();


private:

    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:
    static  CWallDecor* Create(LPDIRECT3DDEVICE9 Device);
private:
    virtual     void        Free();
};

END