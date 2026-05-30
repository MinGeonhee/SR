#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CWeaponController.h"
#include "CSoundManager.h"

BEGIN(Engine)

typedef struct typeMuzzle
{
    CTransform* tTranfrom;      // 현재 무기를 쥐고 있는 플레이어 위치 들고오고 or 카메라
    _vec3       tOffset;        // 어디서 총알을 발사할지 Offset을 주고 총알을 생성할 위치 설정.

}MUZZLE;

class CBullet;

class ENGINE_DLL IWeaponState :
    public CBase
{
public:
    explicit IWeaponState() {};
    virtual ~IWeaponState() {};

public:
    virtual void Initialize(CWeaponController* pWeapon) = 0;  // 초기에 생성할 값들. 지우는 형태가 아니라 교체 형태니까 클래스가 살아있다.
    virtual void Enter(CWeaponController* pWeapon) = 0;
    virtual void Update(CWeaponController* pWeapon, float fDeltaTime) = 0;
    virtual void Exit(CWeaponController* pWeapon) = 0;

    virtual void            Fire(CWeaponController* pWeapon) = 0;         // 내가 설정해둔 Muzzle 위치에서 바라보는 방향으로 발사.
    virtual void            ChargeFire(CWeaponController* pWeapon) = 0;   // 기본 무기 차징 공격
    virtual void            Overdrive_Fire(CWeaponController* pWeapon) = 0;   // 스페셜 무기들 강화 공격

public:

    // 총알이 다 고갈되었나요. bool 값으로 무기 교체 가능 여부 알아봄.
    virtual bool            AmmoDepleted() { return m_bAmmoDepleted; }

    virtual bool            CanPickupBullet() // 현재 내 총알이 최대치 미만이라면 주울 수 있음. true 반납.
    {return (m_iCurrent_BulletCount < m_iMax_BulletCount);}

    virtual uint32_t        Get_BulletCount(){ return m_iCurrent_BulletCount; }
    virtual void            ADD_BulletCount(int count) 
    { 
        if (m_iMax_BulletCount > m_iCurrent_BulletCount)
            m_iCurrent_BulletCount += count;
    }
    virtual void            Dicrease_Bullet(int count)
    {
        m_iCurrent_BulletCount -= count;
    }

    // 기본무기 한정
    virtual bool            CanPickupEnergyBullet() // 현재 내 에너지가 최대치 미만이라면 주울 수 있음. true 반납.
    {
        return (m_iCurrent_EnergyBulletCount < m_iMax_EnergyBulletCount);
    }
    virtual uint16_t        Get_EnergyBulletCount() { return m_iCurrent_EnergyBulletCount; }
    virtual void            ADD_EnergyBullet(int count) 
    {
        if (m_iMax_EnergyBulletCount > m_iCurrent_EnergyBulletCount)
            m_iCurrent_EnergyBulletCount += count;
    }

    virtual void            Dicrease_EnergyBullet(int count)
    {
        m_iCurrent_EnergyBulletCount -= count;
    }

    virtual void            Level_Up() { m_iWeaponLevel++; }

public:

    void                    Set_OverideFire(bool b) { m_bCanOverDrive = b; }
    bool                    Get_OverideFire() { return m_bCanOverDrive; }
    virtual bool            CanChargeFire(const float& fDeltaTime);
    virtual void            ChargeElapsedTime_reset() { m_fChargeElapsedTime = 0.f; }
protected:

    virtual void            IWeapon_Update(const float& fDeltaTime);
    virtual void            CalculateFireDirection(const _vec3& vPos, _vec3& vDir); // 총알이 나아가 할 방향을 구해다 주는 함수.
    virtual void            MonsterBulletDirCalculator(CWeaponController* pWeapon, _vec3& Dir, const _vec3& vPos);
    virtual void            Publish_AmmoDepletedEvent();  // 해당 타입의 무기 총알이 고갈이 되면 Event bus에 고갈되었음을 알림.
    virtual void            Clamp_BulletCount();
    // 총 쏠 수 있는 상태니.
    virtual void            CanFire(const float& fDeltaTime);

    // 차지 Fire 쏠 수 있는 상태니.

    // 레벨 2가 되지 않으면 우클릭 스킬 불가능.
    virtual void            Skill_Lock();

    // 맨 위 include 아래 아무 데나
    virtual void             GetCameraBasis(_vec3& outFwd, _vec3& outUp, _vec3& outRight);
 


protected:

    uint16_t                    m_iMax_BulletCount = 0;  // 최대로 보관 가능한 총알 
    uint16_t                    m_iCurrent_BulletCount = 0; // 남아있는 총알 갯수
    vector<MUZZLE>              m_vMuzzleList;      // 내 MuzzleList

    // 기본 무기 한정
    uint16_t                    m_iMax_EnergyBulletCount = 0;
    uint16_t                    m_iCurrent_EnergyBulletCount = 0; // 에네지탄 카운트

    // 무기 레벨 
    uint8_t                     m_iWeaponLevel = 1;

    bool                        m_bWeaponSkill = false;  // 무기 스킬 쓸 수 있니
    bool                        m_bAmmoDepleted = false; // 총알 고갈 됐니?
    bool                        m_bCanFire = false;
    bool                        m_bCanChargeFire = false;
    bool                        m_bCanOverDrive = true; // 특수 무기 특수 탄 쏠 수 있는지.
    
    float                       m_fFireRate = 0.f; // 무기 발사 속도.
    float                       m_FChargeFireRate = 0.f; // 무기 차지 속도.
    float                       m_fChargeElapsedTime = 0.f;
    float                       m_fElapsedTime = 0.f; // 시간을 담아둘 변수.

private:

    virtual void                Free() 
    { 
        m_vMuzzleList.clear();  // 안에 있는 tranform 지우면 큰일남.
    }
};

END
