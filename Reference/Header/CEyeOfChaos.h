#pragma once
#include "CEventObject.h"


namespace Engine
{
    class CEyeOfChaos_Eye;
    class CAnimation;
};

BEGIN(Engine)



class ENGINE_DLL CEyeOfChaos final:
    public CEventObject
{
public:

    explicit CEyeOfChaos(LPDIRECT3DDEVICE9 Device);
    explicit CEyeOfChaos(const CEyeOfChaos& cpy);
    virtual ~CEyeOfChaos();

private:
    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();

public:

    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    static      CEyeOfChaos* Create(LPDIRECT3DDEVICE9 Device);

public:

    void        SetEye(CEyeOfChaos_Eye* pEye)       { NULLPTR_RETURN(pEye);   m_pEye = pEye; }

public:

    CEyeOfChaos_Eye* GetEye()       { return m_pEye; }
    
    void        EyeEvent();

private:

    CEyeOfChaos_Eye * m_pEye;       
    CAnimation*       m_pAnimationCom;

private:
    // CEventObject을(를) 통해 상속됨
    void Setting_EventObject(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale, const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const EVENT_PATH& EventPath) override;



private:
    virtual void    Free() override;
};

END