#pragma once
#include "CGameObject.h"
#include "CRectCol.h"

BEGIN(Engine)

class ENGINE_DLL CToolBufferRect :
    public CGameObject
{
private:

    explicit CToolBufferRect(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CToolBufferRect(const CToolBufferRect& rhs);
    virtual ~CToolBufferRect();

public:

    virtual     HRESULT     Ready_GameObject(const _vec3& vPos);
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void    Change_Color(const _vec4& Color);
    static CToolBufferRect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);
private:
    HRESULT            Add_Component();


private:
    CRectCol* m_pBufferCom;
private:

    virtual                 void Free();
};

END

