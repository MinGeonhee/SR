#pragma once
#include "CMapToolMapObject.h"

class CMapToolObstacle :
    public CMapToolMapObject
{

private:
    explicit CMapToolObstacle(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMapToolObstacle(const CMapToolObstacle& rhs);
    virtual ~CMapToolObstacle();
public:

    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();


private:

    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:

    static  CMapToolObstacle* Create(LPDIRECT3DDEVICE9 Device);

private:
    virtual     void        Free();
};

