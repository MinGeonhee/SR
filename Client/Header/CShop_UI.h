#pragma once
#include "CUI.h"

class CPlayer;

class CShop_UI :
    public CUI
{
private:
    explicit CShop_UI(const CShop_UI& rhs);
    explicit CShop_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CShop_UI();

public:
    static CShop_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Render_ChildTex();

    void Set_WeaponInput();
    void Set_WeaponUI();

    void Set_SpellInput();
    void Set_SpellUI();

    void Set_CharacterInput();
    void Set_CharacterUI();

    void 	Set_Toggle();
private: 
    Engine::CWeaponController* m_pWeaponCon;
    WEAPONSTATE m_eWeaponState = WEAPON_FIREWAND;

    void Free() override;
    vector<CUI*> m_vecUIObjects;

    vector<CUI*> m_WeaponIcon;
    vector<CUI*> m_SpellIcon;
    vector<CUI*> m_CharIcon;

    list<int> m_BuyWeaponlist;
    list<int> m_BuySpelllist;
    list<int> m_BuyCharlist;

    POINT ptMouse;

    int m_Money;
    int m_hoverIndex = -1;
    int m_CategoryIndex = 0;
    ID3DXLine* m_pLine = nullptr;
};

