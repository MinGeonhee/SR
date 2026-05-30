#include "CWeaponController.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "IWeaponState.h"
#include "CProtoTypeManager.h"
#include "IWeapon_IceWand.h"
#include "IWeapon_FireWand.h"
#include "IWeapon_Gun.h"
#include "IWeapon_Goblin_FireBall.h"
#include "IWeapon_Goblin_Hammer.h"
#include "IWeapon_Rat.h"
#include "IWeapon_Boss_IceBall.h"
#include "IWeapon_Boss_IcePillar.h"
#include "IWeapon_Boss_Wind.h"
#include "IWeapon_Boss_Thunder_Drop.h"
#include "IWeapon_FireRing.h"
#include "IWeapon_Mace.h"
#include "IWeapon_Orge_Axe.h"
#include "IWeapon_Skeleton_Sword.h"
#include "IWeapon_Wizard.h"
#include "CItemSystem.h"
#include "CSoundManager.h"

CWeaponController::CWeaponController()
    : m_pCurstate(nullptr)
{
}

CWeaponController::CWeaponController(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev),
    m_pCurstate(nullptr)
{
}

CWeaponController::CWeaponController(const CWeaponController& rhs)
    : CComponent(rhs)
    , m_pCurstate(rhs.m_pCurstate)
    , m_eWeaponState(rhs.m_eWeaponState)
    , m_mWeapon(rhs.m_mWeapon)
{
}

CWeaponController::~CWeaponController()
{
}

HRESULT CWeaponController::Ready_Weapon()
{
    // 초기 무기는 Weapon_mace 입니다아
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_MACE,                    new IWeapon_Mace() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_GUN,                      new IWeapon_Gun() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_FIRERING,               new IWeapon_FireRing() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_ICEWAND,                  new IWeapon_IceWand() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_FIREWAND,                 new IWeapon_FireWand() });

    //m_mWeapon.insert({ WEAPONSTATE::WEAPON_TRAP_FIREBALL,           new ICam_ToolMode() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_GOBLIN_FIREBALL,          new IWeapon_Goblin_FireBall() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_ORGE_AXE,          new IWeapon_Orge_Axe() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_GOBLIN_HAMMER,          new IWeapon_Goblin_Hammer() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_RAT,          new IWeapon_Rat() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_SKELETON_SWORD,          new IWeapon_Skeleton_Sword() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_WIZARD,          new IWeapon_Wizard() });


    m_mWeapon.insert({ WEAPONSTATE::WEAPON_BOSS_ICEBALL,          new IWeapon_Boss_IceBall() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_BOSS_ICEPILLAR,          new IWeapon_Boss_IcePillar() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_BOSS_THUNDER_DROP,          new IWeapon_Boss_Thunder_Drop() });
    m_mWeapon.insert({ WEAPONSTATE::WEAPON_BOSS_WIND,          new IWeapon_Boss_Wind() });
    //m_mWeapon.insert({ WEAPONSTATE::WEAPON_SKELLETON_SILVER_ARROW,  new ICam_EffectToolMode() });
    //m_mWeapon.insert({ WEAPONSTATE::WEAPON_SKELLETON_GOLD_ARROW,    new ICam_EffectToolMode() });

    // 초기 무기 상태값 설정. (현재 IceWand만 있어서..)
    auto iter = m_mWeapon.find(WEAPON_MACE);
    m_pCurstate = (*iter).second;
    m_eWeaponState = WEAPON_MACE;

    return S_OK;
}

void CWeaponController::Weapon_Inintialize()
{
    for (auto weapon : m_mWeapon)
        weapon.second->Initialize(this);
}

void CWeaponController::Change_Weapon(WEAPONSTATE eState)
{
    if (eState != WEAPON_MACE && CItemSystem::GetInstance()->IsWeaponEquipped(eState) == false)
        return;
    m_pCurstate->Exit(this);

    auto iter = m_mWeapon.find(eState);
    m_pCurstate = (*iter).second;
    m_eWeaponState = eState;

    m_pCurstate->Enter(this);
}

_int CWeaponController::Update_Component(const _float& fTimeDelta)
{
    if (m_pMyOwner == NULL)
        return 0;

    if (m_pCurstate)
        m_pCurstate->Update(this, fTimeDelta);

    return 0;
}

void CWeaponController::LateUpdate_Component(const _float& fTimeDelta)
{
}

void CWeaponController::Fire()
{
    if (m_pCurstate)
        m_pCurstate->Fire(this);
    
}

void CWeaponController::ChargeFire(const float& fTimeDelta)
{
    // 무기 차징중... KeyPress일 때.
    if (m_pCurstate)
    {
        m_pCurstate->CanChargeFire(fTimeDelta);
        m_pCurstate->Overdrive_Fire(this);
    }

}

void CWeaponController::Overdrive_Fire()
{
    // keyup일때.
    if (m_pCurstate)
    {
        m_pCurstate->ChargeFire(this);
        m_pCurstate->Set_OverideFire(true);
        m_pCurstate->ChargeElapsedTime_reset();
    }
}

int CWeaponController::Get_Bullet()
{
    if (m_pCurstate)
    {
        return m_pCurstate->Get_BulletCount();
    }
}

int CWeaponController::Get_Bullet(WEAPONSTATE type)
{
    if (m_pCurstate)
    {
        return m_mWeapon[type]->Get_BulletCount();
    }
}

bool CWeaponController::CanPickUpBullet(ITEMTYPE type)
{
    switch (type)
    {
        case ITEMTYPE::ITEM_FIRE_SHARD:
            return m_mWeapon[WEAPONSTATE::WEAPON_FIRERING]->CanPickupBullet();
           
        case ITEMTYPE::ITEM_ICE_SHARD:
            return m_mWeapon[WEAPONSTATE::WEAPON_ICEWAND]->CanPickupBullet();
            
        case ITEMTYPE::ITEM_GUN_BULLET:
            return m_mWeapon[WEAPONSTATE::WEAPON_GUN]->CanPickupBullet(); 
    }
}

void CWeaponController::Add_WeaponBullet(ITEMTYPE type, int count)
{
    switch (type)
    {
    case ITEMTYPE::ITEM_FIRE_SHARD:
         m_mWeapon[WEAPONSTATE::WEAPON_FIRERING]->ADD_BulletCount(count);
         break;

    case ITEMTYPE::ITEM_ICE_SHARD:
         m_mWeapon[WEAPONSTATE::WEAPON_ICEWAND]->ADD_BulletCount(count);
         break;

    case ITEMTYPE::ITEM_GUN_BULLET:
         m_mWeapon[WEAPONSTATE::WEAPON_GUN]->ADD_BulletCount(count);
         break;

    case ITEMTYPE::ITEM_LOW_MANA:
        m_mWeapon[WEAPONSTATE::WEAPON_MACE]->ADD_EnergyBullet(count);
        m_mWeapon[WEAPONSTATE::WEAPON_FIREWAND]->ADD_EnergyBullet(count);
        break;

    }
}

void CWeaponController::LevelUp(WEAPONSTATE tag)
{
    auto it = m_mWeapon.find(tag);
    if (it == m_mWeapon.end() || it->second == nullptr) return;

    it->second->Level_Up();
}

CComponent* CWeaponController::Clone()
{
    return Create(m_pGraphicDev);
}

CWeaponController* CWeaponController::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWeaponController* pInstance = new CWeaponController(pGraphicDev);
    if (FAILED(pInstance->Ready_Weapon()))
    {
        delete pInstance;
        return nullptr;
    }
    return pInstance;
}

void CWeaponController::Free()
{
    for_each(m_mWeapon.begin(), m_mWeapon.end(), CDeleteMap());
    m_mWeapon.clear();

    CComponent::Free();
}
