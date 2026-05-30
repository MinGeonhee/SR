#pragma once
#include "CUI.h"

class CUI_Image;
class CWeaponGet_UI :
    public CUI
{
private:
    explicit CWeaponGet_UI(const CWeaponGet_UI& rhs);
    explicit CWeaponGet_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CWeaponGet_UI();

public:
    static CWeaponGet_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Render_Text();
    void Set_Toggle();
    void UpdateText(const wstring& tag);
    void Render_ChildTex();

private:
    void Free() override;
    CUI_Image* m_pIcon = nullptr;
    bool bActive = true;
    bool m_prevActive = false;

    wstring m_cachedTag;
    wstring m_weaponName;
    wstring m_weaponLore;
    wstring m_weaponSummary;
};



