#include "CMonster.h"
#include "CParticleSystem_Blood.h"
#include "CParticleSystem_Ice.h"

class CPlayer;

// 세번만에 죽음
// 가까워지고, 특정 거리 이내면 계속 때림
class CMonster_Wizard :
    public CMonster
{
private:
    explicit    CMonster_Wizard();
    explicit    CMonster_Wizard(const CMonster_Wizard& rhs);
    explicit    CMonster_Wizard(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Wizard();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();


private:
    void        Set_Animation();


// 이동 속도
private:
    float       m_fSpeed = 0.f;

public:
    static  CMonster_Wizard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    HRESULT     Add_Component();

private:
    virtual void        Free();

private:
    bool m_bIsBulletFired = false;

private:
    Engine::CWeaponController* m_pWeaponCom;


// 중간보스 패턴 
private:
    enum WIZARD_ATTACK
    {
        WIZARD_ATTACK_FIREBALL,
        WIZARD_ATTACK_BUFF,
        WIZARD_ATTACK_END,
    };

    WIZARD_ATTACK m_eCurrAttack = WIZARD_ATTACK_FIREBALL;
    float         m_fAttackChangeTimer = 0.f;
    float         m_fAttackChangeInterval = 10.f;

// 시간 지남에 따라 공격 패턴 바꿔주는 함수
private:
    void        Change_Attack_To_Buff(const _float& fTimeDelta);

// 파이어볼 용
private:
    void 	    Attack_Fireball(const _float& fTimeDelta);
private:
    void        Chase_Player(const _float& fTimeDelta);

// 공격 시간 인터벌 변수
private:
    float m_fAttackTimer = 0.f;
    float m_fAttackTime = 1.25f;

// 버프 공격 용
private:
    void 	    Attack_Buff(const _float& fTimeDelta);
    void        Move_Up(const _float& fTimeDelta);
    void        Move_Down(const _float& fTimeDelta);
    void        BuffMonsters();
    void        DebuffMonsters();

private:
    float m_fMaxPosY = 2.7f;
    float m_fMinPosY = 2.f;
    float m_fSpeed_UpDown = 0.4f;
    bool m_bIsUp = false;
    _bool m_bMonsterSpawn = false;

private:
    CParticleSystem_Blood* m_pBloodParticleSystem = nullptr;
    CParticleSystem_Ice* m_pIceParticleSystem = nullptr;

public:
    void    Play_BloodParticleSystem();
    void    Play_IceParticleSystem();



};




