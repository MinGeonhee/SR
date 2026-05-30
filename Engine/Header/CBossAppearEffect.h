#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CBossAppearEffect :
    public CGameObject
{
private:
    explicit CBossAppearEffect(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossAppearEffect(const CGameObject& rhs);
    virtual ~CBossAppearEffect();

public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    HRESULT     Add_Component();

private:
    Engine::CBossAppearMeshTex* m_pBufferCom;
    Engine::CMeshAnimation*     m_pMeshAnimCom;

public:
    static CBossAppearEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void            Free();
};

END