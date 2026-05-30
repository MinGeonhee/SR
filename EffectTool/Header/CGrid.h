#pragma once
#include "CBase.h"
#include "CGraphicDev.h"
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CGrid :
    public CGameObject
{
private:
    explicit CGrid(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CGrid(const CGameObject& rhs);
    virtual ~CGrid();

public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    HRESULT     Add_Component();
  
private:

    int                   m_iFloor;

    Engine::CTerrainCol* m_pBufferCom;
    Engine::CTransform* m_pTransformCom;

public:
    static CGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void            Free();
};


