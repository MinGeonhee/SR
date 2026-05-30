#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CTerrain :
    public CGameObject
{
private:
    explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTerrain(const CGameObject& rhs);
    virtual ~CTerrain();

public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    HRESULT     Add_Component();
    void        Key_Input(const _float& fTimeDelta);

private:
    Engine::CTerrainTex*        m_pBufferCom;

public:
    static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void            Free();
};

