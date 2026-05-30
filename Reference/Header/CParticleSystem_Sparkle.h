#pragma once
#include "CParticleSystem.h"
#include "CProtoTypeManager.h"
#include "CParticle.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem_Sparkle :
    public CParticleSystem
{
private:
    explicit    CParticleSystem_Sparkle(const CParticleSystem_Sparkle& rhs);
    explicit    CParticleSystem_Sparkle(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticleSystem_Sparkle();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();


public:
    static CParticleSystem_Sparkle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    // 스파클 생명주기
    //private:
    //    float m_fLifeTime_Sparkle = 1.f;

public:
    void Apply_Values_To_Particles();

public:
    void Particle_Update_Sparkle(const _float& fTimeDelta);

private:
    bool    m_bParticleSystemOff = false;
public:
    void    Set_ParticleSystemOff(bool _b) { m_bParticleSystemOff = _b; }
    bool    Get_ParticleSystemOff() { return m_bParticleSystemOff; }

private:
    virtual void  Free();
};

END





