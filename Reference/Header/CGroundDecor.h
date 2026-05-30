#pragma once
#include "CDecor.h"


BEGIN(Engine)


class CBillboard;

class ENGINE_DLL CGroundDecor  final :
    public CDecor
{
private:
    explicit CGroundDecor(LPDIRECT3DDEVICE9 Device);
    explicit CGroundDecor(const CGroundDecor& rhs);
    virtual ~CGroundDecor();
public:

    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();


private:

    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:

    static  CGroundDecor*    Create(LPDIRECT3DDEVICE9 Device);

protected:
    
    CBillboard*             m_pBillboardCom;

private:
    virtual     void        Free();
};

END