#include "CMonster.h"
#include "CParticleSystem_Blood.h"
#include "CBossAppearEffect.h"
//#include "CMonster_Boss.h"

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

class ENGINE_DLL CMonster_Clone_Boss :
    public CMonster
{
private:
    explicit    CMonster_Clone_Boss();
    explicit    CMonster_Clone_Boss(const CMonster_Clone_Boss& rhs);
    explicit    CMonster_Clone_Boss(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Clone_Boss();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        Set_Animation();


// 이동 속도
private:
    float       m_fSpeed = 0.f;

public:
    static  CMonster_Clone_Boss* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    HRESULT     Add_Component();

private:
    virtual void        Free();


// 공격 시간 인터벌 변수
private:
    float       m_fAttackTimer = 0.f;
    float       m_fAttackTime = 0.5f;


private:
    
    _vec2       m_vRandomPosRange_XZ = { 20.f,20.f };
public:
    void        Move_To_Random_Pos();

private:
    CGameObject* m_pTargetBoss = nullptr;
public:
    void Set_TargetBoss(CGameObject* _p) { m_pTargetBoss = _p; }

// 보스 공격 패턴용 웨폰 컴포넌트
private:
    Engine::CWeaponController* m_pIceBallWeaponCom;

};

END



