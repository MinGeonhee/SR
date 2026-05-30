#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class CColliderBase;

class ENGINE_DLL CBullet :
    public CGameObject
{
public:

    explicit CBullet();
    explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBullet(const CBullet& rhs);
    virtual ~CBullet();

    // Bullet을 어차피 PoolingManager가 들고 있을거니까. 이 놈의 타입을 바꿔주기만 하면 그거에 맞는 로직으로 돌아가도록 설계
public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

public:

    void               Change_BulletState(BULLETSTATE state);
    BULLETSTATE        Get_BulletState() { return m_eBulletState; }
    void               Set_BulletDir(_vec3 vDir) { m_vBulletDir = vDir; }
    _vec3              Get_BulletDir() { return m_vBulletDir; }
    void               Set_BulletStatus(BULLET_STATUS stat) { m_eBulletStatus = stat; }
    void               Reset_ElapsedTime() { m_fElapsedTime = 0.f; }

    static CBullet*    Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT            Add_Component();
    void               Tick_Lifetime(const float& fTimeDelta);
    void               Bullet_Texture_Setting();
    void               Set_Animation();
    void               Move(const float& fTimeDelta);
    

    // 근접 공격은 이 함수를 통해 Move, Render가 제거되어야함.
    bool               IsBulletTypeExempt();

    void               Make_Smoke_Trail_Particle();

    virtual void       Free() override;

private:

    Engine::CAnimation*             m_pAnimationCom;
    Engine::CBillboard*             m_pBillboardCom;

private:

    BULLETSTATE             m_eBulletState = BULLETSTATE::BULLET_NONE; // 초기 설정값 NONE
    _vec3                   m_vBulletDir{};

    BULLET_STATUS           m_eBulletStatus = BULLET_STATUS::BULLET_STATUS_IDLE;
    BULLET_STATUS           m_ePrevBulletStatus;
    BULLETSTATE             m_ePrevBulletState;

    float                   m_fLifeTime = 3.f;      // 총알 생명주기 3초
    float                   m_fElapsedTime = 0.f;
private:
    void                    Spawn_Random_Pos();
    _vec2                   m_vRandomPosRange_XZ = { 15.f,15.f };
};

END