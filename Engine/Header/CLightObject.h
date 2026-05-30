#pragma once
#include "CGameObject.h"



namespace Engine { class CAnimation; }  // 네임스페이스 안에도 있음

BEGIN(Engine)

class ENGINE_DLL CLightObject :
    public CGameObject
{
protected:
    explicit    CLightObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit    CLightObject(const CLightObject& cpy);
    virtual     ~CLightObject();

public:

    virtual     HRESULT      Ready_GameObject();
    virtual     _int         Update_GameObject(const _float& fTimeDelta) override;
    virtual     void         LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual     void         Render_GameObject() override;

    static  CLightObject*    Create(LPDIRECT3DDEVICE9 pGraphicDev);

    virtual void             SetSubType(void* eType) { m_eLight_Object_Type = *static_cast<LIGHT_OBJECT_TYPE*>(eType); }

public:

    void                     Set_LightObjectType(const LIGHT_OBJECT_TYPE& eType) { m_eLight_Object_Type = eType; }

public:

    const LIGHT_OBJECT_TYPE& Get_LightObjectType() { return m_eLight_Object_Type; }

public:

    void            Setting(const ObjectHandle& tHandle , const _vec3 vPos , const _vec3& vRot, const _vec3& vScale , const MAPOBJECT_PLACEMENT& ePlacement);

protected:
    HRESULT         Add_Component();
    virtual void    Free();

protected:
    LIGHT_OBJECT_TYPE       m_eLight_Object_Type;
    MAPOBJECT_PLACEMENT     m_ePlacementType;
    Engine::CAnimation*     m_pAnimationCom;
    bool                    m_isEvent;

};

END