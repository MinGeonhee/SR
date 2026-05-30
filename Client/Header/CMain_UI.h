#pragma once
#include "CUI.h"

class CSetting_UI;
class CUI_Number;
class CSelect_UI;

class CMain_UI :
    public CUI
{
private:
    explicit CMain_UI(const CMain_UI& rhs);
    explicit CMain_UI(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMain_UI();

public:
    static CMain_UI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    void Free() override;
    vector<CUI*> m_vecUIObjects;
    CSetting_UI* m_pSettingUI = nullptr;
    CSelect_UI* m_pSelectUI = nullptr;

    bool MainOpen = true;
    int m_hoverIndex = -1;
    int m_prevHover = -1;
};

