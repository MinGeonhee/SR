#pragma once
#include "CGameObject.h"


namespace Engine
{
    class CAnimation;
};



BEGIN(Engine)

class ENGINE_DLL CEyeOfChaos_Eye :
  public CGameObject
    
{
public:

    explicit CEyeOfChaos_Eye(LPDIRECT3DDEVICE9 Device);
    explicit CEyeOfChaos_Eye(const CEyeOfChaos_Eye& cpy);
    virtual ~CEyeOfChaos_Eye();

public:

    void        MoveLogic(float DT);

public:

    void        SetControlPoint(float x, float y , float z);


private:
    virtual     HRESULT     Ready_GameObject();
    HRESULT                 Add_Component();
    virtual     void        Free();
public:
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    static      CEyeOfChaos_Eye* Create(LPDIRECT3DDEVICE9 Device);
  
private:

    CAnimation*             m_pAnimationCom;
    MAPOBJECT_PLACEMENT     m_ePlacementType;

    _vec3                   m_vControlPos;
};

END