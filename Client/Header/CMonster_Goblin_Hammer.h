#include "CMonster.h"
#include "CParticleSystem_Blood.h"
#include "CParticleSystem_Ice.h"
#include "CBuff.h"


class CPlayer;

// 세번만에 죽음
// 가까워지고, 특정 거리 이내면 계속 때림
class CMonster_Goblin_Hammer :
    public CMonster
{
private:
    explicit    CMonster_Goblin_Hammer();
    explicit    CMonster_Goblin_Hammer(const CMonster_Goblin_Hammer& rhs);
    explicit    CMonster_Goblin_Hammer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Goblin_Hammer();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void        Chase_Player(const _float& fTimeDelta);
private:
    void        Set_Animation();
private:
    void 	    Attack();

// 이동 속도
private:
    float       m_fSpeed = 0.f;

public:
    static  CMonster_Goblin_Hammer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    HRESULT     Add_Component();

private:
    virtual void        Free();

private:
    bool m_bIsBulletFired = false;

private:
    Engine::CWeaponController* m_pWeaponCom;


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




