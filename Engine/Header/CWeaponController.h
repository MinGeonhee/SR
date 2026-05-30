#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;
class IWeaponState;
class CGameObject;

class ENGINE_DLL CWeaponController :
    public CComponent
{
private:
    explicit CWeaponController();
    explicit CWeaponController(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CWeaponController(const CWeaponController& rhs);
    virtual ~CWeaponController();

    // 현재 내가 가지고 있는 무기에 따라서 Status가 달라져야 한다.
    // 현재 내가 가지고 있는 무기에 따라서 bullet 생성이 달라져야 한다.
public:

    void            Weapon_Inintialize();   // 반드시 Player 쪽에서 해주어야 할 것.
    HRESULT         Ready_Weapon();
    virtual _int    Update_Component(const _float& fTimeDelta) override;
    virtual void    LateUpdate_Component(const _float& fTimeDelta) override;

public:
    void            Change_Weapon(WEAPONSTATE eState);

    void            Fire(); // 발사 함수.
    void            ChargeFire(const float& fTimeDelta);
    void            Overdrive_Fire();
    int            Get_Bullet();
    int            Get_Bullet(WEAPONSTATE eState);

    bool            CanPickUpBullet(ITEMTYPE type); // 나 지금 해당 Bullet Item 주울 수 있니.
    void            Add_WeaponBullet(ITEMTYPE type, int count);

    void            Set_FireRateScale(float Scale) { m_fFireRate *= Scale; }
    void            LevelUp(WEAPONSTATE tag);

    map<WEAPONSTATE, IWeaponState*>& Get_WeaponState() { return m_mWeapon; }

    WEAPONSTATE Get_WeaponType() { return m_eWeaponState; }

public:
    virtual CComponent* Clone();
    static  CWeaponController* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    virtual void Free();

private:

    IWeaponState*                       m_pCurstate;           // 현재 내 무기 상태.
    WEAPONSTATE                         m_eWeaponState;        // 현재 내 상태.
    map<WEAPONSTATE, IWeaponState*>     m_mWeapon;         // 현재 내 무기 타입에 맞게 호출해서 출력함.

    float                               m_fFireRate;           // 발사 속도 제한 걸기.
    float                               m_fElapsedTime;        // 시간 저장 함수
};
END
