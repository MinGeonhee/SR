#include "CMonster.h"
#include "CParticleSystem_Blood.h"
#include "CParticleSystem_Ice.h"
#include "CBuff.h"

class CPlayer;
class Engine::CAnimation;
// 세번만에 죽음
// 랜덤하게 앞뒤 좌우로 세번씩 점프하면서 움직임
// 세번 점프 후 공격
// 특정 거리를 유지함
class CMonster_Rat :
    public CMonster
{
private:
    explicit    CMonster_Rat();
    explicit    CMonster_Rat(const CMonster_Rat& rhs);
    explicit    CMonster_Rat(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Rat();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void        Chase_Player(const _float& fTimeDelta);
    void        Attack(const _float& fTimeDelta);
    void        Jump(const _float& fTimeDelta);

private:
    _vec3       m_vFrom_Monster_To_Player = { 0,0,0 };
    void        Get_Vector_Between_Player_Monster();
private:
    void        Set_Animation();

public:
    HRESULT                 Add_Component();
public:
    static  CMonster_Rat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void        Free();

private:
    bool m_bIsBulletFired = false;

private:
    Engine::CWeaponController* m_pWeaponCom;

private:
    float       m_fSpeed = 2.f;
    float       m_fJumpDistance_Max = 12.f;
    float       m_fJumpDistance_Min = 8.f;

private:
    enum MOVING_DIR
    {
        MOVING_DIR_RIGHT = 0,
        MOVING_DIR_LEFT,
        MOVING_DIR_FRONT,
        MOVING_DIR_BACK
    };

    MOVING_DIR m_eCurrDir = MOVING_DIR_FRONT;


private:
    float m_fAttackTimer = 0.f;
    float m_fAttackTime = 2.f;

private:
    float m_fMovingIdleTimer = 0.f;
    float m_fMovingIdleTime = 0.7f;

private:
    int m_iCurrJumpCount = 0;
    int m_iMaxJumpCount = 3;



private:
    CParticleSystem_Blood* m_pBloodParticleSystem = nullptr;
    CParticleSystem_Ice* m_pIceParticleSystem = nullptr;

public:
    void    Play_BloodParticleSystem();
    void    Play_IceParticleSystem();

    // 버프 아이콘
private:
    CBuff* m_pBuff;
};




