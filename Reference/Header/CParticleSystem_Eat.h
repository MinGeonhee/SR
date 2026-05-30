#pragma once
#include "CParticleSystem.h"
#include "CProtoTypeManager.h"
#include "CParticle.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem_Eat :
    public CParticleSystem
{
private:
    explicit    CParticleSystem_Eat(const CParticleSystem_Eat& rhs);
    explicit    CParticleSystem_Eat(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticleSystem_Eat();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();


public:
    static CParticleSystem_Eat* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
    float m_fLifeTime_GoUp = 3.f;
    float m_fActivationTimer = 0.f;


public:
    void Make_And_Apply_Values_To_Particle();

  

private:
    virtual void  Free();

};

END