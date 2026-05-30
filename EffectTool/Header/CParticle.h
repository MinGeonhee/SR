#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CParticle :
    public CGameObject
{
private:
    explicit    CParticle(const CParticle& rhs);
    explicit    CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CParticle();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT             Add_Component();

public:
    static CParticle*   Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CTransform* Get_Transform() { return m_pTransformCom; }

public:
    void Make_Random_Position(_vec3 _vRange);
    void Make_Random_Size(_vec2 _vRange);
    void Set_Particle_Size_Constant(float _fSize);
    void Set_Particle_Texture_Index(int _iXIndex, int _iYIndex);

private:
    Engine::CParticleRecTex*   m_pBufferCom;
    Engine::CTransform* m_pTransformCom;        // 트랜스폼 컴포넌트

private:
    bool m_bIsRenderOn = false;
public:
    bool Get_isRenderOn() { return m_bIsRenderOn; }
    void Set_isRenderOn(bool _b) { m_bIsRenderOn = _b; }

	float m_fActivationTimer = 0.f; 
    // 생명주기 및 한번에 켜지는 파티클 수
	float m_fLifeTime = 3.f; 

public:
    void Reset_All_Particle_Type_Values();

private:
	
public:
    bool m_bIsSparkling = false;
    void Update_Sparkle(const _float& fTimeDelta);

public:
    bool m_bIsGoingUp = false;
    void Update_Go_Up(const _float& fTimeDelta);

public:
    bool m_bIsDropped = false;
    void Update_Drop(const _float& fTimeDelta);

public:
    float m_fSpeed_GoUp = 1.6f;

public:
	_vec3 m_vDir_Drop = { 0.f, 0.f, 0.f };
    float m_fSpeed_XZ_Drop = 0.6f;
    float m_fSpeed_Y_Drop = 7.f;
	float m_fGravity_Drop = 9.8f;

private:
    virtual void  Free();
};



