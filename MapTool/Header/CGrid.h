#pragma once
#include "CGameObject.h"
#include "CGridCol.h"
#include "CCenterLineCol.h"

class CGrid :
    public CGameObject
{
private:
    //explicit    CGrid();
    explicit    CGrid(const CGrid& rhs);
    explicit    CGrid(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CGrid();

    virtual void  Free();

public:
    virtual     HRESULT         Ready_GameObject(const _int& iFloor);
    virtual     _int            Update_GameObject(const _float& fTimeDelta);
    virtual     void            LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void            Render_GameObject();

    static CGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _int& iFloor = FLOOR::FLOOR_GROUND);

private:

    HRESULT                 Add_Component();


private:
    Engine::CGridCol* m_pBufferCom;
    Engine::CCenterLineCol* m_pCenterBufferCom;
};

