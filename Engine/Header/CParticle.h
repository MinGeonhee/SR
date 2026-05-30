#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CParticle :
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
    virtual CTransform* Get_Transform() { return m_pTransform; }

public:
    void Make_Random_Position(_vec3 _vRange, _vec3 _vPos);
    void Make_Random_Position_XYZ(_vec3 _vRange, _vec3 _vPos);
    void Make_Random_Size(_vec2 _vRange);
    void Set_Particle_Size_Constant(float _fSize);
    void Set_Particle_Texture_Index(int _iXIndex, int _iYIndex);
    void Set_Collider_Type(COLLIDERTYPE _e);

private:
    Engine::CParticleRecTex*            m_pBufferCom;
    Engine::CParticleRecTex_Alpha*      m_pAlphaBufferCom;
    Engine::CParticleOnFloorRecTex*     m_pBufferOnFloorCom;
    Engine::CBillboard*                 m_pBillboardCom;
    Engine::CAnimation*                 m_pAnimationCom;

private:
	float m_fActivationTimer = 0.f; 

private:
    int m_iTextureIndex_X = 0;
    int m_iTextureIndex_Y = 0;
public:
    int Get_TextureIndex_X() { return m_iTextureIndex_X; }


public:
    void Reset_All_Particle_Type_Values();

// 파티클 상태
private:
    PARTICLE_STATUS m_eParticleStatus = PARTICLE_STATUS_INACTIVE;
public:
    void Set_ParticleStatus(PARTICLE_STATUS _e) { m_eParticleStatus = _e; }
	PARTICLE_STATUS Get_ParticleStatus() const { return m_eParticleStatus; }

// 파티클 타입
// 파티클 테스트
private:
    PARTICLE_TYPE m_eParticleType = PARTICLE_TYPE_TEST;
public:
	void Set_ParticleType(const PARTICLE_TYPE& eType) { m_eParticleType = eType; }
	PARTICLE_TYPE Get_ParticleType() const { return m_eParticleType; }

// 파티클 스파클
// 스파클 생명주기
private:
    float m_fLifeTime_Sparkle = 1.f;
public:
    void Update_Sparkle(const _float& fTimeDelta);


// 파티클 고업
// 올라가는 파티클 속도
public:
    float m_fSpeed_GoUp = 1.6f;
    float m_fLifeTime_GoUp = 3.f;
public:
    void Update_Go_Up(const _float& fTimeDelta);


// 파티클 드랍
// 드랍 파티클 관련
private:
    bool m_bIsDropped = false;
    bool m_bIsOnFloor = false;
public:
	_vec3 m_vDir_Drop = { 0.f, 0.f, 0.f };
    float m_fSpeed_XZ_Drop = 0.6f;
    float m_fSpeed_Y_Drop = 7.f;
	float m_fGravity_Drop = 9.8f;

    float m_fLifeTime_Drop = 5.f;
    float m_fSpeed = 1.5f;

public:    
    void Update_Drop(const _float& fTimeDelta);
    void Update_Drop_On_Floor(const _float& fTimeDelta);
public:
	void Set_IsOnFloor(const bool& b) { m_bIsOnFloor = b; }
	bool Get_IsOnFloor() const { return m_bIsOnFloor; }


// 파티클 스모크
private:
    float m_fLifeTime_Smoke = 0.4f;
public:
    void Update_Smoke(const _float& fTimeDelta);
    void Set_AlphaBufferColor(float r, float g, float b);

// 파티클 init 스모크
public:
    void Update_Init_Smoke(const _float& fTimeDelta);

// 파티클 비
// 비 생명주기
public:
    void Update_Rain(const _float& fTimeDelta);
    float m_fSpeed_Rain = 2.f;



// 파티클 충돌 관련 세팅
private:
    bool m_bIsCollisionOn = false;
public:
    void Set_IsCollisionOn(bool _b) { m_bIsCollisionOn = _b; }


public:
    void Set_ColliderOffset(float _y) { m_pColliderCom->Set_Offset({ 0.f,_y,0.f }); }

public:
    void Change_OnFloorType(float _yPos);

public: 
    void Set_Animation();

    void Reset_And_Return();

private:
    virtual void  Free();
};



END