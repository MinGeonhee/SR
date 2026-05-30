#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"
#include "CParticle.h"
#include "CBoundingBox.h"

class CParticleSystem :
    public CGameObject
{
private:
    explicit    CParticleSystem(const CParticleSystem& rhs);
    explicit    CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticleSystem();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();

public:
    static CParticleSystem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CTransform*     Get_Transform() { return m_pTransformCom; }

private:
    Engine::CTransform*     m_pTransformCom;
   
private:
    vector<CParticle*>      m_vecParticles; 
	CBoundingBox*           m_pBoundingBox; 

private:
    int m_iParticleNum = 1;
    int m_iMaxParticleNum = 50;

private:
    int m_iParticleTextureIndex_X = 0;
    int m_iParticleTextureIndex_Y = 0;

private:
	float m_fParticleTimer = 0.f;

private:
    int m_iParticleType = 0;

public:
    void Apply_New_Particle_Values();
    void Apply_New_Texture();

public:
    void Particle_Update_Test(const _float& fTimeDelta);
    void Particle_Update_Sparkle(const _float& fTimeDelta);
	void Particle_Update_GoUp(const _float& fTimeDelta);
    void Particle_Update_Drop(const _float& fTimeDelta);
    void Particle_Update_Flies(const _float& fTimeDelta);
	

private:
    virtual void  Free();
};





