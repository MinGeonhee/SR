#include "CMonster.h"
#include "CParticleSystem_Bone.h"
#include "CParticleSystem_Ice.h"
#include "CBuff.h"

class CPlayer;

// 세번만에 죽음
// 가까워지고, 특정 거리 이내면 계속 때림
class CMonster_Skeleton_Sword :
    public CMonster
{
private:
    explicit    CMonster_Skeleton_Sword();
    explicit    CMonster_Skeleton_Sword(const CMonster_Skeleton_Sword& rhs);
    explicit    CMonster_Skeleton_Sword(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CMonster_Skeleton_Sword();

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
    static  CMonster_Skeleton_Sword* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    HRESULT     Add_Component();

private:
    virtual void        Free();

private:
    bool m_bIsBulletFired = false;

private:
    Engine::CWeaponController* m_pWeaponCom;



private:
    CParticleSystem_Bone* m_pBoneParticleSystem = nullptr;
    CParticleSystem_Ice* m_pIceParticleSystem = nullptr;

public:
    void    Play_BoneParticleSystem();
    void    Play_IceParticleSystem();

    // 버프 아이콘
private:
    CBuff* m_pBuff;
};




