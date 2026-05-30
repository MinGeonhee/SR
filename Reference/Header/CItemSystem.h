#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CItem;
struct EVENT;

enum
{
    ITEMCOUNT = ITEMTYPE::ITEM_COUNT_END
};

class ENGINE_DLL CItemSystem :
    public CBase
{
    DECLARE_SINGLETON(CItemSystem)

public:
    explicit CItemSystem();
    virtual ~CItemSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

    // 내가 무기를 들고 있는가 없는가 bool값 판단
public:

    bool    IsWeaponEquipped(WEAPONSTATE weaponType);
    bool    HasKey(ITEMTYPE type); 
    void    UseKey(ITEMTYPE type);

private:

    void    ItemCollisionEvent();
    void    Pickup_StaticItem(EVENT event);
    void    Pickup_DynamicItem(EVENT event);

    void    HealItem(EVENT event);
    void    BulletItem(EVENT event, ITEMTYPE type, int count);
    void    AddMoney(EVENT event, int count);
    void    RegisterWeaponType(EVENT event, ITEMTYPE type);
    void    RegisterKeyType(EVENT event, ITEMTYPE type);

private:
    void    Free();
    
private:
    std::queue<CItem*>          m_pItemList[static_cast<size_t>(ITEMCOUNT)];
};

END