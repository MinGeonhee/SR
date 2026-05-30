#pragma once
#include "CGameObject.h"
#include "CCubeCol.h"

BEGIN(Engine)

class ENGINE_DLL CToolBufferCube :
    public CGameObject
{
private:

    explicit CToolBufferCube(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CToolBufferCube(const CToolBufferCube& rhs);
    virtual ~CToolBufferCube();

public:

    virtual     HRESULT     Ready_GameObject(const _vec3& vPos);
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void                    ChangeColor(_vec4 Color);

    static CToolBufferCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
private:
    HRESULT            Add_Component();


private:
    CCubeCol* m_pBufferCom;
private:

    virtual                 void Free();
};

END