#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"
#include "CMonster.h"
#include "CParticleSystem_Blood.h"
#include "CBossAppearEffect.h"
#include "CBossSystem.h"

BEGIN(Engine)


//보스 
//0. 기본적으로 둥실둥실 움직임
//1. 첫 등장때 나오는 번개 광선(이건 정점 찍어서 해야할듯..)
//2. 쉴드
//3. 사방향 방사 공격(바닥 메쉬..이걸어케함?)
//4. 토네이도 공격
//5. 번개 공격
//6. 손에서 번개 공격
//7. 순간이동하며 얼음결정 공격
//8. 분신술하며 동그란 구슬? 던지는 공격
//9. 죽을때 DYING 애니메이션과 함께 피 개많이 쏟으면서 아이템 쏟고, 그리고 팡

class ENGINE_DLL CMonster_Boss :
    public CMonster
{
private:
    explicit    CMonster_Boss();
    explicit    CMonster_Boss(const CMonster_Boss& rhs);
    explicit    CMonster_Boss(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Boss();

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
    static  CMonster_Boss* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    HRESULT     Add_Component();

private:
    virtual void        Free();

// 보스 패턴 
// 페이즈별로 나열
private:
    enum BOSS_ATTACK
    {
        //BOSS_ATTACK_ICEBALL = 0,
        BOSS_ATTACK_THUNDER_DROP = 0,
        BOSS_ATTACK_WIND,
        BOSS_ATTACK_ICEPILLAR,
        BOSS_ATTACK_ONE_END,

        BOSS_ATTACK_TELEPORT,
        BOSS_ATTACK_CLONE,
        BOSS_ATTACK_TWO_END,
    };

    BOSS_ATTACK m_eCurrBossAttack = BOSS_ATTACK_THUNDER_DROP;
    float       m_fBossAttackChangeTimer = 0.f;
    float       m_fBossAttackChangeInterval = 5.f;

// 시간 지남에 따라 공격 패턴 바꿔주는 함수
private:
    void        Change_Attack();


// 보스 페이즈 
private:
   
    BOSS_PHASE m_eCurrPhase = BOSS_PHASE_ONE;

// 보스 체력에 따라 페이즈 바꿔주는 함수 
private:
    void       Phase_Check();

// 페이즈별 공격 함수
private:
    void 	    Attack_Phase_One(const _float& fTimeDelta);
    void 	    Attack_Phase_Two(const _float& fTimeDelta);

// 공격 시간 인터벌 변수
private:
    float       m_fAttackTimer = 0.f;
    float       m_fAttackTime = 0.5f;


// 텔레포트 공격패턴용 함수
private:
    void        Teleport_Attack(const _float& fTimeDelta);
    void        Phase_Update();

private:
    void        Move_To_Random_Pos();
    _vec2       m_vRandomPosRange_XZ = { 14.f,14.f };

private:
    float       m_fTeleportTimer = 0.f;
    float       m_fTeleportInterval = 3.f;

private:
    void        Clone_Attack(const _float& fTimeDelta);
    int         m_iCurrCloneBossNum;
    int         m_iMaxCloneBossNum = 5;
    bool        m_bIsClonedBoss = false;
public:
    int         Get_CurrCloneBossNum() { return m_iCurrCloneBossNum; }
    void         Minus_CurrCloneBossNum() { m_iCurrCloneBossNum -= 1; }

// 피 이펙트
private:
    CParticleSystem_Blood* m_pBloodParticleSystem = nullptr;

public:
    void    Play_BloodParticleSystem();
    void    Drop_Item(const _float& fTimeDelta);

private:
    float   m_fItemTimer = 0.f;
    float   m_fItemSpawnTimeInterval = 0.1f;
    int     m_iItemSpawnNum = 20;
    int     m_iItemSpawnCurrNum = 0;
    _bool   m_bEventpublish = false;


// 보스 공격 패턴용 웨폰 컴포넌트
private:
    Engine::CWeaponController* m_pIceBallWeaponCom;
    Engine::CWeaponController* m_pIcePillarWeaponCom;
    Engine::CWeaponController* m_pThunderDropWeaponCom;
    Engine::CWeaponController* m_pWindWeaponCom;
 
private:

    // 베리어한테 보내주어야할 컴포넌트.
    Engine::CTransform*        m_pBarrierTransform;
    Engine::CAnimation*        m_pBarrierAnimantion;

// 보스 등장 이펙트
private:
    vector<CBossAppearEffect*> m_vecBossAppearEffect;
    int                        m_iAppearEffectNum = 4;

    bool                        m_bTurnOffAppearEffect_Trigger = false;
    int                         m_iCurrTurnOffAppearEffect = 0;
    float                       m_fTurnOffTimer = 0.f;
    float                       m_fTurnOffInterval = 0.6f;

public:
    void                        TurnOff_AppearEffect(const _float& fTimeDelta);
    void                        Get_TurnOffAppearEffect_Trigger(bool _b) { m_bTurnOffAppearEffect_Trigger = _b; }

    enum BOSS_APPEAR_EFFECT
    {
        BOSS_APPEAR_EFFECT_R_BACK = 0,
        BOSS_APPEAR_EFFECT_R_FRONT,
        BOSS_APPEAR_EFFECT_L_FRONT,
        BOSS_APPEAR_EFFECT_L_BACK,
    };
};




END