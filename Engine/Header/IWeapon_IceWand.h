#pragma once
#include "IWeaponState.h"

BEGIN(Engine)

class ENGINE_DLL IWeapon_IceWand :
    public IWeaponState
{
public:
    explicit IWeapon_IceWand() {};

public:
    virtual void Initialize(CWeaponController* pWeapon) override;
    virtual void Enter(CWeaponController* pWeapon) override;
    virtual void Update(CWeaponController* pWeapon, float fDeltaTime) override;
    virtual void Exit(CWeaponController* pWeapon) override;

public:

    virtual void            Fire(CWeaponController* pWeapon) override;         // 내가 설정해둔 Muzzle 위치에서 바라보는 방향으로 발사.
    virtual void            ChargeFire(CWeaponController* pWeapon) override {};   // 기본 무기가 아니라서 필요없음.
    virtual void            Overdrive_Fire(CWeaponController* pWeapon) override;   // 특수 무기라서 Overide 스킬을 사용.
};

END