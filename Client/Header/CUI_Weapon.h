#pragma once
#include "CUI.h"
class CUI_Weapon :
    public CUI
{
private:
    explicit CUI_Weapon(const CUI_Weapon& rhs);
    explicit CUI_Weapon(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI_Weapon();

public:
    static CUI_Weapon* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void        Set_Toggle();

    Engine::CWeaponController* m_pWeaponCon;
    WEAPONSTATE m_eWeaponState = WEAPON_FIREWAND;

private:
    void Free() override;
    vector<CUI*> m_WeaponIcon;
    vector<CUI*> m_vecUIObjects;

    int WeaponBullet = 0;
    bool bActive = false;
    int m_hoverIndex = 0;

    bool bFireWand = false;
    bool bMace = false;
    bool bIceWand = false;
    bool bFireRing = false;
    /*bool bActive = false;*/
    //bool bActive = false;
    //bool bActive = false;
    //bool bActive = false;
};

