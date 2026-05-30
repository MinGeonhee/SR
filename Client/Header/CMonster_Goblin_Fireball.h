#include "CMonster.h"
#include "CParticleSystem_Sparkle.h"
#include "CParticleSystem_Blood.h"
#include "CParticleSystem_Ice.h"

class CPlayer;
class Engine::CAnimation;
// 세번만에 죽음
// 랜덤하게 앞뒤 좌우로 세번씩 점프하면서 움직임
// 세번 점프 후 공격
// 특정 거리를 유지함
class CMonster_Goblin_Fireball :
    public CMonster
{
private:
    explicit    CMonster_Goblin_Fireball();
    explicit    CMonster_Goblin_Fireball(const CMonster_Goblin_Fireball& rhs);
    explicit    CMonster_Goblin_Fireball(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Goblin_Fireball();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

// 몬스터 패턴용 함수
private:
    void        Chase_Player(const _float& fTimeDelta);
    void        Manage_Jump(const _float& fTimeDelta);
    void        Attack_Fireball(const _float& fTimeDelta);

// 애니메이션 적용
private:
    void        Set_Animation();

// 몬스터랑 플레이어 사이 방향 벡터
private:
    _vec3       m_vFrom_Monster_To_Player = { 0,0,0 };
    void        Get_Vector_Between_Player_Monster();


// 이동 속도
private:
    float       m_fSpeed = 0.5f;

// 이동 방향
private:
    enum MOVING_DIR
    {
        MOVING_DIR_RIGHT = 0,
        MOVING_DIR_LEFT,
        MOVING_DIR_FRONT,
        MOVING_DIR_BACK
    };

    MOVING_DIR m_eCurrDir = MOVING_DIR_FRONT;

// 현재 방향을 기준으로 구한 랜덤한 이동방향 벡터
private:
    void Make_New_Dir();
private:
    _vec3 m_vCurrDirVector = { 0,0,0 };

// 공격 모션 타이머
private:
    float m_fAttackTimer = 0.f;
    float m_fAttackTime = 2.f;

// 점프 하고 몇초 대기하게 할건지
private:
    float m_fMovingIdleTimer = 0.f;
    float m_fMovingIdleTime = 0.7f;

// 점프 카운트
private:
    int m_iCurrJumpCount = 0;
    int m_iMaxJumpCount = 3;

private:
    bool m_bIsBulletFired = false;

private:
    Engine::CWeaponController* m_pWeaponCom;

private:
    CParticleSystem_Sparkle* m_pSparkleParticleSystem = nullptr;


public:
    HRESULT     Add_Component();

public:
    static  CMonster_Goblin_Fireball* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void        Free();

private:
    CParticleSystem_Blood* m_pBloodParticleSystem = nullptr;
    CParticleSystem_Ice* m_pIceParticleSystem = nullptr;

public:
    void    Play_BloodParticleSystem();
    void    Play_IceParticleSystem();
};



