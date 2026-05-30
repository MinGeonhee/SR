#pragma once
#include "CParticleSystem.h"
#include "CProtoTypeManager.h"
#include "CParticle.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem_Rain :
    public CParticleSystem
{
private:
    explicit    CParticleSystem_Rain(const CParticleSystem_Rain& rhs);
    explicit    CParticleSystem_Rain(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticleSystem_Rain();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();


public:
    static CParticleSystem_Rain* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
    float                   m_fSpeed_Rain = 7.f;
    float                   m_fElapsedTime = 0.f;

public:
    void Insert_Particles();
    void Apply_Values_To_Particle(CParticle* _p);

    void Particle_Update_Rain(const _float& fTimeDelta);
	

private:
    virtual void  Free();
};

END




