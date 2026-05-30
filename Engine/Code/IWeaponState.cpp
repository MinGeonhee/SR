#include "IWeaponState.h"
#include "CEventBus.h"
#include "CCameraManager.h"
#include "CInputManager.h"
#include "CGraphicDev.h"
#include "CTransform.h"

void IWeaponState::IWeapon_Update(const float& fDeltaTime)
{
    Clamp_BulletCount();
    Skill_Lock();
    CanFire(fDeltaTime);
}

void IWeaponState::Clamp_BulletCount()
{
    if (m_iCurrent_EnergyBulletCount <= 0)
        m_iCurrent_EnergyBulletCount = 0;

    if (m_iCurrent_BulletCount <= 0)
    {
        m_iCurrent_BulletCount = 0;
        m_bAmmoDepleted = true;
    }

    if (m_iCurrent_BulletCount > 0)
    {
        m_bAmmoDepleted = false;
    }
}

void IWeaponState::Publish_AmmoDepletedEvent()
{
    if (m_bAmmoDepleted)
    {
        EVENT event
        {
            PHASE::PHASE_BEGIN,
            TOPIC::AMMO_EMPTY,
            CHANNEL::Player,
            nullptr,
            nullptr,
            {},
            {}
        };

        CEventBus::GetInstance()->publish(event);
    }
}

void IWeaponState::Skill_Lock()
{
    if (m_iWeaponLevel >= 2) m_bWeaponSkill = true;
    else m_bWeaponSkill = false;
}


void IWeaponState::CalculateFireDirection(const _vec3& muzzleWorldPos, _vec3& outDir)
{
    _vec3 fwd, up, right;
    GetCameraBasis(fwd, up, right);
    outDir = fwd; // 카메라가 보는 방향 그대로
}


void IWeaponState::CanFire(const float& fDeltaTime)
{
    m_fElapsedTime += fDeltaTime;

    if (m_fElapsedTime >= m_fFireRate && !m_bAmmoDepleted)
        m_bCanFire = true;
    else
        m_bCanFire = false;
}

bool IWeaponState::CanChargeFire(const float& fDeltaTime)
{
    m_fChargeElapsedTime += fDeltaTime;

    if (m_fChargeElapsedTime >= m_FChargeFireRate)
    {
        m_bCanChargeFire = true;
        return true;
    }

    m_bCanChargeFire = false;
    return false;
}

void IWeaponState::MonsterBulletDirCalculator(CWeaponController* pWeapon, _vec3& Dir, const _vec3& vPos)
{
    if (!pWeapon) return;

    auto target = pWeapon->Get_MyOwner()->Get_Target();
    if (!target) return;

    auto targetPos = *(target->Get_Transform()->GetInfoPos());
    Dir = targetPos - vPos;
    D3DXVec3Normalize(&Dir, &Dir);
}

void IWeaponState::GetCameraBasis(_vec3& outFwd, _vec3& outUp, _vec3& outRight)
{
    D3DXMATRIX view = CCameraManager::GetInstance()->Get_MainViewMatrix();
    D3DXMATRIX invV; D3DXMatrixInverse(&invV, 0, &view);

    outRight = _vec3{ invV._11, invV._12, invV._13 };
    outUp = _vec3{ invV._21, invV._22, invV._23 };
    outFwd = _vec3{ invV._31, invV._32, invV._33 };

    D3DXVec3Normalize(&outRight, &outRight);
    D3DXVec3Normalize(&outUp, &outUp);
    D3DXVec3Normalize(&outFwd, &outFwd);
    
}
