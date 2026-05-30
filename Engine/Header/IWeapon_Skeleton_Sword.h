#pragma once
#include "IWeaponState.h"

BEGIN(Engine)

class ENGINE_DLL IWeapon_Skeleton_Sword :
    public IWeaponState
{
public:
    explicit IWeapon_Skeleton_Sword() {};

public:
    virtual void Initialize(CWeaponController* pWeapon) override;
    virtual void Enter(CWeaponController* pWeapon) override;
    virtual void Update(CWeaponController* pWeapon, float fDeltaTime) override;
    virtual void Exit(CWeaponController* pWeapon) override;

public:

    virtual void            Fire(CWeaponController* pWeapon) override;         // 내가 설정해둔 Muzzle 위치에서 바라보는 방향으로 발사.
    virtual void            ChargeFire(CWeaponController* pWeapon) override {};    // 몬스터는 그런거 없어요
    virtual void            Overdrive_Fire(CWeaponController* pWeapon) override {};

};
END