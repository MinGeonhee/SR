#pragma once
#include "CUI.h"

class CUI_Number;
class CPlayer;
class CWeaponController;
class CPlayerUI :
    public CUI
{
private:
    explicit CPlayerUI(const CPlayerUI& rhs);
    explicit CPlayerUI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CPlayerUI();

public:
    static CPlayerUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void Free() override;
    vector<CUI*> m_vecUIObjects;
    Engine::CWeaponController* m_pWeaponCon;
    int m_pPlayerMoney = 0;
    WEAPONSTATE m_eWeaponState;

};

