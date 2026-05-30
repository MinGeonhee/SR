#pragma once
#include "CMapObject.h"

BEGIN(Engine)

class CBillboard;

class ENGINE_DLL CObstacle final:
    public CMapObject
{
private:
    explicit CObstacle(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CObstacle(const CObstacle& rhs);
    virtual ~CObstacle();


private:
    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Compoenet();

public:
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    static      CObstacle*  Create(LPDIRECT3DDEVICE9 Device);

protected:
    CBillboard*             m_pBillboardCom;

private:
    virtual void            Free();
};

END
